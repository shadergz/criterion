# criterion

Extract, edit, and compress IMG, TXD, and DFF files, with support for converting DDS to PNG

### How to build
- mkdir build; cd build; cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
- ninja

### Usage examples

- How to extract a database in .img format
    - criterion --mode img --operation extract --input gta3.img

- How to extract an image database in DDS and PNG formats
    - criterion --mode txd --operation extract --input 8bars.txd --output 8bars

