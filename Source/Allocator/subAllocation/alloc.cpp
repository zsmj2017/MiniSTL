#include "alloc.h"

namespace MiniSTL {
char *_default_alloc::start_free = nullptr;
char *_default_alloc::end_free = nullptr;
size_t _default_alloc::heap_size = 0;
_default_alloc::obj *volatile _default_alloc::free_list[NFREELISTS] = {
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

//定义handler
typename _malloc_alloc::malloc_handler _malloc_alloc::_malloc_alloc_oom_handler = nullptr;
} // namespace MiniSTL