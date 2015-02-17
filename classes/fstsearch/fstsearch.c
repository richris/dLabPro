/* dLabPro class CFstsearch (fstsearch)
 * - Search processor for wFSTs.
 *
 * AUTHOR : frank.duckhorn
 * PACKAGE: dLabPro/classes
 *
 * This file was generated by dcg. DO NOT MODIFY! Modify fstsearch.def instead.
 * 
 * Copyright 2013 dLabPro contributors and others (see COPYRIGHT file) 
 * - Chair of System Theory and Speech Technology, TU Dresden
 * - Chair of Communications Engineering, BTU Cottbus
 * 
 * This file is part of dLabPro.
 * 
 * dLabPro is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * dLabPro is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with dLabPro. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dlp_cscope.h" /* Indicate C scope */
/*{{CGEN_INCLUDE */
/*}}CGEN_END */
#include "dlp_fstsearch.h"

/* Class CFstsearch */

void CFstsearch_Constructor(CFstsearch* _this, const char* lpInstanceName, BOOL bCallVirtual)
{
	DEBUGMSG(-1,"CFstsearch_Constructor; (bCallVirtual=%d)",(int)bCallVirtual,0,0);

#ifndef __cplusplus

	/* Register instance */
	dlp_xalloc_register_object('J',_this,1,sizeof(CFstsearch),
		__FILE__,__LINE__,"fstsearch",lpInstanceName);

	/* Create base instance */
	_this->m_lpBaseInstance = calloc(1,sizeof(CDlpObject));
	CDlpObject_Constructor(_this->m_lpBaseInstance,lpInstanceName,FALSE);

	/* Override virtual member functions */
	_this->m_lpBaseInstance->AutoRegisterWords = CFstsearch_AutoRegisterWords;
	_this->m_lpBaseInstance->Reset             = CFstsearch_Reset;
	_this->m_lpBaseInstance->Init              = CFstsearch_Init;
	_this->m_lpBaseInstance->Serialize         = CFstsearch_Serialize;
	_this->m_lpBaseInstance->SerializeXml      = CFstsearch_SerializeXml;
	_this->m_lpBaseInstance->Deserialize       = CFstsearch_Deserialize;
	_this->m_lpBaseInstance->DeserializeXml    = CFstsearch_DeserializeXml;
	_this->m_lpBaseInstance->Copy              = CFstsearch_Copy;
	_this->m_lpBaseInstance->ClassProc         = CFstsearch_ClassProc;
	_this->m_lpBaseInstance->GetInstanceInfo   = CFstsearch_GetInstanceInfo;
	_this->m_lpBaseInstance->IsKindOf          = CFstsearch_IsKindOf;
	_this->m_lpBaseInstance->Destructor        = CFstsearch_Destructor;
	_this->m_lpBaseInstance->ResetAllOptions   = CFstsearch_ResetAllOptions;

	/* Override pointer to derived instance */
	_this->m_lpBaseInstance->m_lpDerivedInstance = _this;

	#endif /* #ifndef __cplusplus */

	dlp_strcpy(BASEINST(_this)->m_lpClassName,"fstsearch");
	dlp_strcpy(BASEINST(_this)->m_lpObsoleteName,"");
	dlp_strcpy(BASEINST(_this)->m_lpProjectName,"fstsearch");
	dlp_strcpy(BASEINST(_this)->m_version.no,"2.00 DLC22");
	dlp_strcpy(BASEINST(_this)->m_version.date,"");
	BASEINST(_this)->m_nClStyle = CS_AUTOACTIVATE;

	if (bCallVirtual)
	{
		DLPASSERT(OK(INVOKE_VIRTUAL_0(AutoRegisterWords)));
		INVOKE_VIRTUAL_1(Init,TRUE);
	}
}

void CFstsearch_Destructor(CDlpObject* __this)
{
	GET_THIS_VIRTUAL(CFstsearch);
	{
	/*{{CGEN_DONECODE */
  CFstsearch_Unload(_this);
  DONE;
	/*}}CGEN_DONECODE */
	}

#ifndef __cplusplus

	/* Destroy base instance */
	CDlpObject_Destructor(_this->m_lpBaseInstance);
	dlp_free(_this->m_lpBaseInstance);
	_this->m_lpBaseInstance = NULL;

#endif /* #ifndef __cplusplus */
}

