#include <algorithm>
#include <boost/algorithm/string.hpp>

#include <process.h>
namespace criterion {
    void Process::imageFiles(const std::vector<std::filesystem::path>& inputDirs,
        std::list<FileStream>&& imgStreams) {
        const std::filesystem::path out{output};

        if (!inputDirs.empty()) {
            if (out.extension() != ".img")
                throw std::runtime_error("");

            image = std::make_unique<img::ImgStream>(inputDirs);
        } else {
            if (with.empty() && by.empty() && !out.extension().empty())
                throw std::runtime_error("");

            image = std::make_unique<img::ImgStream>(std::move(imgStreams));
        }

        std::vector<std::filesystem::path> files;
        split(files, by, boost::is_any_of(",;+"));

        if (operation == "extract") {
            image->extract(output);
        }
        if (operation == "remove") {
            image->excludeFiles(with);
            image->compress(output);
        }

        if (operation == "overwrite") {
            image->excludeFiles(with);
            image->addFiles(files);
            image->compress(output);
        }

        if (operation == "compress") {
            image->compress(output);
        }
    }

    void Process::txdFiles(std::list<FileStream>&& txdStreams) {
        if (txdStreams.size() > 1) {
            throw std::runtime_error("Too many txd files");
        }
        txd = std::make_unique<txd::TxdStream>(txdStreams.back());

        if (operation == "extract") {
            txd->extract(true, output);
        }
    }

    void Process::dffFiles(std::list<FileStream>&& dffStreams) {
        if (dffStreams.size() > 1) {
            throw std::runtime_error("Too many txd files");
        }
        dff = std::make_unique<dff::DffStream>(dffStreams.back());
    }
}
