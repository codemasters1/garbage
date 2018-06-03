#include <cstdint>
#include <cstdio>
#include <new>
#include <utility>
#include <vector>

struct guwno {
    int x;
    guwno(int x): x{x}{};
};

class Allocator {
private:
    void* base_;
    void* ptr_;
    size_t size_;
public:
    Allocator(void* pool, size_t size) : base_{pool}, ptr_{pool}, size_{size} {}

    template <typename T, typename... TParams>
    T* alloc(TParams&&... params) {
    	if ((uint8_t*)ptr_ > (uint8_t*)base_ + size_) {
    		return nullptr;
    	}

    	auto obj = new (ptr_) T(std::forward<TParams>(params)...);
    	ptr_ = reinterpret_cast<uint8_t*>(ptr_) + sizeof(T);
        return obj;
    }
};

int main(void) {
    void* p = new uint8_t[0x1000];
	Allocator a{p, 0x1000};

	std::vector<guwno*> guwna;

	for (int i = 0; i < 10000; ++i) {
		auto o = a.alloc<guwno>(i);
		if (o == nullptr) {
			break;
		} else {
			guwna.push_back(o);
		}
	}

	for (const auto& it : guwna) {
		printf("%p: %d\n", it, it->x);
	}



    return 0;
}