INT16 CFstsearch_AutoRegisterWords(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	DEBUGMSG(-1,"CFstsearch_AutoRegisterWords",0,0,0);

	/* Call base class implementation */
	IF_NOK(INVOKE_BASEINST_0(AutoRegisterWords)) return NOT_EXEC;

	/*{{CGEN_REGISTERWORDS */

	/* Register methods */
	REGISTER_METHOD("-backtrack","",LPMF(CFstsearch,OnBacktrack),"Backtrack from the current decoding state",0,"<fst itDst> <fstsearch this>","")
	REGISTER_METHOD("-isearch","",LPMF(CFstsearch,OnIsearch),"Iterative search: decode frames in idWeights",0,"<data idWeights> <fstsearch this>","")
	REGISTER_METHOD("-load","",LPMF(CFstsearch,OnLoad),"Load the source automaton",0,"<fst itSrc> <long nUnit> <fstsearch this>","")
	REGISTER_METHOD("-restart","",LPMF(CFstsearch,OnRestart),"Reset iterative search",0,"<fstsearch this>","")
	REGISTER_METHOD("-search","",LPMF(CFstsearch,OnSearch),"All steps in one method",0,"<fst itSrc> <long nUnit> <data idWeights> <fst itDst> <fstsearch this>","")

	/* Register options */
	REGISTER_OPTION("/fast","",LPMV(m_bFast),NULL,"Do not copy the source automaton for backtracking",0)
	REGISTER_OPTION("/final","",LPMV(m_bFinal),NULL,"Decode until a final state is reached",0)
	REGISTER_OPTION("/start","",LPMV(m_bStart),NULL,"Start state in every frame",0)

	/* Register fields */
	REGISTER_FIELD("algo","",LPMV(m_lpsAlgo),LPMF(CFstsearch,OnAlgoChanged),"Search algorithm to use",0,5000,1,"string","tp")
	REGISTER_FIELD("as_aheutype","",LPMV(m_lpsAsAheutype),LPMF(CFstsearch,OnAsAheutypeChanged),"Timeinvariant heurisitc type.",0,5000,1,"string","pot")
	REGISTER_FIELD("as_prnf","",LPMV(m_nAsPrnf),LPMF(CFstsearch,OnAsPrnfChanged),"Frame prune threshold",0,2008,1,"long",(INT64)0)
	REGISTER_FIELD("as_prnw","",LPMV(m_nAsPrnw),LPMF(CFstsearch,OnAsPrnwChanged),"Weight prune threshold",0,3008,1,"double",(FLOAT64)0)
	REGISTER_FIELD("as_qsize","",LPMV(m_nAsQsize),LPMF(CFstsearch,OnAsQsizeChanged),"Queue size",0,2008,1,"long",(INT64)0)
	REGISTER_FIELD("as_sheutype","",LPMV(m_lpsAsSheutype),LPMF(CFstsearch,OnAsSheutypeChanged),"Timevariant heurisitc type.",0,5000,1,"string","min")
	REGISTER_FIELD("bt","",LPMV(m_lpsBt),LPMF(CFstsearch,OnBtChanged),"Backtrack type",0,5000,1,"string","os")
	REGISTER_FIELD("glob","",LPMV(m_lpGlob),NULL,"Internal use: Search status",FF_HIDDEN | FF_NOSET | FF_NOSAVE,6000,1,"void*",NULL)
	REGISTER_FIELD("latprn","",LPMV(m_nLatprn),LPMF(CFstsearch,OnLatprnChanged),"Lattice pruning threshold",0,3008,1,"double",(FLOAT64)0)
	REGISTER_FIELD("loaded","",LPMV(m_bLoaded),NULL,"TRUE if a transducer is loaded and the decoder ready",FF_NOSET,1000,1,"bool",(BOOL)FALSE)
	REGISTER_FIELD("mem","",LPMV(m_nMem),NULL,"Memory used for last decoding including backtracking in Bytes",FF_NOSET,3008,1,"double",(FLOAT64)0)
	REGISTER_FIELD("numpaths","",LPMV(m_nNumpaths),LPMF(CFstsearch,OnNumpathsChanged),"Number of paths to decode",0,2008,1,"long",(INT64)1)
	REGISTER_FIELD("sdp_epsremove","",LPMV(m_bSdpEpsremove),LPMF(CFstsearch,OnSdpEpsremoveChanged),"Removes epsilon transitions",0,1000,1,"bool",(BOOL)FALSE)
	REGISTER_FIELD("sdp_fwd","",LPMV(m_bSdpFwd),LPMF(CFstsearch,OnSdpFwdChanged),"Do forward algorithm instead of sdp",0,1000,1,"bool",(BOOL)FALSE)
	REGISTER_FIELD("sdp_prn","",LPMV(m_nSdpPrn),LPMF(CFstsearch,OnSdpPrnChanged),"Pruning constant",0,3008,1,"double",(FLOAT64)0)
	REGISTER_FIELD("stkprn","",LPMV(m_bStkprn),LPMF(CFstsearch,OnStkprnChanged),"Prune hypothesis with different pushdown memory",0,1000,1,"bool",(BOOL)FALSE)
	REGISTER_FIELD("time","",LPMV(m_nTime),NULL,"Time used for last decoding including backtracking in milliseconds (only Linux!)",FF_NOSET,3008,1,"double",(FLOAT64)0)
	REGISTER_FIELD("tp_prnh","",LPMV(m_nTpPrnh),LPMF(CFstsearch,OnTpPrnhChanged),"Hypothesis number pruning threshold",0,2008,1,"long",(INT64)0)
	REGISTER_FIELD("tp_prnw","",LPMV(m_nTpPrnw),LPMF(CFstsearch,OnTpPrnwChanged),"Weight pruning threshold",0,3008,1,"double",(FLOAT64)0)
	REGISTER_FIELD("tp_threads","",LPMV(m_nTpThreads),LPMF(CFstsearch,OnTpThreadsChanged),"Number of threads to use for decoding",0,2008,1,"long",(INT64)1)

	/* Register errors */
	REGISTER_ERROR("~e1_0_0__1",EL_ERROR,FSTS_STR,"%s")
	/*}}CGEN_REGISTERWORDS */

	return O_K;
}

