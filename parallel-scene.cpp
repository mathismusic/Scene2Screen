#include "essentials.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"

#include <iostream>
#include <thread>
#include <cmath>
#include <mutex>

hittable_list random_scene()
{
    hittable_list world;
    auto ground_material = std::make_shared<Lambertian>(RGBcolor(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(point3d{0, -1000, 0}, 1000, ground_material));
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = sampler.random_double();
            point3d center{a + 0.9 * sampler.random_double(), 0.2, b + 0.9 * sampler.random_double()};
            if ((center - point3d{4, 0.2, 0}).length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;
                if (choose_mat < 0.8)
                {
                    // diffuse
                    RGBcolor random_color = RGBcolor(sampler.random_vector(0., 1.));
                    auto albedo = random_color * random_color;
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = RGBcolor(sampler.random_vector(0.5, 1));
                    auto fuzz = sampler.random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    // RGBcolor random_color = RGBcolor(sampler.random_vector(0., 1.));
                    // auto albedo = random_color * random_color;
                    sphere_material = std::make_shared<Dielectric>(1.0, 1.5, 1.0);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    auto material1 = std::make_shared<Dielectric>(1.0, 1.5, 1.0);
    world.add(std::make_shared<Sphere>(point3d(0, 1, 0), 1.0, material1));
    auto material2 = std::make_shared<Lambertian>(RGBcolor(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(point3d(-4, 1, 0), 1.0, material2));
    auto material3 = std::make_shared<Metal>(RGBcolor(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(point3d(4, 1, 0), 1.0, material3));
    return world;
}

// the goal is to compute the color of the ray at each pixel.

RGBcolor ray_color(const Ray &r, const hittable_list &h, int stackdepth)
{
    if (stackdepth <= 0)
    {
        return {0, 0, 0}; // low probability event, return whatever (contributes v.less to averaged pixel color)
    }
    hit_record rec;
    if (h.hit(r, 0.0001, INFINITY, rec))
    {
        // it actually hits. Find an incident vector and trace that back
        RGBcolor attenuation;
        Ray incident;
        if (rec.mat_ptr->scatter(r, rec, attenuation, incident))
        {
            return attenuation * ray_color(incident, h, stackdepth - 1);
        }
    }
    // default
    double y_coord = unit_vector(r.direction()).y();
    double t = abs((1 + y_coord) / 2);
    return (1 - t) * WHITE + t * RGBcolor(0.7, 0.8, 1.0);
}

std::vector<RGBcolor> pixels;
std::mutex console_mutex; // mutex for thread terminal access

void render(int thread_id, const Camera &cam, const hittable_list &world, int image_width, int image_height, int start_n, int end_n, int n_samples)
{
    for (int idx = start_n; idx < end_n; idx++) {
        RGBcolor cum_color;
        for (int _ = 0; _ < n_samples; ++_)
        {
            int i = idx%image_width;
            int j = image_height-1 - idx/image_width;
            double u = (i + sampler.random_double()) / (image_width - 1);
            double v = (j + sampler.random_double()) / (image_height - 1);
            Ray r = cam.get_ray(u, v);
            cum_color += ray_color(r, world, 50);
        }

        pixels[idx] = cum_color/n_samples;
        if ((idx - start_n) % 100 == 0) {
            int progress = ((idx - start_n) * 100) / (end_n - start_n);
            int bar_width = 30; // Width of the progress bar
            int filled = (progress * bar_width) / 100;
            int empty = bar_width - filled;

            std::lock_guard<std::mutex> lock(console_mutex);
            std::cerr << "\033[" << (thread_id + 1) << ";0H"  // Move to thread's line
                      << "Thread " << thread_id + 1 << ": ["
                      << std::string(filled, '#')  // Filled part
                      << std::string(empty, ' ')  // Empty part
                      << "] " << progress << "%\033[K" << std::flush;
        }
    }

    // Final update
    std::lock_guard<std::mutex> lock(console_mutex);
    std::cerr << "\033[" << (thread_id + 1) << ";0H"
              << "Thread " << thread_id + 1 << ": ["
              << std::string(30, '#')  // Fully filled bar
              << "] 100%\033[K" << std::flush;
}

void output(int image_width, int image_height, const std::vector<RGBcolor> &pixels) {
    std::cerr << "Writing image to stdout...";
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int idx = 0; idx < image_width * image_height; idx++) {
        write_color(std::cout, pixels[idx]);
    }
    std::cerr << "Done.\n";
}


int main()
{
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height= int(0.5 + image_width/aspect_ratio);
    point3d lookfrom{13, 2, 3}, lookat{0, 0, 0};
    Camera cam(lookfrom, lookat, {0, 1, 0}, aspect_ratio, 20.0, 10.0, 0.1);
    auto world = random_scene();

    const int n_threads = 16;
    const int N = image_width * image_height;
    const int n_per_thread = N/n_threads + N%n_threads;
    
    std::thread threads[n_threads];
    pixels = std::vector<RGBcolor>(N);
    
    // Clear terminal and set up status lines
    std::cerr << "\033[2J"; // Clear screen
    for (int i = 0; i < n_threads; i++) {
        std::cerr << "\033[" << (i + 1) << ";0H"
                  << "Thread " << i + 1 << ": ["
              << std::string(30, '#')  // Fully filled bar
              << "] 0%\033[K" << std::flush;
    }
    
    for (int i = 0; i < n_threads; i++) {
        int start_idx = i * n_per_thread;
        int end_idx = (i == n_threads - 1) ? N : (i + 1) * n_per_thread;
        threads[i] = std::thread(render, i, std::cref(cam), std::cref(world), image_width, image_height, start_idx, end_idx, 50);
    }
    for (auto &thread: threads) thread.join();
    std::cerr << "\033[" << (n_threads + 1) << ";0HDone rendering!\n";
    output(image_width, image_height, pixels);
}