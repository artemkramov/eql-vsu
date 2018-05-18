#ifndef KG_H
#define KG_H

#include <tchar.h>

#ifdef _DEBUG
#define RINLINE
#else
#define RINLINE __forceinline
#endif

//функции шифровки/расшифровки должны использовать указатель на этот тип 
//для описания буферов
typedef unsigned char keyc;

//параметры алгоритма (по умолчанию параметры для EQL Desktop)
extern int swap_nibbles_mode;
extern int xor_cycles;
typedef void (*swapinbuf_t)(unsigned int inbuflen,keyc const*inbuf,bool forward);
extern swapinbuf_t swapinbuf;
extern int swapinbuf_cycles;

//swapinbuf for EQLD and default
void swapinbufEQLD(unsigned int inbuflen,keyc const*inbuf,bool forward);

//swapinbuf for vsu
void swapinbufVSU(unsigned int inbuflen,keyc const*inbuf,bool forward);

//тип, описывающий серийный номер
struct sernum_t{
	union{
		unsigned long sernum;
		keyc bytes[sizeof(unsigned long)];
	};
};

//данные для процедуры авторизации
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

//функция, вставляющая серийный номер в ключ. длина данных увеличивается на длину
//ключа плюс контрольный (выравнивающий) байт
void add_sernum(unsigned int snlen,void const*sn,unsigned int inbuflen,keyc const*inbuf,keyc*outbuf);

//проверяет верность входного ключа и выковыривает из него серийный номер
//выходной буфер должен быть не менее входного
/*RINLINE */bool check_sernum(unsigned int snlen,void*sn,unsigned int inbuflen,keyc const*inbuf,keyc*outbuf);

//кодирует буфер в 64 ичную систему. Максимум 255 байт
void encode64(unsigned int inbuflen,keyc const*inbuf,char*outbuf);

//раскодирует буфер и возвращает длину
/*RINLINE */unsigned int decode64(char const*inbuf,keyc*outbuf);

//выдает имя файла с ключами
/*RINLINE */void keysfile(_TCHAR*fn,unsigned maxlen);

#ifdef _DEBUG
inline unsigned long calc_ctrl(keyc kc);
#endif

#endif