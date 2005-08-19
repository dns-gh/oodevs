//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Meteo_Local.cpp $
// $Author: Ape $
// $Modtime: 20/07/04 17:47 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_Local.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_Meteo_Local.h"

#include "MOS_ASN_Messages.h"
#include "MOS_App.h"
#include "MOS_World.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo_Local.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local constructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
MOS_Meteo_Local::MOS_Meteo_Local( int nId )
    : MOS_Meteo_ABC()
    , vUpperLeftCorner_()
    , vLowerRightCorner_()
    , nId_( nId )
{
    vUpperLeftCorner_ = MOS_App::GetApp().GetWorld().GetRect().GetCenter();
    vLowerRightCorner_ = vUpperLeftCorner_;
    vLowerRightCorner_.rX_ += 100;
    vLowerRightCorner_.rY_ -= 100;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local destructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
MOS_Meteo_Local::~MOS_Meteo_Local()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local::SendUpdateMsg
// Created: AGN 03-08-06
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void MOS_Meteo_Local::SendUpdateMsg()
{
    MOS_ASN_MsgCtrlMeteoLocale asnMsg;

    asnMsg.GetAsnMsg().meteo.t = T_MsgCtrlMeteoLocale_meteo_attributs;
    asnMsg.GetAsnMsg().meteo.u.attributs = new ASN1T_MeteoAttributs();

    BuildAsnMsg( *asnMsg.GetAsnMsg().meteo.u.attributs );

    std::string strMGRS;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vUpperLeftCorner_, strMGRS );
    asnMsg.GetAsnMsg().rect_point_haut_gauche = strMGRS.c_str();

    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vLowerRightCorner_, strMGRS );
    asnMsg.GetAsnMsg().rect_point_bas_droite = strMGRS.c_str();

    asnMsg.Send();

    // mise � jour de la m�t�o pour affichage des zones d'intervisibilit�
    MOS_App::GetApp().GetRawVisionData().RegisterMeteoPatch( vUpperLeftCorner_, vLowerRightCorner_, MOS_Meteo::Create( *asnMsg.GetAsnMsg().meteo.u.attributs ) );

    delete asnMsg.GetAsnMsg().meteo.u.attributs;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local::SendDeleteMsg
// Created: AGN 03-08-07
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void MOS_Meteo_Local::SendDeleteMsg() const
{
    MOS_ASN_MsgCtrlMeteoLocale asnMsg;
 
    asnMsg.GetAsnMsg().meteo.t = T_MsgCtrlMeteoLocale_meteo_globale;
    
    std::string strMGRS;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vUpperLeftCorner_, strMGRS );
    asnMsg.GetAsnMsg().rect_point_haut_gauche = strMGRS.c_str();

    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vLowerRightCorner_, strMGRS );
    asnMsg.GetAsnMsg().rect_point_bas_droite = strMGRS.c_str();

    asnMsg.Send();

    // mise � jour de la m�t�o pour affichage des zones d'intervisibilit�
    MOS_App::GetApp().GetRawVisionData().RegisterMeteoPatch( vUpperLeftCorner_, vLowerRightCorner_, 0 );
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local::SetCorners
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Local::SetCorners( const MT_Vector2D& corner1, const MT_Vector2D& corner2 )
{
    vUpperLeftCorner_.rX_ = min( corner1.rX_, corner2.rX_ );
    vLowerRightCorner_.rX_ = max( corner1.rX_, corner2.rX_ );
    vUpperLeftCorner_.rY_ = max( corner1.rY_, corner2.rY_ );
    vLowerRightCorner_.rY_ = min( corner1.rY_, corner2.rY_ );
}
