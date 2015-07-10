#pragma once

#include <vector>

#include "iTree.h"
#include "vebFunc/vanemdeboas.h"

namespace tree
{
class VebLayoutTree : public ITree
{
public:
	VebLayoutTree() = default;
    //! @arg hintSize forward declaration of input data amount; May speedup building
	VebLayoutTree(size_t hintSize);
	~VebLayoutTree();

	void insert(Type const &key);
	bool lookup(Type const &key) const;

	bool isEmpty() const;

	bool isBuildable() const;
	void buildTree();

protected:
	std::vector<Type> mData;
	VanEmdeBoas<Type>* mVebber = nullptr;
	int mSize = 0;
};
}
