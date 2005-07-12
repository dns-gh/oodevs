//*****************************************************************************
//
// $Created: FBD 03-08-25 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_HistoryMission.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_HistoryMission.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_HistoryMission.h"

#ifndef MOS_USE_INLINE
#	include "MOS_HistoryMission.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission constructor
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
MOS_HistoryMission::MOS_HistoryMission( uint nTimeStart, uint nTimeEnd, std::string& sName, std::string& sParam, E_MissionID nMission )
:   sName_( sName )
,   nTimeStart_( nTimeStart )
,   nTimeEnd_( nTimeEnd )
,   nColor_( nMission * 16 )
,   sParam_( sParam )
,   rect_( 0,0, -1,-1 )
,   nMission_( nMission )
,   nState_( eMissionSended )
{
}



//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission destructor
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
MOS_HistoryMission::~MOS_HistoryMission()
{
    
}

