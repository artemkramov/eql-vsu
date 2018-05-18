#include "stdafx.h"

#import "l2srv.tlb"

bool chkln(long ln){
	using namespace L2SRVLib;
	IHcCashes2Ptr ocp;
	HRESULT hr;
	bool autodetected=false;
	hr=ocp.CreateInstance(OLESTR("L2Srv.HcCfgManager"));
	if(SUCCEEDED(hr)){
retry:
		hr=ocp->Move(0);
		while(hr==S_OK){//S_FALSE означает eof
			tagcash_param id;
			unsigned char ucCom,ucAdr;
			hr=ocp->GetParameters(&ucCom,&ucAdr,&id);
			if(hr!=S_OK)
				break;
			if(long(id.log_num)==ln){
				if(!ocp->Saved()){
					ocp->Save();
				}
				return true;
			}
			hr=ocp->Next();
			if(FAILED(hr))
				break;
		}
		if(!autodetected){
			autodetected=true;
			hr=ocp->AutoDetect();
			if(SUCCEEDED(hr)){
				goto retry;
			}
		}
	}
	return false;
}