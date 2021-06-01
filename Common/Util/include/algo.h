
#ifndef _ALGO_H_
#define _ALGO_H_

int base64(char const* src, unsigned int src_len, char* dst,
           unsigned int dst_max_len, unsigned int* dst_len);
int ibase64(char const* src, unsigned int src_len, char* dst, unsigned int* dst_len);
unsigned char const* sha1(unsigned char const* msg,unsigned int len, unsigned char* md);

void init_Noise( int nNoise, char szKey[4] );
bool encrypt_Noise(char szKey[4], char* src, unsigned int src_len);
bool decrypt_Noise(char szkey[4], char* src, unsigned int src_len);

bool encrypt_A(char* out, char* in, long data_len, char const* key, int key_len, bool en = true);
bool encrypt_B(char* src, unsigned int src_len, char* key, unsigned int key_len, bool en = true);

bool dbpswd_out(char const* ctxt, int ctxt_len, string& pswd);
bool dbpswd_in(char const* pswd, int pswd_len, string& ctxt);

void md5(char const* msg, unsigned char dig[16]);
void md5string(char const* msg, char str[33]);

#include <string>

#define KCHAP_SESSION_KEY_LEN 6

class KCHAPc
{
public:
    KCHAPc(char const* name, char const* password);
    ~KCHAPc();

    string GetPwdDigest() const
    {
        return m_strPwdDig;
    }
    void SetChapString(char const* chap_string);
    void GetStep1Data(char* buf, int buf_len, int& out_len);
    void GetSessionClrKey(void* pdat, int dat_len, char* buf, int buf_len, int& out_len);

private:
    std::string m_strName;
    std::string m_strPwd;
    std::string m_strChapString;

    char skey[KCHAP_SESSION_KEY_LEN];

public:

	std::string m_strPwdDig;
    
};

class KCHAPs
{
public:
    KCHAPs(const char *passwd_digest="");
    ~KCHAPs();

    bool DoAuth(char const* name, char const* chap_string,
                char const* pdat, int dat_len, char const* passwd_digest);
	void NewSessionKey();
    void GetSessionClrKey(char* buf, int buf_len, int& out_len);
    void GetSessionEncKey(char* buf, int buf_len, int& out_len);
	

private:

    std::string m_strName;
    std::string m_strPwdDig;
    std::string m_strChapString;

    char skey[KCHAP_SESSION_KEY_LEN];
};

class CSHA1
{
public:
    CSHA1();
    ~CSHA1();

    unsigned char const* sha1(unsigned char const* msg,unsigned int len, unsigned char* md);

private:
    unsigned long func_S(char n, unsigned long num);
    unsigned long func_F(unsigned int t, unsigned long B, unsigned long C, unsigned long D);
};

class C3DES
{
public:
    C3DES();
    ~C3DES();

    long Encrypt(char* out, char const* in, long data_len,
        char const* key, int key_len);
    long Decrypt(char* out, char const* in, long data_len,
        char const* key, int key_len);

private:
    static char const ip_table[64];
    static char const ipr_table[64];
    static char const e_table[48];
    static char const p_table[32];
    static char const pc1_table[56];
    static char const pc2_table[48];
    static char const loop_table[16];
    static char const s_box[8][4][16];

    typedef bool (*psubkey)[16][48];
    bool sub_key[2][16][48];
    bool is_3A;
    char tmp[256];
    char des_key[16];

    bool mr[48];

    bool m[64];
    bool m_tmp[32];
    bool* li;
    bool* ri;

    bool k[64];
    bool* kl;
    bool* kr;

    void transform(bool* out, bool* in, char const* table, int len);
    void xor(bool* inA, bool const* inB, int len);
    void rotate_L(bool* in, int len, int loop);
    void byte2bit(bool* out, char const* in, int bits);
    void bit2byte(char* out, const bool* in, int bits);
    void set_sub_key(psubkey pSubkey, char const key[8]);
    void set_key(char const* key, int len);
    void S_func(bool out[32], bool const in[48]);
    void F_func(bool in[32], bool const ki[48]);
    void base_A(char out[8], char in[8], psubkey const pSubKey, bool encrypt);
    bool algo_A(char* out, char* in, long data_len, char const* key,
        int key_len, bool encrypt);
};

class CDES  
{
public:
    CDES();
    virtual ~CDES();

    enum {ENCRYPT = 0, DECRYPT};
    enum {ECB = 0, CBC};
    typedef bool (*PSubKey)[16][48];
    enum {PAD_ISO_1 = 0, PAD_ISO_2, PAD_PKCS_7};
    static bool	RunPad(int nType,const char* In,unsigned datalen,char* Out,unsigned& padlen);
    static bool RunDes(bool bType,bool bMode,char* In,char* Out,unsigned datalen,const char* Key,const unsigned char keylen);

protected:
    static void SetSubKey(PSubKey pSubKey, const char Key[8]);
    static void DES(char Out[8], char In[8], const PSubKey pSubKey, bool Type);
    static void S_func(bool Out[32], const bool In[48]);
    static void F_func(bool In[32], const bool Ki[48]);
};

#endif
