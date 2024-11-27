#include <model.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
namespace gls {
    gls::model::model(const float* pos, size_t n, const index_t* idx, size_t n_id): meshes(0) {
        sub_mesh mesh;
        for (size_t i = 0; i != n; ++i) {
            mesh.vertex.emplace_back(vertex_t(pos[i*3], pos[i*3+1], pos[i*3+2]), uv_t(0.f), norm_t(0.f));
        }
        mesh.index = std::vector<index_t>(idx, idx+n_id);

        // for (size_t i = 0; i != n; ++i) {
        //     std::cout << glm::to_string(mesh.vertex[i].vertex) << std::endl;
        // }
        meshes.push_back(std::move(mesh));
        
        VBOs = new GLuint[1];
        VIOs = new GLuint[1];
    }
    bool gls::model::load(const std::string& s) {

        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(s.c_str(), model::flag);
        if (!scene) {
            std::cerr << "Error loading model from " << s << ": " << importer.GetErrorString();
            return false;
        }
        auto nmesh = scene->mNumMeshes;
        PRINT_VALUE(nmesh);

        meshes.resize(nmesh);
        VBOs = new GLuint[nmesh];
        VIOs = new GLuint[nmesh];

        const aiMesh* mesh;
        for(size_t i = 0; i != nmesh; ++i) {
            mesh = scene->mMeshes[i];
            sub_mesh s_mesh;
            s_mesh.read_mesh(mesh);
            meshes[i] = s_mesh;
        }
        return true;
    }

    void gls::model::sub_mesh::read_mesh(const aiMesh* mesh) {
        // read mesh from struct pointer, and use attribute vertex vector to store it.
        aiVector3D pos, norm;
        float u, v;
        if (!mesh->HasTextureCoords(0))
            throw "at least one uv channel!";
        index.resize(mesh->mNumFaces*3);        // all face is triangle.
        const aiFace* face;
        for (size_t i = 0; i != mesh->mNumFaces; ++i) {
            face = mesh->mFaces+i;
            if (face->mNumIndices != 3) {
                throw "model not triangle devided.";
            }
            index[i*3]   = face->mIndices[0];
            index[i*3+1] = face->mIndices[1];
            index[i*3+2] = face->mIndices[2];
        }

        for (size_t i = 0; i != mesh->mNumVertices; ++i) {
            pos = mesh->mVertices[i];
            if (mesh->mNormals) {
                norm = mesh->mNormals[i];
            } else {
                norm = aiVector3D(0.f);
            }
            u = mesh->mTextureCoords[0][i].x;
            v = mesh->mTextureCoords[0][i].y;

            vertex.emplace_back(pos.x, pos.y, pos.z, u, v, norm.x, norm.y, norm.z);
        }
        // std::cout << "z " << std::max_element(vertex.cbegin(), vertex.cend(), [](const attribute_vertex& a, const attribute_vertex& b) {return a.vertex.z < b.vertex.z;})->vertex.z << ' ' <<\
          std::max_element(vertex.cbegin(), vertex.cend(), [](const attribute_vertex& a, const attribute_vertex& b) {return a.vertex.z > b.vertex.z;})->vertex.z << std::endl;
        
        // std::cout << "x " << std::max_element(vertex.cbegin(), vertex.cend(), [](const attribute_vertex& a, const attribute_vertex& b) {return a.vertex.x < b.vertex.x;})->vertex.x << ' ' <<\
          std::max_element(vertex.cbegin(), vertex.cend(), [](const attribute_vertex& a, const attribute_vertex& b) {return a.vertex.x > b.vertex.x;})->vertex.x << std::endl;
        
        // std::cout << "y " << std::max_element(vertex.cbegin(), vertex.cend(), [](const attribute_vertex& a, const attribute_vertex& b) {return a.vertex.y < b.vertex.y;})->vertex.y << ' ' <<\
          std::max_element(vertex.cbegin(), vertex.cend(), [](const attribute_vertex& a, const attribute_vertex& b) {return a.vertex.y > b.vertex.y;})->vertex.y << std::endl;
    }

    void gls::model::sub_mesh::bind_buffer(const GLuint vbo, const GLuint vio) {
        VBO = vbo;
        VIO = vio;
        assert(sizeof(attribute_vertex) == 8*sizeof(float));

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertex.size()*sizeof(attribute_vertex), &vertex[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size()*sizeof(index_t), &index[0], GL_STATIC_DRAW);


        vertex_size = vertex.size();
        index_size = index.size();
        vertex.clear();         // clean the vector to release space in CPU.
        index.clear();
    }

    void gls::model::sub_mesh::draw_mesh() const {
        if(VBO == 0 || VIO == 0) {
            throw "call bind buffer first!";
        }
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(attribute_vertex), nullptr);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(attribute_vertex), (const void*)(3*sizeof(float)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(attribute_vertex), (const void*)(5*sizeof(float)));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
        assert(index_size > 0);

        // PRINT_VALUE(index_size);
        glDrawElements(GL_TRIANGLES, index_size, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }

}