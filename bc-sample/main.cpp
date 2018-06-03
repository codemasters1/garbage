#include <iostream>

enum {
    i32 = 1,
    f32 = 2
};

enum {
    METHOD_RETURNS = 1
};

struct Exported {
    uint32_t nameLength;
    uint8_t name[0];
};

struct DvcHeader {
    uint8_t tag[8] = {'D', 'V', 'C', ' ', 'F', 'i', 'l', 'e'};
    uint16_t version = 0x0000;
    uint32_t numExported;
    Exported exported[0];
};

// .DVC - DV compiled file

struct Function {
    uint32_t flags;
    uint8_t numArgs;
    uint8_t argTypes;
    uint8_t retType;
};

int main() {
    DvcHeader header;
    header.numExported = 2;

    FILE* f = std::fopen("test.dvc", "wb");
    std::fwrite(&header, sizeof(DvcHeader), 1, f);

    for (int i = 0; i < 10; ++i) {
        Exported exported{};
        exported.nameLength = 4;
        std::fwrite(&exported, sizeof(Exported), 1, f);
        std::fwrite("test", 1, 4, f);
    }

    std::fclose(f);

    return 0;
}