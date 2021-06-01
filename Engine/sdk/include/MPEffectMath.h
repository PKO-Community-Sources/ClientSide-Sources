#ifndef MPEffectMath_H
#define MPEffectMath_H

#include "MindPowerAPI.h"

#include <string>
#include <math.h>

/// ǰ������
class MPDegree;

/** �����ĽǶ��Ի���Ϊ��λ�İ�װ��.
@remarks
����ֵ�ͽǶ�ֵ�����໥ת��������ת�����Զ�����
*/
class MPRadian
{
	float m_fRad;

public:
	explicit MPRadian ( float r=0 ) : m_fRad(r) {}
	MPRadian ( const MPDegree& d );
	const MPRadian& operator = ( const float& f ) { m_fRad = f; return *this; }
	const MPRadian& operator = ( const MPRadian& r ) { m_fRad = r.m_fRad; return *this; }
	const MPRadian& operator = ( const MPDegree& d );

	float valueDegrees() const; 
	float valueRadians() const { return m_fRad; }
	float valueAngleUnits() const;

	MPRadian operator + ( const MPRadian& r ) const { return MPRadian ( m_fRad + r.m_fRad ); }
	MPRadian operator + ( const MPDegree& d ) const ;
	MPRadian& operator += ( const MPRadian& r ) { m_fRad += r.m_fRad; return *this; }
	MPRadian& operator += ( const MPDegree& d ) ;
	MPRadian operator - () { return MPRadian(-m_fRad); }
	MPRadian operator - ( const MPRadian& r ) const { return MPRadian ( m_fRad - r.m_fRad ); }
	MPRadian operator - ( const MPDegree& d ) const ;
	MPRadian& operator -= ( const MPRadian& r ) { m_fRad -= r.m_fRad; return *this; }
	MPRadian& operator -= ( const MPDegree& d ) ;
	MPRadian operator * ( float f ) const { return MPRadian ( m_fRad * f ); }
	MPRadian operator * ( const MPRadian& f ) const { return MPRadian ( m_fRad * f.m_fRad ); }
	MPRadian& operator *= ( float f ) { m_fRad *= f; return *this; }
	MPRadian operator / ( float f ) const { return MPRadian ( m_fRad / f ); }
	MPRadian& operator /= ( float f ) { m_fRad /= f; return *this; }

	bool operator <  ( const MPRadian& r ) const { return m_fRad <  r.m_fRad; }
	bool operator <= ( const MPRadian& r ) const { return m_fRad <= r.m_fRad; }
	bool operator == ( const MPRadian& r ) const { return m_fRad == r.m_fRad; }
	bool operator != ( const MPRadian& r ) const { return m_fRad != r.m_fRad; }
	bool operator >= ( const MPRadian& r ) const { return m_fRad >= r.m_fRad; }
	bool operator >  ( const MPRadian& r ) const { return m_fRad >  r.m_fRad; }
};

/** �����ĽǶ��ԽǶ�Ϊ��λ�İ�װ��.
@remarks
����ֵ�ͽǶ�ֵ�����໥ת��������ת�����Զ�����
*/
class MPDegree
{
	float m_fDeg; // if you get an error here - make sure to define/typedef 'Real' first

public:
	explicit MPDegree ( float d=0 ) : m_fDeg(d) {}
	MPDegree ( const MPRadian& r ) : m_fDeg(r.valueDegrees()) {}
	const MPDegree& operator = ( const float& f ) { m_fDeg = f; return *this; }
	const MPDegree& operator = ( const MPDegree& d ) { m_fDeg = d.m_fDeg; return *this; }
	const MPDegree& operator = ( const MPRadian& r ) { m_fDeg = r.valueDegrees(); return *this; }

	float valueDegrees() const { return m_fDeg; }
	float valueRadians() const; 
	float valueAngleUnits() const;

