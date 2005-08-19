//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Meteo_ABC.cpp $
// $Author: Ape $
// $Modtime: 29/11/04 10:55 $
// $Revision: 3 $
// $Workfile: MOS_Meteo_ABC.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_Meteo_ABC.h"

#include "MOS_ASN_Messages.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo_ABC.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC constructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
MOS_Meteo_ABC::MOS_Meteo_ABC()
    : nTemperature_( 0 )
    , nWindSpeed_( 0 )
    , nWindDirection_( 0 )
    , nCloudCoverFloor_( 0 )
    , nCloudCoverCeiling_( 0 )
    , nCloudCoverAverageDensity_( 0 )
    , nWeatherType_( eWeather_Clear )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC destructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
MOS_Meteo_ABC::~MOS_Meteo_ABC()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::BuildAsnMsg
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
void MOS_Meteo_ABC::BuildAsnMsg( ASN1T_MeteoAttributs& asnMsg )
{
    asnMsg.temperature = nTemperature_;

    asnMsg.vitesse_vent = nWindSpeed_;
    asnMsg.direction_vent = nWindDirection_;

    asnMsg.plancher_couverture_nuageuse = nCloudCoverFloor_;
    asnMsg.plafond_couverture_nuageuse = nCloudCoverCeiling_;
    asnMsg.densite_moyenne_couverture_nuageuse = nCloudCoverAverageDensity_;

    asnMsg.precipitation = (ASN1T_EnumPrecipitationType)(nWeatherType_);
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::SendUpdateMsg
// Created: AGN 03-08-06
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void MOS_Meteo_ABC::SendUpdateMsg()
{
    MOS_ASN_MsgCtrlMeteoGlobale asnMsg;
    BuildAsnMsg( asnMsg.GetAsnMsg().attributs );

    asnMsg.Send();

    MOS_App::GetApp().GetRawVisionData().UpdateGlobalMeteo( asnMsg.GetAsnMsg().attributs );
}

