#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include <memory>

#include "hittable.h"

class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> object_list;

    hittable_list() = default;
    hittable_list(std::shared_ptr<hittable> object) { add(object); }

    void add(std::shared_ptr<hittable> object) {
        object_list.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    void clear() { object_list.clear(); }

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        hit_record temp_rec;
        double closest_t = t_max;
        bool hit_anything = false;

        for(auto object : object_list) {
            if(object->hit(r, t_min, closest_t, temp_rec)) {
                closest_t = temp_rec.t;
                rec = temp_rec;
                hit_anything = true;
            }
        }

        return hit_anything;
    }

    aabb bounding_box() const override { return bbox; }
private:
    aabb bbox;
};

#endif