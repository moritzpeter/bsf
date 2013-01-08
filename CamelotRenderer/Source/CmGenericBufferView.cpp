#include "CmGenericBufferView.h"
#include "CmUtil.h"

namespace CamelotEngine
{
	size_t GenericBufferView::HashFunction::operator()(const Key &key) const
	{
		size_t seed = 0;
		hash_combine(seed, key.mElementWidth);
		hash_combine(seed, key.mFirstElement);
		hash_combine(seed, key.mNumElements);
		hash_combine(seed, key.mRandomGpuWrite);

		return seed;
	}

	bool GenericBufferView::EqualFunction::operator()
		(const Key &a, const Key &b) const
	{
		return a.mElementWidth == b.mElementWidth && a.mFirstElement == b.mFirstElement 
			&& a.mNumElements == b.mNumElements && a.mRandomGpuWrite == b.mRandomGpuWrite;
	}

	GenericBufferView::GenericBufferView(UINT32 firstElement, UINT32 elementWidth, UINT32 numElements, bool randomGpuWrite)
		:mFirstElement(firstElement), mElementWidth(elementWidth), mNumElements(numElements), mRandomGpuWrite(randomGpuWrite)
	{

	}

	GenericBufferView::~GenericBufferView()
	{

	}
}