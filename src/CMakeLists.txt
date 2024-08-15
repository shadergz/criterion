cmake_minimum_required(VERSION 3.21)

target_sources(criterion PRIVATE
    types.h
    main.cpp
    process.cpp
    process.h
    img/helper.cpp
    img/directory_entry.cpp
    img/directory_entry.h
    img/data_entry.cpp
    img/data_entry.h
    img/img.cpp
    img/img.h

    txd/txd.cpp
    txd/txd.h
    txd/texture.cpp
    txd/texture.h
    txd/textures_container.cpp
    txd/textures_container.h

    dff/dff.cpp
    dff/dff.h
    dff/clump.cpp
    dff/clump.h
    dff/geometry.cpp
    dff/geometry.h
    dff/frames.cpp
    dff/frames.cpp
    dff/extension.cpp
    dff/extension.h

    dds/dds_backing.cpp
    dds/dds_backing.h
)