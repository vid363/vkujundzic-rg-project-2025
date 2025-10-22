
#include "glad/glad.h"


#include <engine/resources/Model.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::resources {

void Model::instance_draw(const Shader *shader, const std::vector<glm::mat4> &model_matrices) {
    const int n_instances = model_matrices.size();
    unsigned int buffer;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, n_instances * sizeof(glm::mat4), &model_matrices[0], GL_STATIC_DRAW);

    for (auto &m: this->m_meshes) {
        unsigned int VAO = m.get_m_vao();
        glBindVertexArray(VAO);

        size_t size = sizeof(glm::vec4);

        glEnableVertexAttribArray(3);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * size, (void *) 0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * size, (void *) (size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * size, (void *) (2 * size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * size, (void *) (3 * size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    shader->use();
    for (auto &m: this->m_meshes) {
        m.instance_draw(shader, n_instances);
    }
}

void Model::draw(const Shader *shader) {
    shader->use();
    for (auto &mesh: m_meshes) {
        mesh.draw(shader);
    }
}

void Model::destroy() {
    for (auto &mesh: m_meshes) {
        mesh.destroy();
    }
}
}