INT16 CFstsearch_Init(CDlpObject* __this, BOOL bCallVirtual)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	DEBUGMSG(-1,"CFstsearch_Init, (bCallVirtual=%d)",(int)bCallVirtual,0,0);
	{
	/*{{CGEN_INITCODE */
  INIT;
	/*}}CGEN_INITCODE */
	}

	/* If last derivation call reset (do not reset members; already done by Init()) */
#ifndef __NORTTI
	if (bCallVirtual) return INVOKE_VIRTUAL_1(Reset,FALSE); else
#endif
	                  return O_K;
}

INT16 CFstsearch_Reset(CDlpObject* __this, BOOL bResetMembers)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	DEBUGMSG(-1,"CFstsearch_Reset; (bResetMembers=%d)",(int)bResetMembers,0,0);
	{
	/*{{CGEN_RESETCODE */
  CFstsearch_Unload(_this);
  RESET;
	/*}}CGEN_RESETCODE */
	}

	return O_K;
}

INT16 CFstsearch_ClassProc(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
	/*{{CGEN_CLASSCODE */
  return CLASSPROC;
	/*}}CGEN_CLASSCODE */
	}

	return O_K;
}

#define CODE_DN3 /* check this for xml specific save code */
#define SAVE  SAVE_DN3
INT16 CFstsearch_Serialize(CDlpObject* __this, CDN3Stream* lpDest)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
	/*{{CGEN_SAVECODE */
  return SAVE;
	/*}}CGEN_SAVECODE */
	}

	return O_K;
}
#undef  SAVE
#undef  CODE_DN3

#define CODE_XML /* check this for xml specific save code */
#define SAVE  SAVE_XML
INT16 CFstsearch_SerializeXml(CDlpObject* __this, CXmlStream* lpDest)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
	/*{{CGEN_SAVECODE */
  return SAVE;
	/*}}CGEN_SAVECODE */
	}

	return O_K;
}
#undef  SAVE
#undef  CODE_XML

