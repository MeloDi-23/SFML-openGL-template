#include <transform.hpp>
#include <model.hpp>
#include <glm/glm.hpp>
namespace gls {
    class object {
        // The actual game object(consists of real models and transforms.)
    public:
        using obj_ptr = const object*;
        using mod_ptr = const model*;
        transform transform;
        mod_ptr model;
        obj_ptr parent;

        object(): model(), parent(nullptr) {};
        object(const gls::transform& trans, const mod_ptr mod, const obj_ptr par=nullptr): transform(trans), model(mod), parent(par) {}
        void draw(const GLint model_matrix_loc) const {
            // TODO: care about texture and UV.
            auto matrix = transform.model_matrix();
            if (parent) {
                matrix = parent->transform.model_matrix()*matrix;
            }
            glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(matrix));

            model->draw_model();
        }
    };
}//namespace gls