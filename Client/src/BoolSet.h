//--------------------------------------------------------------
// ����:����������
// ����:lh 2004-11-01
// ��;:���Կ����ж��Ƿ����в�����Ϊfalse
//--------------------------------------------------------------

#pragma once

class CBoolSet
{
public:
	CBoolSet() : _value(0) {}

	// bit��Χ0~31
	bool IsTrue( unsigned int bit )		{ return (_value & 0x00000001 << bit)!=0;	}
	bool IsFalse( unsigned int bit )    { return (_value & 0x00000001 << bit)==0;	}
	void SetTrue( unsigned int bit )	{ _value |= 0x00000001 << bit;				}	// λΪtrue
	void SetFalse( unsigned int bit )	{ _value &= ~(0x00000001 << bit);			}	// λΪfalse

	void Set( unsigned int bit, bool v ){ if(v) SetTrue(bit); else SetFalse(bit);	}
	bool Get( unsigned int bit )		{ return (_value & 0x00000001 << bit)!=0;	}

	void AllFalse()						{ _value = 0;			}	// ȫfalse
	void AllTrue()						{ _value = 0xffffffff;	}	// ȫtrue

	bool IsNone()						{ return _value==0;		}	// �Ƿ�ȫΪfalse
	bool IsAny()						{ return _value!=0;		}	// �Ƿ���ĳһλΪtrue

private:
	unsigned int _value;

};
