#ifndef HESTIA_GRAPHICS_MATERIAL_MATERIAL_HPP_
#define HESTIA_GRAPHICS_MATERIAL_MATERIAL_HPP_

#include "glad/glad.h"
#include "glm/glm/glm.hpp"

namespace HGE {

    using TextureId = unsigned int;

    class Material {

        TextureId mId;

        public:
        Material(TextureId id) : mId(id) { }
        ~Material() { }

        void useTexture() {
            glBindTexture(GL_TEXTURE_2D, mId);
        }
    };
}

#endif 