#include "stdafx.h"
#include "kg.h"
#include <myreg.h>
#include <atlbase.h>

RINLINE unsigned long calc_ctrl(keyc kc)
{
	unsigned long sum=0;
	keyc kc2=kc&0x0f;
	keyc kc1=(kc>>4)&0x0f;
	if((kc1+kc2)&1){
		keyc tmp=kc1;
		kc1=kc2;
		kc2=tmp;
	}
	sum+=kc1;
	kc2<<=1;
	if(kc2>9){
		kc2=(kc2/10)+(kc2%10);
	}
	sum+=kc2;
	return sum;
}

static int cnts[26]={
	1, 2, 4, 6, 8, 9, 11, 12, 14, 15,
	16, 16, 16, 16, 16, 16, 15, 14, 12, 10,
	8, 7, 5, 4, 2, 1
};

static keyc ctrls[26][16]={
//0	1
	{0},
//1	2
	{1, 80},
//2	4
	{10, 16, 21, 32},
//3	6
	{161, 17, 240, 3, 6, 82},
//4	8
	{176, 18, 2, 31, 42, 53, 64, 97},
//5	9
	{112, 163, 242, 27, 33, 38, 49, 5, 84},
//6	11
	{12, 178, 20, 23, 34, 48, 63, 74, 85, 96, 99},
//7	12
	{114, 165, 19, 193, 244, 35, 59, 7, 70, 8, 81, 86},
//8	14
	{101, 106, 117, 128, 129, 180, 208, 22, 4, 44, 50, 55, 66, 95},
//9	15
	{102, 113, 116, 144, 167, 195, 246, 29, 37, 40, 51, 65, 88, 9, 91},
//10	16
	{103, 127, 131, 138, 14, 149, 160, 182, 210, 24, 25, 36, 52, 76, 87, 98},
//11	16
	{11, 118, 123, 134, 145, 146, 169, 197, 225, 248, 39, 61, 67, 72, 83, 90},
//12	16
	{105, 108, 119, 130, 133, 159, 170, 181, 184, 192, 212, 26, 46, 54, 57, 68},
//13	16
	{104, 115, 120, 13, 148, 155, 166, 171, 177, 199, 227, 250, 41, 69, 92, 93},
//14	16
	{100, 107, 135, 140, 151, 162, 186, 191, 202, 213, 214, 224, 28, 56, 78, 89},
//15	16
	{122, 125, 136, 147, 15, 150, 173, 187, 198, 201, 209, 229, 252, 43, 71, 94},
//16	15
	{109, 110, 121, 132, 137, 172, 183, 188, 194, 216, 223, 234, 245, 30, 58},
//17	14
	{124, 152, 157, 168, 175, 179, 203, 219, 230, 231, 241, 254, 45, 73},
//18	12
	{111, 139, 142, 153, 164, 190, 204, 215, 218, 226, 255, 60},
//19	10
	{126, 154, 189, 200, 205, 211, 233, 251, 47, 75},
//20	8
	{141, 174, 185, 196, 220, 236, 247, 62},
//21	7
	{156, 207, 221, 232, 235, 243, 77},
//22	5
	{143, 206, 217, 222, 228},
//23	4
	{158, 237, 253, 79},
//24	2
	{238, 249},
//25	1
	{239},
};

RINLINE keyc get_ctrl(unsigned long sum,unsigned int snlen,void const*sn)
{
	unsigned int want=(26-sum%26)%26;
	unsigned __int64 cnt;
	if(!QueryPerformanceCounter((LARGE_INTEGER*)&cnt)){
		srand(*(unsigned int*)sn);
		cnt=rand();
		cnt+=(unsigned __int64)(rand())<<32;
	}
	unsigned __int64 sn2=*(unsigned int*)sn;
	sn2^=(sn2<<17);
	cnt^=sn2;
	unsigned __int8*bytes=(unsigned __int8*)&cnt;
	unsigned __int8 prom=0xa5;
	for(int i=0;i<8;i++){
		prom^=bytes[i];
	}
	prom=prom%cnts[want];
	return ctrls[want][prom];
}

