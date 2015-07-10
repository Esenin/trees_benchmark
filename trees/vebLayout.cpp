#include "vebLayout.h"

using namespace tree;

VebLayoutTree::VebLayoutTree(size_t hintSize)
{
    mData.reserve(hintSize + 1);
}


VebLayoutTree::~VebLayoutTree()
{
	delete mVebber;
}

void VebLayoutTree::insert(Type const &key)
{
	mData.push_back(key);
	++mSize;
}

bool VebLayoutTree::lookup(Type const &key) const
{
	if (key == 0)
	{
		Type q_key = mVebber->queryIndex(key);
		return  q_key > 0 && q_key <= mSize && mData[q_key - 1] == key;
	}

	return mVebber->query(key) == key;
}

bool VebLayoutTree::isEmpty() const
{
	return mData.size() > 0;
}

bool VebLayoutTree::isBuildable() const
{
	return true;
}

void VebLayoutTree::buildTree()
{
	delete mVebber;
    mVebber = nullptr;
	std::sort(mData.begin(), mData.end());
	mVebber = new VanEmdeBoas<Type>(&mData);
}

