#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "hittable.hpp"
#include <vector>
#include <memory>

class hittable_list: public hittable {
    std::vector<std::shared_ptr<hittable>> objects;
public:
    hittable_list() {}
    hittable_list(std::shared_ptr<hittable> object) {
        add(object);
    }
    
    void add(std::shared_ptr<hittable> object) { objects.push_back(object); }
    void clear() { objects.clear(); }

    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool hittable_list::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    bool hit_anything = false;
    for (const std::shared_ptr<hittable> &object: objects) {
        hit_record tmp;
        if (object->hit(r, t_min, t_max, tmp)) {
            hit_anything = true;
            t_max = tmp.t;
            rec = tmp;
        }
    }
    return hit_anything;
}
#endif