RINLINE keyc swap_nibbles(int const swap_nibbles_mode,keyc kc)
{
	switch(swap_nibbles_mode){
	case 0:
		return ((kc>>4)&0x0f)|(kc<<4);
		break;
	case 1:
		{
			union{
				keyc k;
				struct{
					keyc b01:2;
					keyc b23:2;
					keyc b45:2;
					keyc b67:2;
					keyc bxx:2;
				};
			};
			k=kc;
			bxx=b01;b01=b23;b23=bxx;
			bxx=b45;b45=b67;b67=bxx;
			kc=k;
		}
		break;
	case 2:
		{
			union{
				keyc k;
				struct{
					keyc b0:1;
					keyc b1:1;
					keyc b2:1;
					keyc b3:1;
					keyc b4:1;
					keyc b5:1;
					keyc b6:1;
					keyc b7:1;
					keyc bx:1;
				};
			};
			k=kc;
			bx=b0;b0=b1;b1=bx;
			bx=b2;b2=b3;b3=bx;
			bx=b4;b4=b5;b5=bx;
			bx=b6;b6=b7;b7=bx;
			kc=k;
		}
		break;
	case 3:
		{
			union{
				keyc k;
				struct{
					keyc b0:1;
					keyc b1:1;
					keyc b2:1;
					keyc b3:1;
					keyc b4:1;
					keyc b5:1;
					keyc b6:1;
					keyc b7:1;
					keyc bx:1;
				};
			};
			k=kc;
			bx=b0;b0=b3;b3=bx;
			bx=b1;b1=b2;b2=bx;
			bx=b4;b4=b7;b7=bx;
			bx=b5;b5=b6;b6=bx;
			kc=k;
		}
		break;
	default:
		kc|=0xa5;//никогда при нормальной работе сюда не должно попадать
		break;
	}
	return kc;
}

void add_sernum(algpar const&ap,unsigned int snlen,void const*sn,unsigned int inbuflen,keyc const*inbuf,keyc*outbuf)
{
	if(!outbuf)
		outbuf=(keyc*)inbuf;
	else
		memcpy(outbuf,inbuf,inbuflen);
	unsigned int i,j;
	unsigned int outbuflen=inbuflen+snlen+1;
	//дописываем в конец серийный номер
	memcpy(outbuf+inbuflen,sn,snlen);
	//вычисляем контрольный байт
	{
		unsigned long sum=0;
		i=outbuflen-1;
		while(i--)
			sum+=calc_ctrl(outbuf[i]);
		outbuf[outbuflen-1]=get_ctrl(sum,snlen,sn);
	}
	//а теперь поксорим чего-нибудь
	i=0;
	while(++i<outbuflen)
		outbuf[i-1]^=swap_nibbles(ap.swap_nibbles_mode,outbuf[i]);
	for(j=0;j<ap.xor_cycles;j++){
		for(i=0;i<outbuflen;i++){
			unsigned int ui=outbuf[i]%outbuflen;
			if(ui!=i)
				outbuf[ui]^=outbuf[i];
		}
	}
}

/*RINLINE */bool check_sernum(algpar const&ap,unsigned int snlen,void*sn,unsigned int inbuflen,keyc const*inbuf,keyc*outbuf)
{
	if(!outbuf)
		outbuf=(keyc*)inbuf;
	else
		memcpy(outbuf,inbuf,inbuflen);
	unsigned int i,j;
	unsigned int outbuflen=inbuflen-snlen-1;
	if(outbuflen>=inbuflen)
		return false;
	//уничтожаем последствия ксоров
	for(j=0;j<ap.xor_cycles;j++){
		i=inbuflen;
		while(i--){
			unsigned int ui=outbuf[i]%inbuflen;
			if(ui!=i)
				outbuf[ui]^=outbuf[i];
		}
	}
	i=inbuflen;
	while(--i)
		outbuf[i-1]^=swap_nibbles(ap.swap_nibbles_mode,outbuf[i]);
	//вычисляем контрольный байт
	bool ret;
	{
		unsigned long sum=0;
		i=inbuflen;
		while(i--)
			sum+=calc_ctrl(outbuf[i]);
		ret=(sum%26)==0;
	}
	if(ret){
		memcpy(sn,outbuf+outbuflen,snlen);
	}
	return ret;
}

void encode64(algpar const&ap,unsigned int inbuflen,keyc const*inbuf,char*outbuf)
{
	unsigned int const savedlen=inbuflen;
	keyc const*savedbuf=inbuf;
	ap.swapinbuf(ap,savedlen,savedbuf,true);
	static char const arr[64]={
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J',
		'K','L','M','N','O','P','Q','R','S','T',
		'U','V','W','X','Y','Z','a','b','c','d',
		'e','f','g','h','i','j','k','l','m','n',
		'o','p','q','r','s','t','u','v','w','x',
		'y','z','{','}',
	};
	bool len=true;
	while(inbuflen||len){
		union{
			struct{
				keyc b1;
				keyc b2;
				keyc b3;
			};
			struct{
				unsigned __int32 i1:6;
				unsigned __int32 i2:6;
				unsigned __int32 i3:6;
				unsigned __int32 i4:6;
			};
		};
		b1=0;
		b2=0;
		b3=0;
		if(inbuflen){
			b1=*inbuf++;
			inbuflen--;
		}
		else if(len){
			b1=*(unsigned __int8*)&savedlen;
			len=false;
		}
		if(inbuflen){
			b2=*inbuf++;
			inbuflen--;
		}
		else if(len){
			b2=*(unsigned __int8*)&savedlen;
			len=false;
		}
		if(inbuflen){
			b3=*inbuf++;
			inbuflen--;
		}
		else if(len){
			b3=*(unsigned __int8*)&savedlen;
			len=false;
		}
		*outbuf++=arr[i1];
		*outbuf++=arr[i2];
		*outbuf++=arr[i3];
		*outbuf++=arr[i4];
		*outbuf++=' ';
	}
	*outbuf++=0;
	ap.swapinbuf(ap,savedlen,savedbuf,false);
}

