#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "essentials.hpp"
#include "hittable.hpp"

class Material {
public:
    // how the material scatters an incident ray (in reverse). i.e. what incident ray could scatter into reflected. Results stored in attenuation and scattered. Of course, since many incidents could have scattered resulting in `reflected`, this function just samples from one possibility. 
    virtual bool scatter(const Ray &reflected, const hit_record &rec, RGBcolor &attenuation, Ray &incident) const = 0;
    virtual ~Material(){}
};

// diffuse material
class Lambertian: public Material {
    RGBcolor albedo; // how much is reflected [0,1].
public:
    Lambertian(const RGBcolor &albedo): albedo(albedo) {}

    virtual bool scatter(const Ray& reflected, const hit_record& rec, RGBcolor& attenuation, Ray& incident) const override {
        Vec3 incident_dirn = rec.normal + sampler.random_unit_vector();
        if (incident_dirn == 0) incident_dirn = rec.normal;
        incident = Ray(rec.p, incident_dirn);
        attenuation = albedo;
        return true;
    }
};

// specular material (reflects)
class Metal: public Material {
    RGBcolor albedo;
    double fuzz;
public:
    Metal(const RGBcolor &albedo, double fuzz): albedo(albedo), fuzz(std::min(fuzz, 1.0)) {}

    virtual bool scatter(const Ray& reflected, const hit_record& rec, RGBcolor& attenuation, Ray& incident) const override {
        Vec3 incident_dirn = reflect(reflected.direction(), rec.normal) + fuzz * sampler.random_unit_vector();
        incident = Ray(rec.p, incident_dirn);
        attenuation = albedo;
        return dot(incident_dirn, rec.normal) > EPS;
    }
};

// reflection and refraction (dielectrics). Reflection is assumed to not be diffuse
class Dielectric: public Material {
    RGBcolor albedo;
    double refr_index;
    double refr_likelihood;
public:
    Dielectric(const RGBcolor &albedo, double refr_index, double refr_likelihood): albedo(albedo), refr_index(std::max(refr_index, 1.0)), refr_likelihood(clamp(refr_likelihood, 0, 1)) {}

    virtual bool scatter(const Ray& reflected, const hit_record& rec, RGBcolor& attenuation, Ray& incident) const override {
        bool do_refract = (sampler.random_double() < refr_likelihood);
        if (do_refract) {
            // refractionn/TIR
            attenuation = albedo;
            incident = Ray(rec.p, refract(reflected.direction(), rec.normal, (rec.front_face?refr_index:1/refr_index)));
            return true;
        } else {
            // behave like a metal (reflect)
            attenuation = albedo;
            incident = Ray(rec.p, reflect(reflected.direction(), rec.normal));
            return true;
        }
    }
};

#endif