#define CODE_DN3 /* check this for dn3 specific restore code */
#define RESTORE  RESTORE_DN3
INT16 CFstsearch_Deserialize(CDlpObject* __this, CDN3Stream* lpSrc)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
	/*{{CGEN_RESTORECODE */
  return RESTORE;
	/*}}CGEN_RESTORECODE */
	}

	return O_K;
}
#undef  RESTORE
#undef  CODE_DN3

#define CODE_XML /* check this for xml specific restore code */
#define RESTORE  RESTORE_XML
INT16 CFstsearch_DeserializeXml(CDlpObject* __this, CXmlStream* lpSrc)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
	/*{{CGEN_RESTORECODE */
  return RESTORE;
	/*}}CGEN_RESTORECODE */
	}

	return O_K;
}
#undef  RESTORE
#undef  CODE_XML

INT16 CFstsearch_Copy(CDlpObject* __this, CDlpObject* __iSrc)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
	/*{{CGEN_COPYCODE */
  return COPY;
	/*}}CGEN_COPYCODE */
	}

	return O_K;
}

/* Runtime class type information and class factory */
INT16 CFstsearch_InstallProc(void* lpItp)
{
	{
	/*{{CGEN_INSTALLCODE */
  return INSTALL;
	/*}}CGEN_INSTALLCODE */
	}

	return O_K;
}

CFstsearch* CFstsearch_CreateInstance(const char* lpName)
{
	CFstsearch* lpNewInstance;
	ICREATEEX(CFstsearch,lpNewInstance,lpName,NULL);
	return lpNewInstance;
}

INT16 CFstsearch_GetClassInfo(SWord* lpClassWord)
{
	if (!lpClassWord) return NOT_EXEC;
	dlp_memset(lpClassWord,0,sizeof(SWord));

	lpClassWord->nWordType          = WL_TYPE_FACTORY;
	lpClassWord->nFlags             = CS_AUTOACTIVATE;

#ifdef __cplusplus

	lpClassWord->ex.fct.lpfFactory  = (LP_FACTORY_PROC)CFstsearch::CreateInstance;
	lpClassWord->ex.fct.lpfInstall  = CFstsearch::InstallProc;

#else /* #ifdef __DLP_CSCOPE */

	lpClassWord->ex.fct.lpfFactory  = (LP_FACTORY_PROC)CFstsearch_CreateInstance;
	lpClassWord->ex.fct.lpfInstall  = CFstsearch_InstallProc;

#endif /* #ifdef __DLP_CSCOPE */

	lpClassWord->ex.fct.lpProject   = "fstsearch";
	lpClassWord->ex.fct.lpBaseClass = "-";
	lpClassWord->lpComment          = "Search processor for wFSTs.";
	lpClassWord->ex.fct.lpAutoname  = "";
	lpClassWord->ex.fct.lpCname     = "CFstsearch";
	lpClassWord->ex.fct.lpAuthor    = "frank.duckhorn";

	dlp_strcpy(lpClassWord->lpName             ,"fstsearch");
	dlp_strcpy(lpClassWord->lpObsname          ,"");
	dlp_strcpy(lpClassWord->ex.fct.version.no  ,"2.00 DLC22");

	return O_K;
}

INT16 CFstsearch_GetInstanceInfo(CDlpObject* __this, SWord* lpClassWord)
{
	return CFstsearch_GetClassInfo(lpClassWord);
}

BOOL CFstsearch_IsKindOf(CDlpObject* __this, const char* lpClassName)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);

  if (dlp_strncmp(lpClassName,"fstsearch",L_NAMES) == 0) return TRUE;
	else return INVOKE_BASEINST_1(IsKindOf,lpClassName);
}

INT16 CFstsearch_ResetAllOptions(CDlpObject* __this, BOOL bInit)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	DEBUGMSG(-1,"CFstsearch_ResetAllOptions;",0,0,0);
	{
	/*{{CGEN_RESETALLOPTIONS*/
	_this->m_bFast = FALSE;
	_this->m_bFinal = FALSE;
	_this->m_bStart = FALSE;
	/*}}CGEN_RESETALLOPTIONS*/
	}

	return INVOKE_BASEINST_1(ResetAllOptions,bInit);
}

