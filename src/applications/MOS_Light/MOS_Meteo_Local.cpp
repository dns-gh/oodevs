//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_Local.cpp $
// $Author: Age $
// $Modtime: 7/02/05 15:44 $
// $Revision: 4 $
// $Workfile: MOS_Meteo_Local.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_Meteo_Local.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo_Local.inl"
#endif

#include "MOS_ASN_Messages.h"
#include "MOS_App.h"
#include "MOS_World.h"

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
    vUpperLeftCorner_ = MT_Vector2D( 0, 0 ); // $$$$ AGE 2005-02-07: 
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

    // mise à jour de la météo pour affichage des zones d'intervisibilité
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

    // mise à jour de la météo pour affichage des zones d'intervisibilité
    MOS_App::GetApp().GetRawVisionData().RegisterMeteoPatch( vUpperLeftCorner_, vLowerRightCorner_, 0 );
}



//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local::SetCorners
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Local::SetCorners( const MT_Vector2D& corner1, const MT_Vector2D& corner2 )
{
    if( corner1.rX_ < corner2.rX_ )
    {
        vUpperLeftCorner_.rX_ = corner1.rX_;
        vLowerRightCorner_.rX_ = corner2.rX_;
    }
    else
    {
        vUpperLeftCorner_.rX_ = corner1.rX_;
        vLowerRightCorner_.rX_ = corner2.rX_;
    }

    if( corner1.rY_ < corner2.rY_ )
    {
        vUpperLeftCorner_.rY_ = corner2.rY_;
        vLowerRightCorner_.rY_ = corner1.rY_;
    }
    else
    {
        vUpperLeftCorner_.rY_ = corner1.rY_;
        vLowerRightCorner_.rY_ = corner2.rY_;
    }
}
