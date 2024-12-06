#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include <xsim_config>
#include <cassert>

namespace xsim {
    template<size_t ChunkSize>
    class PoolAllocator {
        struct Page {
            char memory[4096];
        };

        union Node {
            Node* previous;
            char chunk[ChunkSize];
        };
    public:
        PoolAllocator()
        {
        }

        ~PoolAllocator()
        {
            for (auto page : pages)
                ::free(page);
            pages.clear();
        }

        void* allocate(size_t size)
        {
            assert(size <= ChunkSize);
            if (!stack && !grow())
                return nullptr;
            Node* top = stack;
            stack = stack->previous;
            return top->chunk;
        }

        void free(void* ptr)
        {
            Node* top = static_cast<Node*>(ptr);
            top->previous = stack;
            stack = top;
        }

        size_t chunk_size() const
        {
            return ChunkSize;
        }

    private:
        bool grow()
        {
            Page* new_page = static_cast<Page*>(::malloc(sizeof(Page)));
            if (!new_page)
                return false;
            init_page(new_page);
            pages.push_back(new_page);
            return true;
        }

        void init_page(Page* page)
        {
            Node* new_stack = reinterpret_cast<Node*>(page->memory);
            new_stack->previous = stack;

            const size_t num_chunks = sizeof(Page) / sizeof(Node);
            for (size_t i = 0; i < num_chunks - 1; ++i, ++new_stack)
                new_stack[1].previous = new_stack;
            stack = new_stack;
        }

        Node* stack = nullptr;
        std::vector<Page*> pages;
    };

} // namespace xsim

#endif // EVENT_H