	MPDegree operator + ( const MPDegree& d ) const { return MPDegree ( m_fDeg + d.m_fDeg ); }
	MPDegree operator + ( const MPRadian& r ) const { return MPDegree ( m_fDeg + r.valueDegrees() ); }
	MPDegree& operator += ( const MPDegree& d ) { m_fDeg += d.m_fDeg; return *this; }
	MPDegree& operator += ( const MPRadian& r ) { m_fDeg += r.valueDegrees(); return *this; }
	MPDegree operator - () { return MPDegree(-m_fDeg); }
	MPDegree operator - ( const MPDegree& d ) const { return MPDegree ( m_fDeg - d.m_fDeg ); }
	MPDegree operator - ( const MPRadian& r ) const { return MPDegree ( m_fDeg - r.valueDegrees() ); }
	MPDegree& operator -= ( const MPDegree& d ) { m_fDeg -= d.m_fDeg; return *this; }
	MPDegree& operator -= ( const MPRadian& r ) { m_fDeg -= r.valueDegrees(); return *this; }
	MPDegree operator * ( float f ) const { return MPDegree ( m_fDeg * f ); }
	MPDegree operator * ( const MPDegree& f ) const { return MPDegree ( m_fDeg * f.m_fDeg ); }
	MPDegree& operator *= ( float f ) { m_fDeg *= f; return *this; }
	MPDegree operator / ( float f ) const { return MPDegree ( m_fDeg / f ); }
	MPDegree& operator /= ( float f ) { m_fDeg /= f; return *this; }

	bool operator <  ( const MPDegree& d ) const { return m_fDeg <  d.m_fDeg; }
	bool operator <= ( const MPDegree& d ) const { return m_fDeg <= d.m_fDeg; }
	bool operator == ( const MPDegree& d ) const { return m_fDeg == d.m_fDeg; }
	bool operator != ( const MPDegree& d ) const { return m_fDeg != d.m_fDeg; }
	bool operator >= ( const MPDegree& d ) const { return m_fDeg >= d.m_fDeg; }
	bool operator >  ( const MPDegree& d ) const { return m_fDeg >  d.m_fDeg; }
};

// ��Щ����������MPRadian���ڶ��壬��Ϊ��Щ������ҪMPDegree��Ķ���
inline MPRadian::MPRadian(const MPDegree& d) : m_fRad(d.valueRadians()) {
}
inline const MPRadian& MPRadian::operator =(const MPDegree& d) { 
	m_fRad = d.valueRadians(); return *this; 
}
inline MPRadian MPRadian::operator +(const MPDegree& d) const {
	return MPRadian ( m_fRad + d.valueRadians() ); 
}
inline MPRadian& MPRadian::operator +=(const MPDegree& d) {
	m_fRad += d.valueRadians(); return *this; 
}
inline MPRadian MPRadian::operator -(const MPDegree& d) const {
	return MPRadian ( m_fRad - d.valueRadians() ); 
}
inline MPRadian& MPRadian::operator -=(const MPDegree& d) { 
	m_fRad -= d.valueRadians(); return *this; 
}


/** ������ṩ������ѧ�����ķ���.
@remarks
�������ѧ��������C����ʱ�⺯���ı����������ñ���ʹ�������ṩ��
�Ż��ĺ�����Ϊ���ܣ�����ݸ��������ʱ��������Զ�����ѧ��
@note
<br>�����Ǹ���MgcMath.hժ��
<a href="http://www.magic-software.com">Wild Magic</a>.
*/
class MINDPOWER_API MPEffectMath
{
public:
	/** �Ƕȵ�λ. �������ʹ�Ƕȵ�λ�ܹ�ͬʱʹ������ĽǶȺͻ�����.
	�������ļ�ʱ��������ܽ����Զ�ʹ�á���Щ�÷��ɲ鿴Angle��
	*/
	enum AngleUnit
	{
		AU_DEGREE,
		AU_RADIAN
	};

protected:
	// API�ڲ�ʹ�õĽǶȵ�λ
	static AngleUnit m_sAngleUnit;

	/// ���Ǻ������ɹ��캯������.
	static int m_TrigTableSize;

	/// ����ֵ -> ���Ǻ���������С��ʾ���� ( m_TrigTableSize / 2 * PI )
	static float m_TrigTableFactor;
	static float* m_SinTable;
	static float* m_TanTable;

	/** �������Ǻ����� ��˽�У�*/
	void buildTrigTables();

