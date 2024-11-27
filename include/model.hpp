#pragma once
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

#include <util.h>

namespace gls {
    using std::vector;
    class model {
    private:
        using vertex_t = glm::vec3;
        using index_t = unsigned int;
        using color_t = glm::vec4;
        using norm_t = glm::vec3;
        using uv_t = glm::vec2;

        struct attribute_vertex { // vertex with attribute combined together.
            vertex_t vertex;
            uv_t uv;
            norm_t norm;
            attribute_vertex(const vertex_t& v, const uv_t& uv, const norm_t& n): vertex(v), uv(uv), norm(n) {}
            attribute_vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz): \
                vertex(x, y, z), uv(u, v), norm(nx, ny, nz) {}
        };

        struct sub_mesh {
            // sub_mesh stores the message about a vertex.
            vector<attribute_vertex> vertex;
            vector<index_t> index;
            size_t vertex_size, index_size;
            GLuint VBO, VIO;
            sub_mesh() = default;
            void read_mesh(const aiMesh*);
            void bind_buffer(const GLuint vbo, const GLuint vio);
            void draw_mesh() const;
        };

        const static int flag =  aiProcess_CalcTangentSpace        |
                            aiProcess_Triangulate            |
                            aiProcess_JoinIdenticalVertices  |
                            aiProcess_SortByPType;

        vector<sub_mesh> meshes;
        GLuint *VBOs, *VIOs;
    public:
        model()=default;
        model(const float* pos, size_t n, const index_t* index, size_t n_id);
        // construct a model with specified vertex. just for debuging.
        model(const std::string& file) {
            this->load(file);
        }
        ~model() {
            this->free_buffer();
            delete VBOs;
            delete VIOs;
        }
        static model from_file(const std::string& file) {model new_mod; new_mod.load(file); return new_mod;}
        bool load(const std::string& file);

        void free_buffer() {
            // This is automatically called when destroyed.
            glDeleteBuffers(meshes.size(), VBOs);
            glDeleteBuffers(meshes.size(), VIOs);
        }
        void bind_buffer() {
            glGenBuffers(meshes.size(), VIOs);
            glGenBuffers(meshes.size(), VBOs);
            for(size_t i = 0; i != meshes.size(); ++i) {
                meshes[i].bind_buffer(VBOs[i], VIOs[i]);
            }
        }
        void draw_model() const {
            for(const auto& mesh: meshes) {
                mesh.draw_mesh();
            }
        }
    };

    class model_pool {

    };

}