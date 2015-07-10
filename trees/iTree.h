#pragma once

#include <stdexcept>

namespace tree
{
//! singleshot change of type. In research purpose templates are not necessary
typedef long long Type;

//! @class ITree is an interface for tree-like datastructures
//! which have no remove method, because it doesnt necessary
class ITree
{
public:
	ITree(){}
	virtual ~ITree(){}

	virtual void insert(Type const &key) = 0;

	//! @return true case the key was found in tree
	virtual bool lookup(Type const &key) const = 0;
	virtual bool isEmpty() const = 0;

	//! hook method for static-insert trees
	virtual void buildTree() {}

	//! @return True in case call buildTree is necessary
	virtual bool isBuildable() const
	{
		return false;
	}
};
}
