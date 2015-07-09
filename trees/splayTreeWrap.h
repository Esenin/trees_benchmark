#pragma once

#include "iTree.h"
#include "splayTree/splayTree.h"

namespace Tree
{
class SplayTreeWrap : public ITree
{
public:
	SplayTreeWrap() = default;

	void insert(Type const &key);
	bool lookup(Type const &key) const;
	bool isEmpty() const;

protected:
	SplayTree<Type, bool> mTree;
};
}