/* Generated primary method invocation functions */

#ifndef __NOITP
/*{{CGEN_CPMIC */
INT16 CFstsearch_OnBacktrack(CDlpObject* __this)
/* DO NOT CALL THIS FUNCTION FROM C++ SCOPE.     */
/* IT MAY INTERFERE WITH THE INTERPRETER SESSION */
{
	INT16 __nErr    = O_K;
	INT32  __nErrCnt = 0;
	fst* itDst;
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	MIC_CHECK;
	__nErrCnt = CDlpObject_GetErrorCount();
	itDst = MIC_GET_I_EX(itDst,fst,1,1);
	if (CDlpObject_GetErrorCount()>__nErrCnt) return NOT_EXEC;
	__nErr = CFstsearch_Backtrack(_this, itDst);
	return __nErr;
}

INT16 CFstsearch_OnIsearch(CDlpObject* __this)
/* DO NOT CALL THIS FUNCTION FROM C++ SCOPE.     */
/* IT MAY INTERFERE WITH THE INTERPRETER SESSION */
{
	INT16 __nErr    = O_K;
	INT32  __nErrCnt = 0;
	data* idWeights;
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	MIC_CHECK;
	__nErrCnt = CDlpObject_GetErrorCount();
	idWeights = MIC_GET_I_EX(idWeights,data,1,1);
	if (CDlpObject_GetErrorCount()>__nErrCnt) return NOT_EXEC;
	__nErr = CFstsearch_Isearch(_this, idWeights);
	return __nErr;
}

INT16 CFstsearch_OnLoad(CDlpObject* __this)
/* DO NOT CALL THIS FUNCTION FROM C++ SCOPE.     */
/* IT MAY INTERFERE WITH THE INTERPRETER SESSION */
{
	INT16 __nErr    = O_K;
	INT32  __nErrCnt = 0;
	fst* itSrc;
	INT64 nUnit;
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	MIC_CHECK;
	__nErrCnt = CDlpObject_GetErrorCount();
	nUnit = (INT64)MIC_GET_N(1,0);
	itSrc = MIC_GET_I_EX(itSrc,fst,2,1);
	if (CDlpObject_GetErrorCount()>__nErrCnt) return NOT_EXEC;
	__nErr = CFstsearch_Load(_this, itSrc, nUnit);
	return __nErr;
}

INT16 CFstsearch_OnRestart(CDlpObject* __this)
/* DO NOT CALL THIS FUNCTION FROM C++ SCOPE.     */
/* IT MAY INTERFERE WITH THE INTERPRETER SESSION */
{
	INT16 __nErr    = O_K;
	INT32  __nErrCnt = 0;
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	MIC_CHECK;
	__nErrCnt = CDlpObject_GetErrorCount();
	if (CDlpObject_GetErrorCount()>__nErrCnt) return NOT_EXEC;
	__nErr = CFstsearch_Restart(_this);
	return __nErr;
}

INT16 CFstsearch_OnSearch(CDlpObject* __this)
/* DO NOT CALL THIS FUNCTION FROM C++ SCOPE.     */
/* IT MAY INTERFERE WITH THE INTERPRETER SESSION */
{
	INT16 __nErr    = O_K;
	INT32  __nErrCnt = 0;
	fst* itSrc;
	INT64 nUnit;
	data* idWeights;
	fst* itDst;
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	MIC_CHECK;
	__nErrCnt = CDlpObject_GetErrorCount();
	itDst = MIC_GET_I_EX(itDst,fst,1,1);
	idWeights = MIC_GET_I_EX(idWeights,data,2,2);
	nUnit = (INT64)MIC_GET_N(3,0);
	itSrc = MIC_GET_I_EX(itSrc,fst,4,3);
	if (CDlpObject_GetErrorCount()>__nErrCnt) return NOT_EXEC;
	__nErr = CFstsearch_Search(_this, itSrc, nUnit, idWeights, itDst);
	return __nErr;
}

