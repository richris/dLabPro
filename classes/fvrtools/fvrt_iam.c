/* dLabPro class CFvrtools (fvrtools)
 * - Class CFvrtools interactive methods (user defined SMI functions)
 *
 * AUTHOR : Matthias Wolff, Werner Mexer
 * PACKAGE: dLabPro/classes
 *
 * Copyright 2013-2015 dLabPro contributors and others (see COPYRIGHT file)
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
#include "dlp_fvrtools.h"

/*
 * Documentation in fvrtools.def
 */
BOOL CGEN_PUBLIC CFvrtools_IsFvr(CFvrtools* _this, INT32 nU, CFst* itFvr)
{
  FST_TID_TYPE* lpTI   = NULL;                                                  /* Transition iterator               */
  BYTE*         lpT    = NULL;                                                  /* Transition pointer                */
  BOOL          bRet   = FALSE;                                                 /* Return value                      */
  FST_STYPE     nIsFvr = -1;                                                    /* Index of input symbol "FVR"       */

  /* Simple tests */                                                            /* --------------------------------- */
  if (itFvr==NULL)                                                              /* FST is NULL                       */
    return OK(IERROR(_this,ERR_NULLINST,"itFvr",0,0));                          /*   but must not be                 */
  if (nU<0 || nU>=UD_XXU(itFvr))                                                /* Unit index is invalid             */
    return OK(IERROR(itFvr,FST_BADID,"unit",nU,0));                             /*   but must not be                 */
  if ((nIsFvr = CFvrtools_FindIs("FVR",FALSE,itFvr))<0)                         /* Find input symbol "FVR"           */
    return FALSE;                                                               /*   Not found -> not an FVR         */

  /* Structure test */                                                          /* --------------------------------- */
  if (CFst_Analyze(itFvr,nU,FST_FWDTREE)==0) return FALSE;                      /* Not a forward tree -> not an FVR  */

  /* - Iterate transitions starting at root; NO RETURNS BEYOND HERE! */         /*                                   */
  lpTI=CFst_STI_Init(itFvr,nU,0);                                               /* Initialize transition iterator    */
  if (lpTI->nOfTTis<=0) goto L_EXCEPTION;                                       /* No input symbols -> not an FVR    */
  lpT=CFst_STI_TfromS(lpTI,0,lpT);                                              /* Get 1st transition from root      */
  if (lpT==NULL) goto L_EXCEPTION;                                              /* There is none -> not an FVR       */
  if (nIsFvr==*CFst_STI_TTis(lpTI,lpT))                                         /* Input symbol is "FVR"             */
    bRet = TRUE;                                                                /*   Looks very good...              */
  if (CFst_STI_TfromS(lpTI,0,lpT)!=NULL)                                        /* There are more trans. from root   */
    bRet = FALSE;                                                               /*   Not so good, after all :(       */

L_EXCEPTION:                                                                    /* : Clean exit label                */
  CFst_STI_Done(lpTI);                                                          /* Release iterator                  */
  return bRet;                                                                  /* Return result                     */
}

/*
 * Documentation in fvrtools.def
 */
INT16 CGEN_PUBLIC CFvrtools_FromString(CFvrtools* _this, const char* lpsSrc, CFst* itFvr)
{
  INT16 nRet = O_K;                                                             /* The return value                  */

  /* Initialization */                                                          /* --------------------------------- */
  CHECK_THIS_RV(NOT_EXEC);                                                      /* Check this instance               */
  if (dlp_strlen(lpsSrc)==0)                                                    /* Input empty                       */
    FVRT_EXCEPTION(FVRT_SEQSYNTAX,"must not be empty",0,0);                     /*   Error message and exit          */

  /* Create wFVR from string representation */                                  /* --------------------------------- */
  IF_NOK(nRet=CFvrtools_StrToSeq(_this,lpsSrc,itFvr)) goto L_EXCEPTION;         /* Create normalized sequence FST    */
  IF_NOK(nRet=CFvrtools_SeqToFvr(_this,itFvr,itFvr) ) goto L_EXCEPTION;         /* Create FVR                        */

  /* Clean-up */                                                                /* --------------------------------- */
L_EXCEPTION:                                                                    /* : Clean exit label                */
  return nRet;                                                                  /* Return                            */
}

