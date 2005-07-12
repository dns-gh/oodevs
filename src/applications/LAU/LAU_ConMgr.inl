//*****************************************************************************
//
// Name     : LAU_ConMgr.inl
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 5 $
// $Workfile: LAU_ConMgr.inl $
//
//*****************************************************************************

//#include "LAU_ConMgr.h" // Pour pouvoir le compiler tout seul...


//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr::IsLinkToMOS
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
bool32 LAU_ConMgr::IsLinkToMOS( DIN::DIN_Link& link )
{
    return mapMos_.end() != mapMos_.find( link.GetStationID() );
}


//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr::IsLinkToSIM
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
bool32 LAU_ConMgr::IsLinkToSIM( DIN::DIN_Link& link )
{
    return mapSim_.end() != mapSim_.find( link.GetStationID() );
}


//-----------------------------------------------------------------------------
// Name: LAU_ConMgr::IsSIM_Connected
// Return : cette sim est t'elle connecté?
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
bool32 LAU_ConMgr::IsSIM_Connected( LAU_SIM& simToFind )
{
    IT_ID_SIM_PtrMap itMapSim, itMapSimEnd;
    for( itMapSim = mapSim_.begin(), itMapSimEnd = mapSim_.end(); itMapSim != itMapSimEnd; ++itMapSim )
    {
         if( (*itMapSim).second == &simToFind )
         {
            return true;
         }
    }

    return false;
}