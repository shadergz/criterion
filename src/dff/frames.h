#pragma once
#include <glm/glm.hpp>

#include <types.h>
namespace criterion::dff {
    struct Model {
        glm::vec3 right;
        glm::vec3 up;
        glm::vec3 at;
        glm::vec3 position;
        u32 parent;
        u32 flags;
    };
    static_assert(sizeof(Model) == 56);

    class Frames {
    public:
        explicit Frames(FileStream& stream);

    private:
        Model model{};
        static_assert(sizeof(glm::vec3) == sizeof(float) * 3);
    };
}