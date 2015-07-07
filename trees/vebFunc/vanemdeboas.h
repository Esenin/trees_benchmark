/*
 * author: https://github.com/vineetgopal
 */

#include <vector>
#include <algorithm>
#include <iostream>
#include <list>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <limits>

template <typename Key>
class VanEmdeBoas {
	int height;
	std::vector<Key> vEB;
	std::vector<Key>* list;
	static const int base_height_size = 3;
	int queryIndex(Key *vEB, int height, Key q);
	int getHeight(int length) { return (int) ceil(log2((double) length)); }
	void populate(Key *vEB, int height, int levels_below, int offset, std::vector<Key>* data);
public:
	VanEmdeBoas<Key>(std::vector<Key>* l);
	int query(Key q);
	int queryIndex(Key q) { return queryIndex(vEB.data(), height, q); }
};

template <typename Key>
VanEmdeBoas<Key>::VanEmdeBoas(std::vector<Key>* l) {
	height = getHeight(l->size());
	vEB.resize((1 << height), 0);
	populate(vEB.data(), height, 0, 0, l);
	list = l;
}


// either write the contents of the BST at the position pointed to by vEB or recursively subdivide the space into an upper triangle and set of lower triangles
template <typename Key>
void VanEmdeBoas<Key>::populate(Key *vEB, int _height, int levels_below, int offset, std::vector<Key>* data) {
	if (_height <= base_height_size) {
		int index_offset = offset * (1 << (levels_below + _height));
		int root = (1 << (_height + levels_below - 1)) - 1;
		for (int i = 0; i < _height; i++) {
			for (int j = 0; j < (1 << i); j++) {
				int index = index_offset + root + j*((root + 1) << 1);
				*vEB =  (index < data->size()) ? (*data)[index] : std::numeric_limits<Key>::max();
				vEB++;
			}
			root = root >> 1;
		}
	} else {
		int height_top = (_height >> 1);
		int height_bottom = ((_height + 1) >> 1);
		populate(vEB, height_top, levels_below + height_bottom, offset, data);
		vEB += (1 << height_top) - 1;
		for (int i = 0; i < (1 << height_top); i++) {
			populate(vEB, height_bottom, levels_below, offset*(1 << height_top) + i, data);
			vEB += (1 << height_bottom) - 1;
		}
	}
}

template <typename Key>
int VanEmdeBoas<Key>::queryIndex(Key *vEB, int _height, Key q) {
	int index = 0;
	if (_height <= base_height_size) {
		// traverse upper triangle using heap ordering
		for (int i = 0; i < _height; i++) {
			if (*(vEB + index) > q) {
				index = ((index + 1) << 1) - 1; // go left
			} else {
				index = ((index + 1) << 1);
			}
		}
		index -= ((1 << _height) - 1);
	} else {
		// find out which leaf structure we need to follow
		int height_top = (_height >> 1);
		int height_bottom = ((_height + 1) >> 1);
		index = queryIndex(vEB, height_top, q);
		vEB += index * ((1 << height_bottom) - 1) + ((1 << height_top) - 1);
		index *= (1 << height_bottom);
		index += queryIndex(vEB, height_bottom, q);
	}
	return index;
}

template <typename Key>
int VanEmdeBoas<Key>::query(Key q) {
	return (list->front() <= q) ? (*list)[queryIndex(vEB.data(), height, q) - 1] : std::numeric_limits<Key>::min();
}


