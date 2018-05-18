#ifndef KG_H
#define KG_H

#include <tchar.h>

#ifdef _DEBUG
#define RINLINE
#else
#define RINLINE __forceinline
#endif

//������� ��������/����������� ������ ������������ ��������� �� ���� ��� 
//��� �������� �������
typedef unsigned char keyc;

struct algpar;

typedef void (*swapinbuf_t)(algpar const&ap,unsigned int inbuflen,keyc const*inbuf,bool forward);

//swapinbuf for EQLD and default
void swapinbufEQLD(algpar const&ap,unsigned int inbuflen,keyc const*inbuf,bool forward);

//swapinbuf for vsu
void swapinbufVSU(algpar const&ap,unsigned int inbuflen,keyc const*inbuf,bool forward);

//���, ����������� �������� �����
struct sernum_t{
	union{
		unsigned long sernum;
		keyc bytes[sizeof(unsigned long)];
	};
};

//������ ��� ��������� �����������
struct author_t{
	union{
		struct{
			unsigned __int32 psw1;
			unsigned __int32 psw2;
			unsigned __int32 seed;
			keyc answer[16];
		};
		keyc bytes[28];
	};
	author_t(){memset(this,0,sizeof(*this));}
};

//��������� ��������� (�� ��������� ��������� ��� EQL Desktop)
struct algpar{
	int swap_nibbles_mode;
	int xor_cycles;
	swapinbuf_t swapinbuf;
	int swapinbuf_cycles;
	algpar():swap_nibbles_mode(0),xor_cycles(4),swapinbuf(swapinbufEQLD),swapinbuf_cycles(0){}
};

//�������, ����������� �������� ����� � ����. ����� ������ ������������� �� �����
//����� ���� ����������� (�������������) ����
void add_sernum(algpar const&ap,unsigned int snlen,void const*sn,unsigned int inbuflen,keyc const*inbuf,keyc*outbuf);

//��������� �������� �������� ����� � ������������ �� ���� �������� �����
//�������� ����� ������ ���� �� ����� ��������
/*RINLINE */bool check_sernum(algpar const&ap,unsigned int snlen,void*sn,unsigned int inbuflen,keyc const*inbuf,keyc*outbuf);

//�������� ����� � 64 ����� �������. �������� 255 ����
void encode64(algpar const&ap,unsigned int inbuflen,keyc const*inbuf,char*outbuf);

//����������� ����� � ���������� �����
/*RINLINE */unsigned int decode64(algpar const&ap,char const*inbuf,keyc*outbuf);

//������ ��� ����� � �������
/*RINLINE */void keysfile(_TCHAR*fn,unsigned maxlen,_TCHAR const*kn=NULL,wchar_t const*dkf=NULL);

#ifdef _DEBUG
inline unsigned long calc_ctrl(keyc kc);
#endif

#endif