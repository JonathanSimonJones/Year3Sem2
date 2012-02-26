#ifndef MULTISAMPLE_H
#define MULTISAMPLE_H

class MultiSample
{
public:
	MultiSample(int counts, int quality);
	int GetCount();
	int GetQuality();
private:
	int m_Counts;
	int m_Quality;
};

#endif