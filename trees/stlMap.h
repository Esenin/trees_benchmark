#pragma once

#include <map>

#include "iTree.h"

namespace Tree
{
class StlMap : public ITree
{
public:
	StlMap(){}
	~StlMap(){}
	void insert(Type const &key);
	bool lookup(Type const &key) const;
	bool isEmpty() const;

protected:
	std::map<Type, bool> mMap;
};
}