/*}}CGEN_CPMIC */
#endif /* #ifndef __NOITP */


/* Generated secondary method invocation functions */

/*{{CGEN_CSMIC */
/*}}CGEN_CSMIC */


/* Generated option change callback functions */

/*{{CGEN_COCCF */
/*}}CGEN_COCCF */


/* Generated field change callback functions */

/*{{CGEN_CFCCF */
INT16 CFstsearch_OnAlgoChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnAsAheutypeChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnAsPrnfChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnAsPrnwChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnAsQsizeChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnAsSheutypeChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnBtChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnLatprnChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnNumpathsChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnSdpEpsremoveChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnSdpFwdChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnSdpPrnChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnStkprnChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnTpPrnhChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnTpPrnwChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

INT16 CFstsearch_OnTpThreadsChanged(CDlpObject* __this)
{
	GET_THIS_VIRTUAL_RV(CFstsearch,NOT_EXEC);
	{
  	CFstsearch_Restart(_this);
	}

	return O_K;
}

/*}}CGEN_CFCCF */


/* C++ wrapper functions */
#ifdef __cplusplus

#define _this this

CFstsearch::CFstsearch(const char* lpInstanceName, BOOL bCallVirtual) : inherited(lpInstanceName,0)
{
	CFstsearch_Constructor(this,lpInstanceName,bCallVirtual);
}

CFstsearch::~CFstsearch()
{
	CFstsearch_Destructor(this);
}

INT16 CFstsearch::AutoRegisterWords()
{
	return CFstsearch_AutoRegisterWords(this);
}

INT16 CFstsearch::Init(BOOL bCallVirtual)
{
	return CFstsearch_Init(this,bCallVirtual);
}

INT16 CFstsearch::Reset(BOOL bResetMembers)
{
	return CFstsearch_Reset(this,bResetMembers);
}

INT16 CFstsearch::ClassProc()
{
	return CFstsearch_ClassProc(this);
}

INT16 CFstsearch::Serialize(CDN3Stream* lpDest)
{
	return CFstsearch_Serialize(this,lpDest);
}

INT16 CFstsearch::SerializeXml(CXmlStream* lpDest)
{
	return CFstsearch_SerializeXml(this,lpDest);
}

INT16 CFstsearch::Deserialize(CDN3Stream* lpSrc)
{
	return CFstsearch_Deserialize(this,lpSrc);
}

INT16 CFstsearch::DeserializeXml(CXmlStream* lpSrc)
{
	return CFstsearch_DeserializeXml(this,lpSrc);
}

INT16 CFstsearch::Copy(CDlpObject* __iSrc)
{
	return CFstsearch_Copy(this,__iSrc);
}

INT16 CFstsearch::InstallProc(void* lpItp)
{
	return CFstsearch_InstallProc(lpItp);
}

CFstsearch* CFstsearch::CreateInstance(const char* lpName)
{
	return CFstsearch_CreateInstance(lpName);
}

INT16 CFstsearch::GetClassInfo(SWord* lpClassWord)
{
	return CFstsearch_GetClassInfo(lpClassWord);
}

INT16 CFstsearch::GetInstanceInfo(SWord* lpClassWord)
{
	return CFstsearch_GetInstanceInfo(this,lpClassWord);
}

BOOL CFstsearch::IsKindOf(const char* lpClassName)
{
	return CFstsearch_IsKindOf(this,lpClassName);
}

INT16 CFstsearch::ResetAllOptions(BOOL bInit)
{
	return CFstsearch_ResetAllOptions(this,bInit);
}

#ifndef __NOITP
/*{{CGEN_PMIC */
INT16 CFstsearch::OnBacktrack()
/* DO NOT CALL THIS FUNCTION FROM C++ SCOPE.     */
/* IT MAY INTERFERE WITH THE INTERPRETER SESSION */
{
	return CFstsearch_OnBacktrack(this);
}

INT16 CFstsearch::OnIsearch()
/* DO NOT CALL THIS FUNCTION FROM C++ SCOPE.     */
/* IT MAY INTERFERE WITH THE INTERPRETER SESSION */
{
	return CFstsearch_OnIsearch(this);
}

