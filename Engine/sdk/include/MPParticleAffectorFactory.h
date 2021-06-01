#ifndef MPParticleAffectorFactory_H
#define MPParticleAffectorFactory_H

#include "MPEffPrerequisites.h"

class MINDPOWER_API MPParticleAffectorFactory
{
protected:
	std::vector<MPParticleAffector*> m_Affectors;
public:
	MPParticleAffectorFactory() {};
	virtual ~MPParticleAffectorFactory();
	/** ���ع���������, ������ֱ�ʶ�������������������Ӱ�����ӵ����� */
	virtual string getName() const = 0;

	/** ����һ���µ�Ӱ�����Ӷ���.*/
	virtual MPParticleAffector* createAffector(MPParticleSystem* psys) = 0;

	/** ����Ӱ�����Ӷ���. */
	virtual void destroyAffector(MPParticleAffector* e);
};
#endif