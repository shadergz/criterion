#pragma once
#include <vector>
#include <img/directory_entry.h>

#include <types.h>
namespace criterion::img {
    class DataEntry {
    public:
        DataEntry(DirectoryEntry& en, FileStream& img)
            : super(en), io(img) {}
        ~DataEntry();

        [[nodiscard]] std::vector<u8> getData() const;
        void getData(FileStream& img, std::vector<u8>& content) const;
    private:
        DirectoryEntry& super;
        FileStream& io;
    };
}
