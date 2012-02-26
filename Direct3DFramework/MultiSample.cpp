#include "Multisample.h"

MultiSample::MultiSample(int counts, int quality)
{
	m_Counts = counts;
	m_Quality = quality;
}

int MultiSample::GetCount()
{
	return m_Counts;
}

int MultiSample::GetQuality()
{
	return m_Quality;
}