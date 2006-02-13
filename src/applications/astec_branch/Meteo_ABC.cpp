//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo_ABC.cpp $
// $Author: Ape $
// $Modtime: 29/11/04 10:55 $
// $Revision: 3 $
// $Workfile: Meteo_ABC.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "Meteo_ABC.h"

#include "ASN_Messages.h"

//-----------------------------------------------------------------------------
// Name: Meteo_ABC constructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
Meteo_ABC::Meteo_ABC()
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
// Name: Meteo_ABC destructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
Meteo_ABC::~Meteo_ABC()
{
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::BuildAsnMsg
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
void Meteo_ABC::BuildAsnMsg( ASN1T_MeteoAttributs& asnMsg )
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
// Name: Meteo_ABC::SendUpdateMsg
// Created: AGN 03-08-06
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void Meteo_ABC::SendUpdateMsg()
{
    ASN_MsgCtrlMeteoGlobale asnMsg;
    BuildAsnMsg( asnMsg.GetAsnMsg().attributs );

    asnMsg.Send();

    App::GetApp().GetRawVisionData().UpdateGlobalMeteo( asnMsg.GetAsnMsg().attributs );
}

