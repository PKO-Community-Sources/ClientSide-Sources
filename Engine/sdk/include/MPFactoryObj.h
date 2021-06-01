#ifndef MPFactoryObj_H
#define MPFactoryObj_H

/** ���󹤳���. ����û���κι���, ��������������ӹ���.
*/
template< typename T > class FactoryObj
{
public:
	virtual ~FactoryObj() {};

	/** ���ع�������.	*/
	virtual const string& getType() const = 0;

	/** ����һ���¶���.
	@param name �������������
	@return ��������������Ķ��������������������ڹ���.
	*/
	virtual T* createInstance( const string& name ) = 0;    
	/** ����һ����������������Ķ���.
	@param ptr ָ����Ҫ�������Ķ���ָ��
	*/
	virtual void destroyInstance( T* ) = 0;    
};

#endif