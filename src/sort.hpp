#ifndef VAL_SORT_
#define VAL_SORT_

namespace val {
	template <typename Iterator, typename Comparator = std::less<typename Iterator::value_type>>
	void insertionSort(Iterator first, Iterator last, Comparator comp = Comparator()) {
		for (auto it = std::next(first); it != last; ++it) {
			auto key = *it;
			auto j = it;
			
			while (j != first) {
				auto prev = std::prev(j);
				if (!comp(key, *prev)) break;
				*j = *prev;
				j = prev;
			}

			*j = key;
		}
	}
}

#endif