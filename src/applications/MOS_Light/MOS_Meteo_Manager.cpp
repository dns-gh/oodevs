//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_Manager.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_Meteo_Manager.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_Meteo_Manager.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo_Manager.inl"
#endif

#include "MOS_Meteo_Local.h"
#include "MOS_Meteo_GUI.h"

MT_IdentifierManager MOS_Meteo_Manager::idManager_;

//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager constructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
MOS_Meteo_Manager::MOS_Meteo_Manager()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager destructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
MOS_Meteo_Manager::~MOS_Meteo_Manager()
{
    MT_DELETEOWNED( localWeatherVector_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager::CreateWeatherGUI
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
QWidget* MOS_Meteo_Manager::CreateWeatherGUI( QWidget* pParent )
{
    MOS_Meteo_GUI* pGUI = new MOS_Meteo_GUI( pParent );
    pGUI->SetGlobalWeather( & globalWeather_ );
    return pGUI;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager::CreateLocalWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_Local& MOS_Meteo_Manager::CreateLocalWeather()
{
    MOS_Meteo_Local* pNew = new MOS_Meteo_Local( idManager_.GetFreeIdentifier() );
    localWeatherVector_.push_back( pNew );
    return *pNew;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager::DeleteLocalWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Manager::DeleteLocalWeather( MOS_Meteo_Local& localWeather )
{
    localWeather.SendDeleteMsg();
    idManager_.ReleaseIdentifier( localWeather.GetId() );
    IT_Meteo_Vector found = std::find( localWeatherVector_.begin(), localWeatherVector_.end(), & localWeather );
    if( found != localWeatherVector_.end() )
        localWeatherVector_.erase( found );
}

