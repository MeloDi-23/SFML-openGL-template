#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <memory>
namespace gls {     // make gl simple.
    class transform {
    private:
        glm::vec3 pos;
        glm::quat rotation;
        glm::vec3 sca;

    public:
        transform(): pos(0), rotation(glm::vec3(0.f)), sca(1.f) { }
        transform(const glm::vec3& pos, const glm::vec3& ang, const float scale, const transform* const parent): 
        pos(pos), rotation(ang), sca(scale){ }
        const glm::mat4 model_matrix() const {
            auto model = glm::scale(glm::mat4(1.f), sca);
            auto rotate = glm::toMat4(this->rotation);
            model = rotate*model;
            model = glm::translate(model, this->pos);

            return model;
        }
        
        void set_pos(const glm::vec3& pos) {this->pos = pos;}
        void move(const glm::vec3& dis) {this->pos += dis;}
        void move_local(const glm::vec3& dis) {
            this->pos += this->rotation*dis;
        }
        // these are all in global, which means the rotation are aligned in global(parent) XYZ.
        void rotate_to(const glm::quat& ang) {this->rotation = glm::normalize(ang);}
        void rotate_to(const glm::vec3& ang) {this->rotation = glm::quat(ang);}
        void rotate_to(const glm::mat4& mat) {this->rotation = glm::quat(mat);}
        void rotate(const glm::vec3& ang) {this->rotation = glm::quat(ang)*this->rotation;}
        void rotate(const glm::quat& ang) {this->rotation = ang*this->rotation;}
        void rotate(const glm::mat4& mat) {this->rotation = glm::quat(mat)*this->rotation;}

        // This uses local axis, which means the rotation 
        void rotate_local(const glm::vec3& ang) {this->rotation = this->rotation*glm::quat(ang);}
        void rotate_local(const glm::quat& ang) {this->rotation = this->rotation*ang;}
        void rotate_local(const glm::mat4& mat) {this->rotation = this->rotation*glm::quat(mat);}

        void scale_to(const glm::vec3& s) {sca = s;}
        void scale(const glm::vec3& s) {sca *= s;}

        const glm::vec3 position() const {return pos;}
        const glm::vec3 scale() const {return sca;}
        const glm::quat quat() const {return rotation;}
        const glm::vec3 euler_angle() const {return glm::eulerAngles(this->rotation);}
    };

}
