#ifndef BVH_H
#define BVH_H

#include "hittable.h"
#include "hittable_list.h"

#include <memory>
#include <algorithm>

class bvh_node : public hittable {
public:
    bvh_node(const hittable_list& list)
        : bvh_node(list.object_list, 0, list.object_list.size()) {}
    bvh_node(const std::vector<std::shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
        auto objects = src_objects;

        int axis = random_int(0,2);
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                                      : box_z_compare;
        size_t object_span = end - start;

        if(object_span == 1) {
            left = right = objects[start];
        } else if(object_span == 2) {
            if(comparator(objects[start], objects[start+1])) {
                left = objects[start];
                right = objects[start+1];
            } else {
                left = objects[start+1];
                right = objects[start];
            }
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            left = std::make_shared<bvh_node>(objects, start, mid);
            right = std::make_shared<bvh_node>(objects, mid, end);
        }

        bbox = aabb(left->bounding_box(), right->bounding_box());
    }

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const {        
        if(!bbox.hit(r, interval(t_min, t_max)))
            return false;
        
        bool hit_left = left->
            hit(r, t_min, t_max, rec);
        bool hit_right = right->
            hit(r, t_min, hit_left ? rec.t : t_max, rec);

        return hit_left || hit_right;
    }
    
    aabb bounding_box() const override { return bbox; }
private:
    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb bbox;

    static bool box_compare(
        const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis
    ) {
        return a->bounding_box().axis(axis).min < b->bounding_box().axis(axis).min;
    }

    static bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return box_compare(a, b, 2);
    }
};

#endif