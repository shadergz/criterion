#include <vector>
#include <cstring>

#include <dff/frames.h>
namespace criterion::dff {
    Frames::Frames(FileStream& stream) {
        std::vector<char> buffer(sizeof(model));
        stream.read(&buffer[0], sizeof(model));

        std::memcpy(&model, &buffer[0], sizeof(model));
    }
}
