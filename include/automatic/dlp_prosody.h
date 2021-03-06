// dLabPro class CProsody (Prosody)
// - Header file
//
// AUTHOR : Hussein Hussein
// PACKAGE: dLabPro/classes
//
// This file was generated by dcg. DO NOT MODIFY! Modify prosody.def instead.
// 
// Copyright 2013 dLabPro contributors and others (see COPYRIGHT file) 
// - Chair of System Theory and Speech Technology, TU Dresden
// - Chair of Communications Engineering, BTU Cottbus
// 
// This file is part of dLabPro.
// 
// dLabPro is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
// 
// dLabPro is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with dLabPro. If not, see <http://www.gnu.org/licenses/>.


//{{CGEN_INCLUDE
#include "dlp_config.h"
#include "dlp_object.h"
#include "dlp_data.h"
//}}CGEN_END

//{{CGEN_ERRORS
#undef FALSE_PAUSE         
#undef NO_F0               
#undef NO_SRate            
#undef NO_PM               
#undef Div_Zero            
#define FALSE_PAUSE          -1001
#define NO_F0                -1002
#define NO_SRate             -1003
#define NO_PM                -1004
#define Div_Zero             -1005
//}}CGEN_END

// C/C++ language abstraction layer
#undef prosody_par
#define prosody_par CProsody

// dLabPro/C++ language abstraction layer
#undef Prosody
#define Prosody CProsody

//{{CGEN_DEFINE
//}}CGEN_DEFINE

#ifndef __PROSODY_H
#define __PROSODY_H

//{{CGEN_HEADERCODE

//}}CGEN_HEADERCODE

// Class CProsody

class CProsody : public CDlpObject 
{

typedef CDlpObject inherited;
typedef CProsody thisclass;

//{{CGEN_FRIENDS
//}}CGEN_FRIENDS
public:
	CProsody(const char* lpInstanceName, BOOL bCallVirtual = 1);
	virtual ~CProsody();

// Virtual and static function overrides
public:
	virtual INT16 AutoRegisterWords();
	virtual INT16 Init(BOOL bCallVirtual = 0);
	virtual INT16 Reset(BOOL bResetMembers = 1);
	virtual INT16 Serialize(CDN3Stream* lpDest);
	virtual INT16 SerializeXml(CXmlStream* lpDest);
	virtual INT16 Deserialize(CDN3Stream* lpSrc);
	virtual INT16 DeserializeXml(CXmlStream* lpSrc);
	virtual INT16 Copy(CDlpObject* iSrc);
	virtual INT16 ClassProc();
	static  INT16 InstallProc(void* lpItp);
	static  CProsody* CreateInstance(const char* lpName);
	static  INT16 GetClassInfo(SWord* lpClassWord);
	virtual INT16 GetInstanceInfo(SWord* lpClassWord);
	virtual BOOL  IsKindOf(const char* lpClassName);
	virtual INT16 ResetAllOptions(BOOL bInit = 0);

// Primary method invocation functions            
// DO NOT CALL THESE FUNCTIONS FROM C SCOPE.      
// THEY MAY INTERFERE WITH THE INTERPRETER SESSION
#ifndef __NOITP
public:
//{{CGEN_PMIC
	INT16 OnAddRms();
	INT16 OnEnergyContour();
	INT16 OnPauseDetect();
	INT16 OnPmFo();
//}}CGEN_PMIC
#endif // #ifndef __NOITP

// Secondary method invocation functions
public:
//{{CGEN_SMIC
//}}CGEN_SMIC

// Option changed callback functions
public:
//{{CGEN_OCCF
//}}CGEN_OCCF

// Field changed callback functions
public:
//{{CGEN_FCCF
//}}CGEN_FCCF

// Scanned member functions
//{{CGEN_EXPORT

// Taken from 'pause.cpp'
	private: INT16 PauseDetect(data* dSignal, data* dPause);
	private: FLOAT64 MaximumValue(FLOAT64* a, INT32 n);
	private: FLOAT64 MinimumValue(FLOAT64* a, INT32 n);
	private: INT16 SignalNorming(FLOAT64* a, INT32 n, FLOAT64* b);
	private: INT16 MovingAverage(FLOAT64* a, INT32 n, INT32 m, FLOAT64* b);
	private: INT16 ShortTimeEnergy(FLOAT64* a, INT32 n, INT32 m, FLOAT64* b);
	private: FLOAT64 MeanValue(FLOAT64* a, INT32 n);

// Taken from 'pm2f0.cpp'
	private: INT16 PmFo(data* dPM, INT32 nSrate, INT32 nSrateF0, data* dF0);

// Taken from 'energy.cpp'
	private: INT16 EnergyContour(data* dSignal, data* dEnergy);

// Taken from 'add_rms.cpp'
	private: INT16 AddRms(data* dSignal, data* dF0, INT32 nSrate, INT32 nSrateF0, data* dF0rms);
//}}CGEN_EXPORT

// Member variables
public:

//{{CGEN_FIELDS
//}}CGEN_FIELDS

//{{CGEN_OPTIONS
//}}CGEN_OPTIONS
}

;

// Scanned C (member) functions
//{{CGEN_CEXPORT

// Taken from 'pause.cpp'

// Taken from 'pm2f0.cpp'

// Taken from 'energy.cpp'

// Taken from 'add_rms.cpp'
//}}CGEN_CEXPORT

#endif //#ifndef __PROSODY_H


// EOF