/*RINLINE */unsigned int decode64(algpar const&ap,char const*inbuf,keyc*outbuf)
{
	keyc*savedoutbuf=outbuf;
	union{
		struct{
			keyc b1;
			keyc b2;
			keyc b3;
		};
		struct{
			unsigned __int32 i1:6;
			unsigned __int32 i2:6;
			unsigned __int32 i3:6;
			unsigned __int32 i4:6;
		};
	};
	b1=0;
	b2=0;
	b3=0;
	int cc=0;
	while(*inbuf){
		unsigned __int8 newch=64;
		if('0'<=*inbuf&&*inbuf<='9')
			newch=*inbuf-'0';
		else if('A'<=*inbuf&&*inbuf<='Z')
			newch=*inbuf-'A'+10;
		else if('a'<=*inbuf&&*inbuf<='z')
			newch=*inbuf-'a'+36;
		else if(*inbuf=='{')
			newch=62;
		else if(*inbuf=='}')
			newch=63;
		if(newch<64){
			switch(cc){
			case 0:
				i1=newch;
				cc++;
				break;
			case 1:
				i2=newch;
				cc++;
				break;
			case 2:
				i3=newch;
				cc++;
				break;
			case 3:
				i4=newch;
				*outbuf++=b1;
				*outbuf++=b2;
				*outbuf++=b3;
				b1=0;
				b2=0;
				b3=0;
				cc=0;
				break;
			}
		}
		inbuf++;
	}
	switch(cc){
	case 1:
		*outbuf++=b1;
		break;
	case 2:
		*outbuf++=b1;
		*outbuf++=b2;
		break;
	case 3:
		*outbuf++=b1;
		*outbuf++=b2;
		*outbuf++=b3;
		break;
	}
	if(outbuf==savedoutbuf)
		return 0;
	while(!*--outbuf);
	ap.swapinbuf(ap,*outbuf,savedoutbuf,false);
	return *outbuf;
}

/*RINLINE */void keysfile(_TCHAR*fn,unsigned maxlen,_TCHAR const*kn/*=NULL*/,wchar_t const*dkf/*=NULL*/)
{
	if(maxlen)
		*fn=0;
	hkey base(KEY_READ,HKEY_LOCAL_MACHINE,kn?kn:_T("SOFTWARE\\Help Co"));
	_bstr_t bkfn=dkf?dkf:L"keysfile.txt";
	try{
		bkfn=base[_T("keysfile")];
	}
	catch(hkey::regerror&){
		//ожидаемое исключение
	}
	if(wcschr(static_cast<wchar_t const*>(bkfn),L'\\')==NULL){
		LPTSTR lpFilePart;
		DWORD sp=SearchPath(NULL,bkfn,NULL,maxlen,fn,&lpFilePart);
		if(sp>maxlen)
			*fn=0;
	}
	else if(bkfn.length()<maxlen)
		_tcscpy(fn,bkfn);
}

void swapinbufEQLD(algpar const&ap,unsigned int inbuflen,keyc const*inbuf,bool forward)
{
}

void swapinbufVSU(algpar const&ap,unsigned int inbuflen,keyc const*inbuf,bool forward)
{
	int i=ap.swapinbuf_cycles;
	int step=inbuflen/i;
	if(!forward)
		step=-step;
	int pos=forward?0:-step*(ap.swapinbuf_cycles-1);
	unsigned sw1,sw2;
	while(i--){
		sw1=inbuf[pos]%inbuflen;
		if(sw1!=pos){
			sw2=swap_nibbles(ap.swap_nibbles_mode,inbuf[pos])%inbuflen;
			if(sw2!=pos){
				keyc*buf=(keyc*)inbuf;
				keyc tmp=buf[sw1];buf[sw1]=buf[sw2];buf[sw2]=tmp;
			}
		}
		pos+=step;
	}
}
