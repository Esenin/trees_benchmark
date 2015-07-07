#pragma once

#include <vector>

#include "iTree.h"
#include "vebFunc/vanemdeboas.h"

namespace Tree
{
class VebLayoutTree : public ITree
{
public:
	VebLayoutTree();
	~VebLayoutTree();

	void insert(Type const &key);
	bool lookup(Type const &key) const;

	bool isEmpty() const;

	bool isBuildable() const;
	void buildTree();

protected:
	std::vector<Type> mData;
	VanEmdeBoas<Type> *mVebber;
	int mSize;
};
}
