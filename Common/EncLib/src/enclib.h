
////////////////////////////////////////
//
//      ����/����
//      by jampe
//      2006/5/26
//
////////////////////////////////////////

#if !defined __ENC_LIB__
#define __ENC_LIB__


typedef unsigned char       __byte;


//  ������Կ
//  keyΪ 8λ��Կ
extern int SetEncKey(const __byte* key);


//  ����
//  buf �������
//  len ������泤��
//  pwd ����������
//  plen ���볤��
//  ���ؼ��ܺ���ַ�������
extern int Encrypt(__byte* buf, int len, const __byte* pwd, int plen);


//  ����
//  buf �������
//  len ������泤��
//  enc ��������Կ
//  elen ��Կ����
//  ���ؼ��ܺ���ַ�������
extern int Decrypt(__byte* buf, int len, const __byte* enc, int elen);



#if NDEBUG
#pragma comment(lib, "EncLib.lib")
#else
#pragma comment(lib, "EncLib_d.lib")
#endif


#endif  /*__ENC_LIB__*/
