#pragma once
#include "transform.hpp"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
/**
 * A simple camera class.
 * support moving the camera, rotating the camera(in local frame), and directing the camera. 
 * also support setting perspective parameter and generate the camera matrix (project*view).
 * utilizes glm quat and matrix to do the calculation.
*/

namespace gls {     // gl simple.
    class camera {
    private:
        float fov;
        float ratio;
        float znear, zfar;
        glm::vec3 pos, dir, up;
    public:
        camera()=default;
        camera(float fov, float ratio, float znear, float zfar,
            const glm::vec3 & pos, const glm::vec3 & tar, const glm::vec3 & up): 
            fov(fov), ratio(ratio), znear(znear), zfar(zfar),
            pos(pos){
                dir = glm::normalize(tar - pos);
                this->up = glm::normalize(up-glm::dot(dir, tar)*dir);
            }
        const glm::mat4 view_matrix() const {
            // Note that the view mat is inverse of common model matrix.
            return glm::lookAt(pos, pos+dir, up);
        }
        const glm::mat4 proj_matrix() const {
            return glm::perspective(fov, ratio, znear, zfar);
        }
        const glm::mat4 camera_matrix() const {
            return proj_matrix() * view_matrix();
        }
        const glm::vec3 position() const {
            return this->pos;
        }

        void set_perspective(float fov, float ratio, float znear, float zfar) {
            this->fov=fov;
            this->ratio=ratio;
            this->znear=znear;
            this->zfar=zfar;
        }

        void move_to(const glm::vec3 dis) {
            pos = dis;
        }

        void move(const glm::vec3 dis) {
            pos += dis;
        }

        void look_at(const glm::vec3 target, const glm::vec3 up) {
            dir = glm::normalize(target - pos);
            this->up = glm::normalize(up-glm::dot(dir, target)*dir);
        }

        // rotate the camera motion seperately.
        void yaw(float ang) {
            dir = glm::rotate(glm::quat(1.f, 0.f, 0.f, 0.f), -ang, up)*dir;
        }
        void roll(float ang) {
            up = glm::rotate(glm::quat(1.f, 0.f, 0.f, 0.f), ang, dir)*up;
        }
        void pitch(float ang) {
            auto s = glm::cross(up, dir);
            auto rot = glm::rotate(glm::quat(1.f, 0.f, 0.f, 0.f), -ang, s);
            dir = rot*dir;
            up = rot*up;
        }
    };
}
