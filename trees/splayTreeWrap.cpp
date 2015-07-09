#include "splayTreeWrap.h"

using namespace Tree;

void SplayTreeWrap::insert(Type const &key)
{
	mTree.insert(key, true);
}

bool SplayTreeWrap::lookup(Type const &key) const
{
	return mTree.find(key) != mTree.end();
}

bool SplayTreeWrap::isEmpty() const
{
	return mTree.empty();
}
