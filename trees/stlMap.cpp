#include "stlMap.h"

using namespace Tree;

void StlMap::insert(Type const &key)
{
	mMap.insert(std::pair<Type, bool>(key, true));
}

bool StlMap::lookup(Type const &key) const
{
	return mMap.count(key) > 0;
}

bool StlMap::isEmpty() const
{
	return mMap.empty();
}

