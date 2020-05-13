#ifndef NOWIDE_STATIC_ASSERT_HPP
#define NOWIDE_STATIC_ASSERT_HPP

#define NOWIDE_STATIC_ASSERT(c) static_assert(c, #c)
#define NOWIDE_STATIC_ASSERT_MSG(c, msg) static_assert(c, msg)

#endif
