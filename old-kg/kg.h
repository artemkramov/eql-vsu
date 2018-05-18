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

//��������� ��������� (�� ��������� ��������� ��� EQL Desktop)
extern int swap_nibbles_mode;
extern int xor_cycles;
typedef void (*swapinbuf_t)(unsigned int inbuflen,keyc const*inbuf,bool forward);
extern swapinbuf_t swapinbuf;
extern int swapinbuf_cycles;

//swapinbuf for EQLD and default
void swapinbufEQLD(unsigned int inbuflen,keyc const*inbuf,bool forward);

//swapinbuf for vsu
void swapinbufVSU(unsigned int inbuflen,keyc const*inbuf,bool forward);

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

//�������, ����������� �������� ����� � ����. ����� ������ ������������� �� �����
//����� ���� ����������� (�������������) ����
void add_sernum(unsigned int snlen,void const*sn,unsigned int inbuflen,keyc const*inbuf,keyc*outbuf);

//��������� �������� �������� ����� � ������������ �� ���� �������� �����
//�������� ����� ������ ���� �� ����� ��������
/*RINLINE */bool check_sernum(unsigned int snlen,void*sn,unsigned int inbuflen,keyc const*inbuf,keyc*outbuf);

//�������� ����� � 64 ����� �������. �������� 255 ����
void encode64(unsigned int inbuflen,keyc const*inbuf,char*outbuf);

//����������� ����� � ���������� �����
/*RINLINE */unsigned int decode64(char const*inbuf,keyc*outbuf);

//������ ��� ����� � �������
/*RINLINE */void keysfile(_TCHAR*fn,unsigned maxlen);

#ifdef _DEBUG
inline unsigned long calc_ctrl(keyc kc);
#endif

#endif