#pragma once
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <transform.hpp>
namespace gls {
    using std::vector;
    class model {
    public:
        enum color_type {
            none,
            color,
            texture
        };
    private:
        using vertex_t = glm::vec3;
        using index_t = unsigned int;
        using color_t = glm::vec4;
        using uv_t = glm::vec2;
        vector<vertex_t> vertex;
        vector<index_t> index;
        vector<color_t> colors;
        vector<uv_t> uvs;
        GLuint VBO, VIO;
        color_type type;
        // TODO: might use texture later.
    public:
        model()=default;
        model(const float* vert, size_t n_vert, const index_t* inde, size_t n_inde): 
            vertex(vert, vert+(void*)(n_vert*sizeof(vertex_t))),
            index(inde, inde+(void*)(n_inde*sizeof(index_t))), type(color_type::none)  { }
        model(const float* vert, size_t n_vert, const index_t* inde, size_t n_inde, const float* color): 
            vertex(vert, vert+(void*)(n_vert*sizeof(vertex_t))),
            index(inde, inde+(void*)(n_inde*sizeof(index_t))),
            colors(color, color+(void*)(n_vert*sizeof(color_t))), type(color_type::color) { }
        model(const float* vert, size_t n_vert, const index_t* inde, size_t n_inde, const float* uv): 
            vertex(vert, vert+(void*)(n_vert*sizeof(vertex_t))),
            index(inde, inde+(void*)(n_inde*sizeof(index_t))),
            uvs(uv, uv+(void*)(n_vert*sizeof(uv_t))), type(color_type::texture) { }
        model(const std::string& file) {
            this->load(file);
        }
        ~model() {
            this->free_buffer();
        }
        static model from_file(const std::string& file) {model new_mod; new_mod.load(file); return new_mod;}
        void load(const std::string& file);             // TODO
        
        void free_buffer() {
            // This is automatically called when destroyed. If you changed the model after binding it to buffer, you need to call it manually and allocate buffer again.
        }
        void to_buffer() {
            // blend these together
            GLuint handles[2];
            glGenBuffer(2, handles);
            VBO = handles[0];
            VIO = handles[1];

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t)*vertex.size(), &vertex[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_t)*index.size(), &index[0], GL_STATIC_DRAW);
        }
        void draw_model() {
            // remember to use correct shader, depending on its mode.
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            // TODO: deal with others, such as 

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
            glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
            glDisableVertexAttribArray(0);
        }
    };

    

}