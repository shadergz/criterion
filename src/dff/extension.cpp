#include <cstring>

#include <dff/extension.h>
namespace criterion::dff {
    Extension::Extension(FileStream& stream) {
        std::vector<char> buffer(sizeof(header));

        do {
            stream.read(&buffer[0], sizeof(header));
            std::memcpy(&header, &buffer[0], sizeof(header));

            if (header.type != 3) {
                stream.seekg(-sizeof(header), std::ios::cur);
                continue;
            }
            while (header.size) {
                ChunkHeader lookup;
                stream.read(&buffer[0], sizeof(lookup));

                std::memcpy(&lookup, &buffer[0], sizeof(lookup));
                if (lookup.type == Frame) {
                    buffer.resize(lookup.size);
                    stream.read(&buffer[0], lookup.size);

                    std::array<char, 0x1a> indexable{};
                    if (buffer.size() > indexable.size()) {
                        throw std::runtime_error("It is not possible to store the variable because the string length exceeds the limit");
                    }
                    std::memcpy(&indexable[0], &buffer[0], buffer.size());
                    couplers.emplace_back(indexable);
                } else {
                    buffer.resize(lookup.size);
                    stream.read(&buffer[0], lookup.size);
                }

                header.size -= (lookup.size + 12);
            }
        } while (header.type == 3);
    }
}