/**
 * Check a sequence FST symbol table.
 * This function fails if there are input symbols containing a square bracket [ or ] and other characters.
 * Furthermore this function removes all strings in round brackets (XX) from the input symbols.
 * All transitions with empty string as input symbol afterwards are converted to epsilon transitions.
 * If itSeq is NULL no changes are mode to itSeq or idS.
 *
 * @param _this
 *          This instance of CFvrtools.
 * @param itSeq
 *          The sequence FST in unit 0 (maybe NULL for no changes).
 * @param idS
 *          The symbol table to check (maybe itSeq->is or itSeq->os).
 * @param nBO
 *          Return pointer for opening bracket index
 * @param nBC
 *          Return pointer for closing bracket index
 * @return <code>O_K</code> if successful, a (negative) error code otherwise.
 */
INT16 CGEN_PRIVATE CFvrtools_CheckSeq(CFvrtools* _this, CFst* itSeq, CData* idS, INT32 *pBO, INT32 *pBC)
{
  INT16 nRet = O_K;                                                             /* The return value                  */
  INT32 nI,nC;                                                                  /* Symbol, character index           */
  INT32 nXI;                                                                    /* Number of input symbols           */
  INT32 nOI;                                                                    /* Input symbol record length        */
  INT32 nXC;                                                                    /* Number of characters per symbol   */
  INT32 nBO=-1;                                                                 /* Opening bracket index             */
  INT32 nBC=-1;                                                                 /* Closing bracket index             */
  char *lpI;                                                                    /* Input symbol pointer              */
  FST_TID_TYPE *lpTI;                                                           /* Transition interator              */
  BYTE* lpT;                                                                    /* Transition pointer                */

  /* Initialization */                                                          /* --------------------------------- */
  CHECK_THIS_RV(NOT_EXEC);                                                      /* Check this instance               */
  if (idS==NULL || CData_IsEmpty(idS))                                          /* Symbol table is empty             */
    FVRT_EXCEPTION(ERR_NULLINST,"idS is empty",0,0);                            /*   Error message and exit          */

  nXI=CData_GetNRecs(idS);                                                      /* Get number of symbols             */
  nOI=CData_GetRecLen(idS);                                                     /* Get input symbol record length    */
  nXC=CData_GetCompType(idS,0);                                                 /* Get number of characters per symb.*/

  /* Remove (XX) in output symbols & check for singular [ or ] */               /* --------------------------------- */
  lpI=(char*)CData_XAddr(idS,0,0);                                              /* Get symbol pointer                */
  for(nI=0;nI<nXI;nI++,lpI+=nOI){                                               /* Loop over input symbols >>        */
    INT32 nBrk=0;                                                               /*   Bracket depth                   */
    INT32 nCd=0;                                                                /*   Character writing index         */
    for(nC=0;nC<nXC && lpI[nC];nC++){                                           /*   Loop over characters >>         */
      if((lpI[nC]=='[' || lpI[nC]==']') && (nC || lpI[nC+1]))                   /*     Check for singular [ or ]     */
        FVRT_EXCEPTION(FVRT_SEQSYNTAX,"Symbol [ or ] occurs with other characters",0,0); /* Check failed             */
      if(lpI[nC]=='[') nBO=nI;                                                  /*     Return opening bracket index  */
      if(lpI[nC]==']') nBC=nI;                                                  /*     Return closing bracket index  */
      if(!itSeq) continue;                                                      /*     No changes                    */
      if(lpI[nC]=='(') nBrk++;                                                  /*     Increase bracket depth on open*/
      if(!nBrk) lpI[nCd++]=lpI[nC];                                             /*     Copy char. if outside bracket */
      if(lpI[nC]==')') nBrk--;                                                  /*     Decrease brck. depth on close */
    }                                                                           /*   <<                              */
    if(itSeq) lpI[nCd]='\0';                                                    /*   End of string                   */
  }                                                                             /* <<                                */
  if(nBO<0 || nBC<0) FVRT_EXCEPTION(FVRT_SEQSYNTAX,"Symbol [ or ] missing",0,0);/* Check existance of bracket symbols*/
  if(pBO) *pBO=nBO;                                                             /* Export opening bracket index      */
  if(pBC) *pBC=nBC;                                                             /* Export closing bracket index      */
  if(!itSeq) return nRet;                                                       /* No changes -> return              */

  /* Remove empty output symbols */
  lpI=(char*)CData_XAddr(idS,0,0);                                              /* Get symbol pointer                */
  lpTI=CFst_STI_Init(itSeq,0,0);                                                /* Setup iterator without sorting    */
  for(lpT=lpTI->lpFT ; lpT<lpTI->lpFT+lpTI->nRlt*lpTI->nXT ; lpT+=lpTI->nRlt){  /* Loop over all transitions >>      */
    FST_STYPE *nTis=CFst_STI_TTis(lpTI,lpT);                                    /*   Get input symbol pointer        */
    if(*nTis>=0 && *nTis<nXI && !lpI[*nTis*nOI]) *nTis=-1;                      /*   Clear input sym. on empty string*/
  }                                                                             /* <<                                */
  CFst_STI_Done(lpTI);                                                          /* Finalize iterator                 */

  /* Clean-up */                                                                /* --------------------------------- */
L_EXCEPTION:                                                                    /* : Clean exit label                */
  return nRet;                                                                  /* Return                            */
}

