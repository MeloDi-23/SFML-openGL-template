#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

#include <transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <camera.hpp>
extern float ratio;


GLint model_matrix_loc, cam_matrix_loc;
GLuint VBO[2], VIO[2];

glm::f32mat4x4 model_mat(1.0f), view_mat(1.0f), proj_mat(1.0f);
gls::transform model;
gls::camera camera;

GLint load_shader();
void draw_openGL();
GLuint shader_from_file(const char* file, GLuint type);

GLint load_shader() {
    GLint ret = 0;
    auto shader_program = glCreateProgram();
    if(!shader_program) {
        std::cerr << "fail to create a program.";
        ret = -1;
    }
    auto shader_vs = shader_from_file("assets/shader/vs.glsl", GL_VERTEX_SHADER);
    auto shader_fs = shader_from_file("assets/shader/fs.glsl", GL_FRAGMENT_SHADER);
    glAttachShader(shader_program, shader_vs);
    glAttachShader(shader_program, shader_fs);
    glLinkProgram(shader_program);
    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        ret = -2;
    }

    model_matrix_loc = glGetUniformLocation(shader_program, "model");
    cam_matrix_loc = glGetUniformLocation(shader_program, "cam");

    glUseProgram(shader_program);

    return ret;
}
GLuint shader_from_file(const char* file, GLuint type) {
    auto shader = glCreateShader(type);
    std::stringstream buffer;
    std::ifstream fstream(file, std::ios::in);
    if(!fstream.good()) {
        std::cerr << "file not exist.";
        return -1;
    }
    buffer << fstream.rdbuf();
    auto str = buffer.str();
    GLint length[1] = {(GLint)(str.size())};
    const GLchar* p[1] = {str.c_str()};

    glShaderSource(shader, 1, p, length);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(shader, sizeof(InfoLog), NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", type, InfoLog);
        std::cout << str;
        return -1;
    }
    return shader;
}

void draw_openGL(float t) {
    glUniformMatrix4fv(cam_matrix_loc, 1, GL_FALSE, glm::value_ptr(camera.camera_matrix()));

    // model.rotate_local(glm::vec3(glm::radians(0.09f*t), 0.f, 0.f));
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model.model_matrix()));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO[0]);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glLineWidth(1.f);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO[1]);
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);



    glLineWidth(4.f);
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO[1]);
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    /**
     * Draw Array from the buffer which VBO points to.
     * glBindBuffer interperete VBO as an array buffer and bring it to table;
     * glVertexAttribPointer interprete the buffer at a given offset and size;
     *  glVertexAttribPointer(index, size, type, normalize, strip, pointer);
     *  means set pointer at `index` as a series of `type` with `size`, and `strip` is the space between two elements.
     *      In the demo above, this means: interprete a float array[3], donot normalize, 
     *      and the space between two arrays is 0, and set the pointer to pointer No.0
     * glDrawArrays do the drawing.
     * glEnable/DisableVertexAttribArray warps the whole process.
    */
}

void init_gl() {
    glewExperimental = GL_TRUE;  // Enable experimental extensions
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Set up OpenGL state
    glEnable(GL_DEPTH_TEST);    // Enable depth testing for 3D rendering
    glDepthFunc(GL_LEQUAL);     // Depth test will accept closer values
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Clear color: dark gray background
}

void prepare_models() {
    // hard coded models and buffer creation.
    glm::vec3 vertex[] = {
        {1, 0, 0},
        {0, 0.5, 0},
        {0, 0, 0},
        {0, 0, 1}
    };

    glm::vec3 axis[] = {
        {0, 0, 0},
        {4, 0, 0},
        {0, 4, 0},
        {0, 0, 4}
    };

    unsigned int index[] = {
        0, 2, 1,
        0, 1, 3,
        0, 3, 2,
        1, 2, 3
    };

    unsigned int index_axis[] = {
        0, 1,
        0, 2, 
        0, 3
    };

    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), &vertex[0], GL_STATIC_DRAW);

    glGenBuffers(2, VIO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis), &axis[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_axis), index_axis, GL_STATIC_DRAW);

}

void prepare_cams() {
    model.rotate(glm::vec3(0.f, 0.f, glm::radians(40.f)));
    camera.move_to(glm::vec3(5.f, 5.f, 5.f));
    camera.look_at(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
    camera.set_perspective(glm::radians(45.f), ratio, 0.2f, 50.f);
}

void delete_resources() {
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(2, VIO);
}