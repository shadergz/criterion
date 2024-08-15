#pragma once
#include <list>

#include <types.h>

#include <dff/geometry.h>
#include <dff/frames.h>
#include <dff/extension.h>
namespace criterion::dff {
    class FrameList {
    public:
        FrameList() = default;
        explicit FrameList(FileStream& stream);
    private:
        struct {
            ChunkHeader chunk;
            ChunkHeader structure;
            u32 frames;
        } header{};

        std::list<Frames> frames;
        // Names of available frames as strings
        Extension extension;
    };
    class GeometryList {
    public:
        GeometryList() = default;
        explicit GeometryList(FileStream& stream);
    private:
        struct {
            ChunkHeader chunk;
            ChunkHeader structure;

            u32 geometryCount;
        } header{};

        std::list<Geometry> geometries;
    };

    class Clump {
    public:
        Clump() = default;
        explicit Clump(FileStream& stream);

    private:
        struct {
            ChunkHeader chunk;
            u32 atomicCount;
            u32 lightCount;
            u32 cameraCount;
        } packet{};

        FrameList frames;
        GeometryList geoPack;
    };
}