/*
 * Documentation in fvrtools.def
 */
INT16 CGEN_PUBLIC CFvrtools_FromFst(CFvrtools* _this, CFst* itSrc, CFst* itFvr)
{
  INT16 nRet = O_K;                                                             /* The return value                  */

  /* Initialization */                                                          /* --------------------------------- */
  CHECK_THIS_RV(NOT_EXEC);                                                      /* Check this instance               */
  if (itSrc==NULL)                                                              /* Output not correct instance       */
    FVRT_EXCEPTION(ERR_NULLINST,"itDst is NULL",0,0);                           /*   Error message and exit          */
  /* Create wFVR from sequence FST  */                                          /* --------------------------------- */
  CFst_CopyUi(itSrc,itFvr,NULL,0);                                              /* Get the first unit                */
  IF_NOK(nRet=CFvrtools_CheckSeq(_this,itFvr,AS(CData,itFvr->is),NULL,NULL) )   /* Check sequence symbol table       */
    goto L_EXCEPTION;                                                           /* |                                 */
  IF_NOK(nRet=CFvrtools_SeqToFvr(_this,itFvr,itFvr) ) goto L_EXCEPTION;         /* Create FVR                        */

  /* Clean-up */                                                                /* --------------------------------- */
L_EXCEPTION:                                                                    /* : Clean exit label                */
  return nRet;                                                                  /* Return                            */
}

/*
 * Documentation in fvrtools.def
 */
