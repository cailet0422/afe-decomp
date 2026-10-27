#ifndef ITERATOR_H
#define ITERATOR_H

#include "types.h"

#ifdef __cplusplus
namespace std {

#ifdef TARGET_PC
/* On PC, don't redefine input_iterator_tag (system <iterator> may provide it),
 * but keep our simple distance() since the decomp iterators don't have
 * iterator_traits::difference_type that the system std::distance requires. */
#else
struct input_iterator_tag {};
#endif

template <class InputIterator>
inline
s32
#ifdef TARGET_PC
__distance(InputIterator first, InputIterator last) {
#else
__distance(InputIterator first, InputIterator last, input_iterator_tag) {
#endif
	s32 result = 0;
	for (; first != last; ++first)
		++result;
	return result;
}

template <class InputIterator>
inline
s32
distance(InputIterator first, InputIterator last) {
#ifdef TARGET_PC
	return __distance(first, last);
#else
    input_iterator_tag tag;
	return __distance(first, last, tag);
#endif
}

} // namespace std
#endif

#endif
