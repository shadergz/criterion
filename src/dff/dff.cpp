#include <vector>
#include <cstring>

#include <dff/dff.h>
namespace criterion::dff {
    DffStream::DffStream(FileStream& stream) {
        std::fstream io{stream.path, std::ios::binary | std::ios::in};
        stream.swap(io);

        std::vector<char> buffer(sizeof(header));
        do {
            stream.seekg(header.size, std::ios::cur);
            stream.read(&buffer[0], sizeof(header));

            std::memcpy(&header, &buffer[0], sizeof(header));
        } while (header.type != 0x10);

        clump = Clump(stream);
    }
}