INT16 CGEN_PUBLIC CFvrtools_Synthesize(CFvrtools* _this, CFst* itDst, CFst* itFvr)
{
  INT16         nRet        = NULL;                                             /* The return value                  */
  FST_ITYPE     nCTIS;                                                          /* Find comp ~Tis                    */
  FST_ITYPE     nCTOS;                                                          /* Find comp ~Tos                    */
  FST_ITYPE     nTis        = -1;                                               /* Terminal input symbol             */
  FST_ITYPE     rank        = NULL;                                             /* Rank of actually iteration(tran.) */
  FST_ITYPE     nMyIniState = 0;                                                /* State iterator of original Fvr    */
  FST_ITYPE     nRecIdSym   = 0;                                                /* variable for NRec of idSym        */
  FST_ITYPE     nComp       = 0;                                                /* Numbers of possible constellation */
  CData*        idRank      = NULL;                                             /* rank of source Fvr                */
  CData*        idSym       = NULL;                                             /* Symbol const. of act. permutation */
  CData*        idSymRef    = NULL;                                             /* Reference(rank) value of idSym    */
  CData*        idSymList   = NULL;                                             /* All variation of symbol const.    */
  CData*        idStArray   = NULL;                                             /* Store number of State as ref.     */
  CData*        idStList    = NULL;                                             /* Store number of State as ref. list*/
  CData*        idTWeight   = NULL;                                             /* Store weight of Transition        */
  FST_TID_TYPE* iMySearch   = CFst_STI_Init(itFvr,0,FSTI_SORTTER);              /* find trans. of act. node          */
  FST_TID_TYPE* iMySearch2  = CFst_STI_Init(itFvr,0,FSTI_SORTINI);              /* find trans. of act. child(2.level)*/
  BYTE*         lpTrans     = NULL;                                             /* transition of 1. level iteration  */
  BYTE*         lpTrans2    = NULL;                                             /* transition of 2. level iteration  */
  FST_ITYPE     nAux;                                                           /* Auxiliary value                   */
  FST_ITYPE     nAux2;                                                          /* Auxiliary value2                  */
  FST_ITYPE     nAux3;                                                          /* Auxiliary value3                  */
  FST_ITYPE     nIsym       = NULL;                                             /* Input Symbol for store new values */
  FST_ITYPE     nRec;                                                           /* Number of transitions             */
  FST_ITYPE     nSym;                                                           /* Number of symbols                 */
  FST_ITYPE     nTransItDst;                                                    /* Transition iterator for new Fst   */
  FST_ITYPE     nTer;                                                           /* Auxiliary value for permutation   */
  FST_ITYPE     nU;                                                             /* Unit index in target              */
  BOOL          permutCheck = TRUE;                                             /* Checking the combination of symb. */
  BOOL          isNotLeaf = FALSE;                                              /* Different mode to store           */
  FST_ITYPE*    p  = NULL; p  = (FST_ITYPE*) dlp_calloc(1,sizeof(FST_ITYPE));   /* Dyn. array for permutation        */

  /* Initialization */                                                          /* --------------------------------- */
  if (p == NULL){                                                               /* Check dynamic Array               */
   printf("Kein virtueller RAM mehr vorhanden ... !"); return ERR_MEM;}         /*                                   */
  CHECK_THIS_RV(NOT_EXEC);                                                      /* Check this instance               */
  if (itDst==NULL)                                                              /* Output not correct instance       */
    FVRT_EXCEPTION(ERR_NULLINST,"itDst is NULL",0,0);                           /*   Error message and exit          */
  if (itFvr==NULL)                                                              /* Input empty                       */
    return OK(IERROR(_this,ERR_NULLINST,"itFvr",0,0));                          /*   Error message and exit          */
  if (!CFvrtools_IsFvr(_this,0,itFvr))                                          /* Check Input is an FVR?            */
    FVRT_EXCEPTION(FVRT_NOTFVR,BASEINST(itFvr)->m_lpInstanceName,0,0);          /*   Error message and exit          */
  if (itDst==itFvr)                                                             /* Input and Output are not same     */
    FVRT_EXCEPTION(ERR_GENERIC,"Source and target must not be identical",0,0);  /*   Error message and exit          */
  ICREATEEX(CData,idRank,"CFvrtools_Synthesize~idRank",NULL);                   /*                                   */
  ICREATEEX(CData,idSym,"CFvrtools_Synthesize~idSym",NULL);                     /*                                   */
  ICREATEEX(CData,idSymRef,"CFvrtools_Synthesize~idSymRef",NULL);               /*                                   */
  ICREATEEX(CData,idSymList,"CFvrtools_Synthesize~idSymList",NULL);             /*                                   */
  ICREATEEX(CData,idStArray,"CFvrtools_Synthesize~idStArray",NULL);             /*                                   */
  ICREATEEX(CData,idStList,"CFvrtools_Synthesize~idStList",NULL);               /*                                   */
  ICREATEEX(CData,idTWeight,"CFvrtools_Synthesize~idTWeight",NULL);             /*                                   */
  CFst_Reset(BASEINST(itDst),TRUE);                                             /* Reset target                      */
  CFst_Rank(itFvr, 0, idRank);                                                  /* Get rank to discern by equal symb.*/
  ISETOPTION(itDst,"/lsr"); ISETOPTION(itDst,"/fst");                           /* Set some options                  */
  nU = CFst_Addunit(itDst,"");                                                  /* Add a unit to the FVR sequence    */
  IRESETOPTIONS(itDst);                                                         /* Clear options                     */
  nCTIS = CData_FindComp(AS(CData,itDst->td),NC_TD_TIS);                        /* Get comp. index of input symbol   */
  nCTOS = CData_FindComp(AS(CData, itDst->td),NC_TD_TOS);                       /* Get comp. index of output symbol  */

  /* Add first 2 states and 1 transition as base */                             /* --------------------------------- */
  if ((nU=CFst_Addstates(itDst,0,2,0))<0)                                       /* Add first 2 states in target      */
    return IERROR(itDst,FST_INTERNAL,__FILE__,__LINE__,"");                     /* Check added state                 */
  CFst_AddtransIam(itDst, 0, nU, nU+1);                                         /* Add transition between both states*/
  CData_Dstore(AS(CData,itDst->td), 0, 0, nCTIS);                               /* Store 0 for first input symbol    */
  CData_Dstore(AS(CData,itDst->td), CData_Dfetch(idRank,1,0), 0, nCTOS);        /* save rank of trans. from source   */
  CData_AddComp(idSym,"Sym",T_LONG);    CData_Allocate(idSym,1);                /* Allocate memory for symbol(s)     */
  CData_AddComp(idSymRef,"Ref",T_LONG); CData_Allocate(idSymRef,1);             /* Allocate memory for Ref to symbol */
  CData_AddComp(idStArray,"Ref",T_LONG); CData_Allocate(idStArray,1);           /* Allocate memory for Ref to symbol */
  CData_AddComp(idTWeight,"~LSR",T_FLOAT); CData_Allocate(idTWeight,UD_XS(itFvr,0));/* Allocate memory for weight    */

  /* Start iteration over all states and collect needed information */          /* --------------------------------- */
  while (nMyIniState<=UD_XT(itFvr,0))                                           /* loop over all States of FVR       */
  {                                                                             /*                                   */
    iMySearch = CFst_STI_Init(itFvr,0,FSTI_SORTTER);                            /*                                   */
    if( (lpTrans=CFst_STI_TtoS(iMySearch, nMyIniState, NULL)) != NULL ){        /* Get input transition              */
      nTis = *CFst_STI_TTis(iMySearch, lpTrans);                                /* Get Tis to compare with child     */
      CData_Dstore(idTWeight,*CFst_STI_TW(iMySearch, lpTrans),nMyIniState,0);   /* Store weight of transition        */
    }                                                                           /*                                   */
    CData_Reallocate(idSym,0);                                                  /* Reset value for next permutation  */
    CData_Reallocate(idSymRef,0);                                               /* Reset value for next permutation  */
    CData_Reallocate(idStArray,0);                                              /* Reset value for next permutation  */

    nAux = 0; lpTrans = NULL; iMySearch = CFst_STI_Init(itFvr,0,FSTI_SORTINI);  /* --------------------------------- */
    while ((lpTrans=CFst_STI_TfromS(iMySearch,nMyIniState,lpTrans))!=NULL)      /* loop over trans. from act. node   */
    {                                                                           /*  to take in sep. list for permut  */
      FST_ITYPE nTerS = *CFst_STI_TTer(iMySearch,lpTrans);                      /* Get terminal state                */
      rank = CData_Dfetch(idRank,nTerS,0);                                      /*  ... to get rank of state         */
      FST_ITYPE nISym = *CFst_STI_TTis(iMySearch,lpTrans);                      /* Get input symbol                  */

      if ( nTis != nISym ){                                                     /* Is child and parent different     */
                                                                                /*  Take ...                         */
        CData_Reallocate(idSym,nAux+1);                                         /* reserve new memory for next value */
        CData_Dstore(idSym,nISym,nAux,0);                                       /*    symbol for Permutation         */
        CData_Reallocate(idSymRef,nAux+1);                                      /* reserve new memory for next Refv. */
        CData_Dstore(idSymRef,rank,nAux,0);                                     /*    ref. value to check sequence   */
        CData_Reallocate(idStArray,nAux+1);                                     /* reserve new memory for next Refv. */
        CData_Dstore(idStArray,nTerS,nAux++,0);                                 /*    ref. value to check save point */
        lpTrans2 = NULL; nAux2 = nTerS;                                         /* --------------------------------- */
        while((lpTrans2=CFst_STI_TfromS(iMySearch2,nAux2,lpTrans2))!=NULL)      /* Check child with same input sym.  */
        {                                                                       /* Dont if act. par. & child same... */
                                                                                /* ... notice before                 */
          FST_ITYPE nTerS2 = *CFst_STI_TTer(iMySearch2,lpTrans2);               /* Get terminal state of act. trans. */
          rank = CData_Dfetch(idRank,nTerS2,0);                                 /*  ... to get rank of state         */
          FST_ITYPE nISym2 = *CFst_STI_TTis(iMySearch2,lpTrans2);               /* Get input symbol of act. trans.   */
          if ( nISym2 == nISym ){                                               /* Take double input for permutation */
                                                                                /* Take ...                          */
            CData_Reallocate(idSym,nAux+1);                                     /* reserve new memory for next value */
            CData_Dstore(idSym,nISym,nAux,0);                                   /*    symbol for Permutation         */
            CData_Reallocate(idSymRef,nAux+1);                                  /* reserve new memory for next Refv. */
            CData_Dstore(idSymRef,rank,nAux,0);                                 /*    ref. value to check sequence   */
            CData_Reallocate(idStArray,nAux+1);                                 /* reserve new memory for next Refv. */
            CData_Dstore(idStArray,nTerS2,nAux++,0);                            /*    ref. value to check save point */
            lpTrans2 = NULL;                                                    /* reset value for next child        */
            nAux2 = nTerS2;                                                     /* set child as next node            */
          }
        }
      }/*if ( nTis != nISym )*/
    }/*while ((lpTrans=CFst_STI_TfromS(iMySearch,nMyIniState,lpTrans))!=NULL)*/

    /* Start permutation only when is more than one element */                  /* --------------------------------- */
    if (nAux > 0){                                                              /* start only if more than one symb. */
      p = (FST_ITYPE*) dlp_realloc(p, nAux, sizeof(FST_ITYPE));                 /* Auxiliary value for permutation   */
      nRecIdSym = CData_GetNRecs(idSym);                                        /* Get NRec of idSym                 */
      for(nAux = 0; nAux < nRecIdSym; nAux++) p[nAux]=nAux;                     /* fill aux. with series of numbers  */
      CData_Join(idSymList,idSym);                                              /* Save first sym. constellation     */
      CData_Join(idStList,idStArray);
      nAux = 1;                                                                 /* Iterator over symbols             */
      while (nAux < nRecIdSym){                                                 /* Start permutation algorithm       */
        p[nAux]--; nAux2 = (nAux % 2)*p[nAux];                                  /* Auxiliary values for permutation  */
        nAux3 = CData_Dfetch(idSym,nAux,0);                                     /* Swap values... Tis,               */
        CData_Dstore(idSym,CData_Dfetch(idSym,nAux2,0),nAux,0);                 /*                                   */
        CData_Dstore(idSym,nAux3,nAux2,0);                                      /*                                   */
        nAux3 = CData_Dfetch(idSymRef,nAux,0);                                  /*    ...Rank(reference),            */
        CData_Dstore(idSymRef,CData_Dfetch(idSymRef,nAux2,0),nAux,0);           /*                                   */
        CData_Dstore(idSymRef,nAux3,nAux2,0);                                   /*                                   */
        nAux3 = CData_Dfetch(idStArray,nAux,0);                                 /*    ...Ter(reference)              */
        CData_Dstore(idStArray,CData_Dfetch(idStArray,nAux2,0),nAux,0);         /*                                   */
        CData_Dstore(idStArray,nAux3,nAux2,0);                                  /*                                   */
        permutCheck = TRUE;                                                     /* Reset bool for permutation check  */
        for (nAux = 0; nAux < nRecIdSym; nAux++){                               /* Check actually permutation is ok  */
          for(nAux2 = 0; nAux2+nAux+1 < nRecIdSym; nAux2++){                    /*                                   */
            if(CData_Dfetch(idSym,nAux,0)==CData_Dfetch(idSym,nAux2+nAux+1,0)   /* Symbol1 == Symbol2 &&...          */
              && CData_Dfetch(idSymRef,nAux,0)>CData_Dfetch(idSymRef,nAux+nAux2+1,0))/*       ...Rank1 > Rank2       */
              permutCheck = FALSE;                                              /*                                   */
          }                                                                     /*                                   */
        }                                                                       /*                                   */
        for(nAux = 1; p[nAux] == 0; nAux++) p[nAux] = nAux;                     /* Reset aux. value for next permut. */
        if (permutCheck){                                                       /* Check is permutation ok?...       */
          CData_Join(idSymList,idSym);                                          /* ... than store the combination    */
          CData_Join(idStList,idStArray);                                       /* ... and combination of ref. value */
        }
      } /* End while (nAux < nRecIdSym) */

      /* Start to save in new FST */                                            /* --------------------------------- */
      nRec = CData_GetNRecs(AS(CData,itDst->td));                               /* Get number of already exists trans*/
      for(nTransItDst = 0; nTransItDst < nRec; nTransItDst++){                  /* Iterate over this transistions    */
        isNotLeaf = FALSE;                                                      /* Reset bool                        */
        if((INT32)CData_Dfetch(AS(CData,itDst->td),nTransItDst,2) == nTis       /* Check input symbol is same        */
          && (FST_ITYPE)CData_Dfetch(AS(CData,itDst->td),nTransItDst,nCTOS) == (nMyIniState)){/* Check rank is same  */
          nTer = (INT32)CData_Dfetch(AS(CData,itDst->td),nTransItDst,0);        /* Get terminal state of transition  */
          nComp = CData_GetNComps(idSymList);                                   /* Get number of possible permut.    */
          for(nAux2 = 0; nAux2 < nComp; nAux2++){                               /* ...loop over this to store permut.*/
            if(nAux2==0){                                                       /* First iteration, check is it leaf */
              for(nAux3=0; nAux3<nRec; nAux3++){                                /*    ...loop over tran.table        */
                if(nTer == (INT32)CData_Dfetch(AS(CData,itDst->td),nAux3,1)){   /* is nTer IniState -> its not a leaf*/
                  nTer = (INT32)CData_Dfetch(AS(CData,itDst->td),nAux3,0);      /*    Get next terminal State        */
                  nIsym = (INT32)CData_Dfetch(AS(CData,itDst->td),nAux3,nCTIS); /*    Get input symbol               */
                  nAux  = (INT32)CData_Dfetch(AS(CData,itDst->td),nAux3,nCTOS); /*    Get output symbol              */
                  isNotLeaf = TRUE;                                             /*   Set bool to store values correct*/
                  break;                                                        /* there are no more terminal States */
                }
              }
            }
            nSym = CData_GetNRecs(idSymList);                                   /* Number of symbols                 */
            if ((nU=CFst_Addstates(itDst,0,nSym,0))<0)                          /* Add states and get first IniTer   */
              return IERROR(itDst,FST_INTERNAL,__FILE__,__LINE__,"");           /* ...Error if not correct added     */
            if(nAux2 == 0 && isNotLeaf)                                         /* is not leaf than change first tran*/
              CData_Dstore(AS(CData,itDst->td),nU+nSym-1, nAux3,1);             /* ...open chain and add already stat*/
            for(nAux3 = 0; nAux3 < nSym; nAux3++){                              /* Add new transitions               */
              if (nAux3 == 0)                                                   /* first tran to already exist state */
                CFst_AddtransIam(itDst, 0, (INT32)CData_Dfetch(AS(CData,itDst->td),nTransItDst,0), nU);/*Add trans.  */
              else{                                                             /* last transition, nTer is not leaf */
                CFst_AddtransIam(itDst, 0, nU, nU+1); nU++;                     /* Add transition                    */
              }                                                                 /* Store Value...                    */
              CData_Dstore(AS(CData,itDst->td),CData_Dfetch(idSymList,nAux3,nAux2),CData_GetNRecs(AS(CData,itDst->td))-1,nCTIS);
              CData_Dstore(AS(CData,itDst->td),CData_Dfetch(idStList,nAux3,nAux2),CData_GetNRecs(AS(CData,itDst->td))-1,nCTOS);
            }
            if(isNotLeaf && nAux2 > 0){
              CFst_AddtransIam(itDst, 0, nU, nTer);
              CData_Dstore(AS(CData,itDst->td),nIsym,CData_GetNRecs(AS(CData,itDst->td))-1,nCTIS);
              CData_Dstore(AS(CData,itDst->td),nAux, CData_GetNRecs(AS(CData,itDst->td))-1,nCTOS);
            }
          } /* for(nAux2 = 0; nAux2 < nComp; nAux2++){ */                       /* End of possible permutations      */
        } /* if((INT32)CData_Dfetch(AS(CData,itDst->td)... */                   /* End of check input symbol and rank*/
      } /* for(nTransItDst = 0; nTransItDst < nRec; nTransItDst++){ */          /* End of loop over transitions      */
    } /* End if(nAux > 0){ */                                                   /* End of start if more than one symb*/

    CData_Reallocate(idSymList,0);                                              /* Reset list of input symbols       */
    CData_Reallocate(idStList,0);                                               /* Reset list of original state num. */
    p = (FST_ITYPE*) dlp_realloc(p, 1, sizeof(FST_ITYPE));                      /* Reset Auxiliary value for permut. */
    nMyIniState ++;                                                             /* Increment IniState for next state */
    CFst_Tree(itDst, itDst, 0);                                                 /* Make tree of added states         */
  }/* End while(nMyIniState<=UD_XT(itFvr,0)) */                                 /* End of loop over states of IniFst */

  /* Add weight and finite state */
  nAux = UD_XS(itDst,0);                                                        /* Auxiliary values to set final st. */
  iMySearch   = CFst_STI_Init(itDst,0,FSTI_SORTINI);                            /* Set search to find finite State   */
  for(nMyIniState=0; nMyIniState < nAux; nMyIniState++){                        /* Iterate over all new states       */
    if((lpTrans=CFst_STI_TfromS(iMySearch, nMyIniState, NULL)) == NULL)         /* Check state has not transition... */
      CData_Dstore(AS(CData,itDst->sd),1,nMyIniState,0); }                      /* ... yes? Than take it to finite   */
                                                                                /* --------------------------------- */
  iMySearch2  = CFst_STI_Init(itDst,0,FSTI_SORTTER);                            /* Set search to find weight of trans*/
  for(nMyIniState=0; nMyIniState < nAux; nMyIniState++){                        /* Iterate over all new states       */
    if( (lpTrans2=CFst_STI_TtoS(iMySearch2, nMyIniState, NULL)) != NULL )       /* Check state has transition...     */
      CData_Dstore(AS(CData,itDst->td),CData_Dfetch(idTWeight,*CFst_STI_TTos(iMySearch2, lpTrans2),0),nMyIniState-1,4);
  }                                                                             /* Store weight                      */

  itDst->is = itFvr->is;                                                        /* Transfer the symbol list, its same*/
  if(CFvrtools_IsFvr(_this, 0, itDst))
    nRet = O_K;

L_EXCEPTION:                                                                    /*                                   */
  CFst_STI_Done(iMySearch);                                                     /*                                   */
  CFst_STI_Done(iMySearch2);                                                    /*                                   */
  IDESTROY(idRank);                                                             /*                                   */
  IDESTROY(idSym);                                                              /*                                   */
  IDESTROY(idSymRef);                                                           /*                                   */
  IDESTROY(idStArray);                                                          /*                                   */
  IDESTROY(idStList);                                                           /*                                   */
  IDESTROY(idSymList);                                                          /*                                   */
  dlp_free(p);                                                                  /*                                   */
  return nRet;                                                                  /*                                   */
}

/* EOF */