	static float SinTable (float fValue);
	static float TanTable (float fValue);

public:        
	/** Ĭ�Ϲ��캯��.
	@param
		trigTableSize �����ڲ�ʹ�����Ǻ�����Ĵ�С����ѡ������
	*/
	MPEffectMath(unsigned int trigTableSize = 4096);

	/** Ĭ����������.
	*/
	~MPEffectMath();

	static const float POS_INFINITY;
	static const float NEG_INFINITY;
	static const float PI;
	static const float TWO_PI;
	static const float HALF_PI;
	static const float fDeg2Rad;
	static const float fRad2Deg;

	static const std::string BLANK;

/// �ǶȻ������
	static inline float DegreesToRadians(float degrees) { return degrees * fDeg2Rad; }
	static inline float RadiansToDegrees(float radians) { return radians * fRad2Deg; }
	/** �������������������APIʹ�õĽǶȵ�λ�����Ȼ�Ƕȣ�	*/
	static void setAngleUnit(AngleUnit unit);
	/** �õ�����API����ʹ�õĽǶȵ�λ. */
	static AngleUnit getAngleUnit(void);

	/** ����ǰ�ĽǶȵ�λת���˻���. */
	static float AngleUnitsToRadians(float units);
	/** ������ת���ɵ�ǰ�ĽǶȵ�λ. */
	static float RadiansToAngleUnits(float radians);
	/** ����ǰ�ĽǶȵ�λת���˽Ƕ�. */
	static float AngleUnitsToDegrees(float units);
	/** ���Ƕ�ת���ɵ�ǰ�ĽǶȵ�λ. */
	static float DegreesToAngleUnits(float degrees);

/// ��������
	static float UnitRandom ();  // [0,1]
	static float RangeRandom (float fLow, float fHigh);  // in [fLow,fHigh]

/// ���Ǻ������
	/** Sine ����.
	@param
		fValue �ǵĻ���ֵ
	@param
		useTables ���Ϊ��, ʹ�ò�������ͨ�����㣬��������쵫���Ƚ���.
	*/
	static inline float Sin (const MPRadian& fValue, bool useTables = false) {
		return (!useTables) ? float(sin(fValue.valueRadians())) : SinTable(fValue.valueRadians());
	}
	/** Sine ����.
	@param
		fValue �ǵĻ���ֵ
	@param
		useTables ���Ϊ��, ʹ�ò�������ͨ�����㣬��������쵫���Ƚ���.
	*/
	static inline float Sin (float fValue, bool useTables = false) {
		return (!useTables) ? float(sin(fValue)) : SinTable(fValue);
	}
	/** Cosine ����.
	@param
		fValue �ǵĻ���ֵ
	@param
		useTables ���Ϊ��, ʹ�ò�������ͨ�����㣬��������쵫���Ƚ���.
	*/
	static inline float Cos (const MPRadian& fValue, bool useTables = false) {
		return (!useTables) ? float(cos(fValue.valueRadians())) : SinTable(fValue.valueRadians() + HALF_PI);
	}
	/** Cosine function.
	@param
		fValue �ǵĻ���ֵ
	@param
		useTables ���Ϊ��, ʹ�ò�������ͨ�����㣬��������쵫���Ƚ���.
	*/
	static inline float Cos (float fValue, bool useTables = false) {
		return (!useTables) ? float(cos(fValue)) : SinTable(fValue + HALF_PI);
	}
	/** Tangent ����.
	@param
		fValue �ǵĻ���ֵ
	@param
		useTables ���Ϊ��, ʹ�ò�������ͨ�����㣬��������쵫���Ƚ���.
	*/
	static inline float Tan (const MPRadian& fValue, bool useTables = false) {
		return (!useTables) ? float(tan(fValue.valueRadians())) : TanTable(fValue.valueRadians());
	}
	/** Tangent function.
	@param
		fValue �ǵĻ���ֵ
	@param
		useTables ���Ϊ��, ʹ�ò�������ͨ�����㣬��������쵫���Ƚ���.
	*/
	static inline float Tan (float fValue, bool useTables = false) {
		return (!useTables) ? float(tan(fValue)) : TanTable(fValue);
	}



};
#endif
