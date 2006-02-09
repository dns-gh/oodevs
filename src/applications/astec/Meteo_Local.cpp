//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo_Local.cpp $
// $Author: Ape $
// $Modtime: 20/07/04 17:47 $
// $Revision: 2 $
// $Workfile: Meteo_Local.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "Meteo_Local.h"

#include "ASN_Messages.h"
#include "App.h"
#include "World.h"

//-----------------------------------------------------------------------------
// Name: Meteo_Local constructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
Meteo_Local::Meteo_Local( int nId )
    : Meteo_ABC()
    , vUpperLeftCorner_()
    , vLowerRightCorner_()
    , nId_( nId )
{
    vUpperLeftCorner_ = App::GetApp().GetWorld().GetRect().GetCenter();
    vLowerRightCorner_ = vUpperLeftCorner_;
    vLowerRightCorner_.rX_ += 100;
    vLowerRightCorner_.rY_ -= 100;
}


//-----------------------------------------------------------------------------
// Name: Meteo_Local destructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
Meteo_Local::~Meteo_Local()
{
}


//-----------------------------------------------------------------------------
// Name: Meteo_Local::SendUpdateMsg
// Created: AGN 03-08-06
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void Meteo_Local::SendUpdateMsg()
{
    ASN_MsgCtrlMeteoLocale asnMsg;

    asnMsg.GetAsnMsg().meteo.t = T_MsgCtrlMeteoLocale_meteo_attributs;
    asnMsg.GetAsnMsg().meteo.u.attributs = new ASN1T_MeteoAttributs();

    BuildAsnMsg( *asnMsg.GetAsnMsg().meteo.u.attributs );

    std::string strMGRS;
    App::GetApp().GetWorld().SimToMosMgrsCoord( vUpperLeftCorner_, strMGRS );
    asnMsg.GetAsnMsg().rect_point_haut_gauche = strMGRS.c_str();

    App::GetApp().GetWorld().SimToMosMgrsCoord( vLowerRightCorner_, strMGRS );
    asnMsg.GetAsnMsg().rect_point_bas_droite = strMGRS.c_str();

    asnMsg.Send();

    // mise à jour de la météo pour affichage des zones d'intervisibilité
    App::GetApp().GetRawVisionData().RegisterMeteoPatch( vUpperLeftCorner_, vLowerRightCorner_, Meteo::Create( *asnMsg.GetAsnMsg().meteo.u.attributs ) );

    delete asnMsg.GetAsnMsg().meteo.u.attributs;
}


//-----------------------------------------------------------------------------
// Name: Meteo_Local::SendDeleteMsg
// Created: AGN 03-08-07
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void Meteo_Local::SendDeleteMsg() const
{
    ASN_MsgCtrlMeteoLocale asnMsg;
 
    asnMsg.GetAsnMsg().meteo.t = T_MsgCtrlMeteoLocale_meteo_globale;
    
    std::string strMGRS;
    App::GetApp().GetWorld().SimToMosMgrsCoord( vUpperLeftCorner_, strMGRS );
    asnMsg.GetAsnMsg().rect_point_haut_gauche = strMGRS.c_str();

    App::GetApp().GetWorld().SimToMosMgrsCoord( vLowerRightCorner_, strMGRS );
    asnMsg.GetAsnMsg().rect_point_bas_droite = strMGRS.c_str();

    asnMsg.Send();

    // mise à jour de la météo pour affichage des zones d'intervisibilité
    App::GetApp().GetRawVisionData().RegisterMeteoPatch( vUpperLeftCorner_, vLowerRightCorner_, 0 );
}


//-----------------------------------------------------------------------------
// Name: Meteo_Local::SetCorners
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void Meteo_Local::SetCorners( const MT_Vector2D& corner1, const MT_Vector2D& corner2 )
{
    vUpperLeftCorner_.rX_ = std::min( corner1.rX_, corner2.rX_ );
    vLowerRightCorner_.rX_ = std::max( corner1.rX_, corner2.rX_ );
    vUpperLeftCorner_.rY_ = std::max( corner1.rY_, corner2.rY_ );
    vLowerRightCorner_.rY_ = std::min( corner1.rY_, corner2.rY_ );
}
