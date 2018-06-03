#include <iostream>
#include <vector>

struct Vector3 {
    float x;
    float y;
    float z;
};


struct SelectablePoint {
    Vector3 position;
    Vector3 normal;
    uint64_t selected;
};

void RemoveSelected(
        const SelectablePoint* vertices,
        size_t countVertices,
        const size_t* indices,
        size_t countIndices
) {
    std::vector<size_t> removed;
    removed.reserve(countVertices);

    std::vector<SelectablePoint> newVertices;
    for (auto i = 0; i < countVertices; ++i)
    {
        if (vertices[i].selected == 0)
        {
            newVertices.push_back(vertices[i]);
        }
        else
        {
            removed.push_back(i);
        }
    }

    std::vector<size_t> newIndices{countIndices};
    for (auto i = 0; i < countIndices; i += 3)
    {
        auto a = indices[i + 0];
        auto b = indices[i + 1];
        auto c = indices[i + 2];

        if (vertices[a].selected == 0
         && vertices[b].selected == 0
         && vertices[c].selected == 0)
        {
            bool _a = false;
            bool _b = false;
            bool _c = false;
            int aCounter = 0;
            int bCounter = 0;
            int cCounter = 0;

            for (int j = 0; j < removed.size(); j++)
            {
                if (!_a)
                    if (removed[j] < a)
                        aCounter = j + 1;
                    else
                        _a = true;
                if (!_b)
                    if (removed[j] < b)
                        bCounter = j + 1;
                    else
                        _b = true;
                if (!_c)
                    if (removed[j] < c)
                        cCounter = j + 1;
                    else
                        _c = true;

                if (_a && _b && _c)
                    break;
            }

            newIndices.push_back((size_t)((int)a - aCounter));
            newIndices.push_back((size_t)((int)b - bCounter));
            newIndices.push_back((size_t)((int)c - cCounter));
        }
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}