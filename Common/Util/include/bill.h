
#ifndef BILL_H_
#define BILL_H_

int PSS_GetCredit(char const* section, char const* name, std::string& strErr);

class CPAI {
public:
    CPAI();
    ~CPAI() {}

    int Login(char const* name, char const* pwd);       //  �޸ķ���ֵ��10000�ɹ���10001ʧ�ܣ�10004����ʧ��
    std::string GetPassport();

    std::string LastError();    //  add by jampe

private:
};

#endif