INT16 CFstsearch::OnLoad()
/* DO NOT CALL THIS FUNCTION FROM C++ SCOPE.     */
/* IT MAY INTERFERE WITH THE INTERPRETER SESSION */
{
	return CFstsearch_OnLoad(this);
}

INT16 CFstsearch::OnRestart()
/* DO NOT CALL THIS FUNCTION FROM C++ SCOPE.     */
/* IT MAY INTERFERE WITH THE INTERPRETER SESSION */
{
	return CFstsearch_OnRestart(this);
}

INT16 CFstsearch::OnSearch()
/* DO NOT CALL THIS FUNCTION FROM C++ SCOPE.     */
/* IT MAY INTERFERE WITH THE INTERPRETER SESSION */
{
	return CFstsearch_OnSearch(this);
}

/*}}CGEN_PMIC */
#endif /* #ifndef __NOITP */

/*{{CGEN_SMIC */
/*}}CGEN_SMIC */

/*{{CGEN_OCCF */
/*}}CGEN_OCCF */

/*{{CGEN_FCCF */
INT16 CFstsearch::OnAlgoChanged()
{
	return CFstsearch_OnAlgoChanged(this);
}

INT16 CFstsearch::OnAsAheutypeChanged()
{
	return CFstsearch_OnAsAheutypeChanged(this);
}

INT16 CFstsearch::OnAsPrnfChanged()
{
	return CFstsearch_OnAsPrnfChanged(this);
}

INT16 CFstsearch::OnAsPrnwChanged()
{
	return CFstsearch_OnAsPrnwChanged(this);
}

INT16 CFstsearch::OnAsQsizeChanged()
{
	return CFstsearch_OnAsQsizeChanged(this);
}

INT16 CFstsearch::OnAsSheutypeChanged()
{
	return CFstsearch_OnAsSheutypeChanged(this);
}

INT16 CFstsearch::OnBtChanged()
{
	return CFstsearch_OnBtChanged(this);
}

INT16 CFstsearch::OnLatprnChanged()
{
	return CFstsearch_OnLatprnChanged(this);
}

INT16 CFstsearch::OnNumpathsChanged()
{
	return CFstsearch_OnNumpathsChanged(this);
}

INT16 CFstsearch::OnSdpEpsremoveChanged()
{
	return CFstsearch_OnSdpEpsremoveChanged(this);
}

INT16 CFstsearch::OnSdpFwdChanged()
{
	return CFstsearch_OnSdpFwdChanged(this);
}

INT16 CFstsearch::OnSdpPrnChanged()
{
	return CFstsearch_OnSdpPrnChanged(this);
}

INT16 CFstsearch::OnStkprnChanged()
{
	return CFstsearch_OnStkprnChanged(this);
}

INT16 CFstsearch::OnTpPrnhChanged()
{
	return CFstsearch_OnTpPrnhChanged(this);
}

INT16 CFstsearch::OnTpPrnwChanged()
{
	return CFstsearch_OnTpPrnwChanged(this);
}

INT16 CFstsearch::OnTpThreadsChanged()
{
	return CFstsearch_OnTpThreadsChanged(this);
}

/*}}CGEN_FCCF */

/*{{CGEN_CXXWRAP */
INT16 CFstsearch::Load(CFst* itSrc, long nUnit)
{
	return CFstsearch_Load(this, itSrc, nUnit);
}

INT16 CFstsearch::Isearch(CData* idWeights)
{
	return CFstsearch_Isearch(this, idWeights);
}

INT16 CFstsearch::Backtrack(CFst* itDst)
{
	return CFstsearch_Backtrack(this, itDst);
}

INT16 CFstsearch::Unload()
{
	return CFstsearch_Unload(this);
}

INT16 CFstsearch::Restart()
{
	return CFstsearch_Restart(this);
}

INT16 CFstsearch::Search(CFst* itSrc, long nUnit, CData* idWeights, CFst* itDst)
{
	return CFstsearch_Search(this, itSrc, nUnit, idWeights, itDst);
}

/*}}CGEN_CXXWRAP */

#endif /* #ifdef __cplusplus */

/* EOF */
