//*****************************************************************************
//
// $Created: AGN 03-05-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 7 $
// $Workfile: MOS_EnemyInfo.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_EnemyInfo.h"

#ifndef MOS_USE_INLINE
#	include "MOS_EnemyInfo.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ASN_Messages.h"

//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo constructor
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
MOS_EnemyInfo::MOS_EnemyInfo( int nID )
: nId_( nID )
, nConfirmationLevel_( eConfirmationLevel_None )
, nHierarchicalLevel_( eHierarchicalLevel_None )
, nAttitude_( eAttitude_NotDefined )
, nSpeed_( 0 )
, nWeapon_             ( eUnitNatureWeapon_None )
, nCategory_           ( eUnitNatureCategory_None )
, nQualifier_          ( eUnitNatureQualifier_None )
, nSpecialization_     ( eUnitNatureSpecialization_None )
, nMobility_           ( eUnitNatureMobility_None )
, startDir_( MOS_App::GetApp().GetWorld().GetRect().GetCenter() )
, endDir_( MOS_App::GetApp().GetWorld().GetRect().GetCenter() )
, strName_( "Unité " + itostring( nId_ ) )
, nCamp_( 6 )
{
    pointList_.push_back( MOS_App::GetApp().GetWorld().GetRect().GetCenter() );
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo destructor
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
MOS_EnemyInfo::~MOS_EnemyInfo()
{
/*    MOS_ASN_MsgRensEnnemiDestruction asnMsg;
    asnMsg.GetAsnMsg().oid_renseignement = nId_;
    asnMsg.GetAsnMsg().oid_automates.n    = referencingAutomataSet_.size();
    asnMsg.GetAsnMsg().oid_automates.elem = new ASN1T_OID[ referencingAutomataSet_.size() ];

    int i = 0;
    for( MOS_Agent::IT_AgentConstPtrSet it = referencingAutomataSet_.begin(); it != referencingAutomataSet_.end(); ++it, ++i )
    {
        asnMsg.GetAsnMsg().oid_automates.elem[i] = (*it)->GetAgentID();
    }
    asnMsg.Send( (MIL_MOSContextID) this );
    delete[] asnMsg.GetAsnMsg().oid_automates.elem;*/
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo::Draw
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo::Draw()
{
    if( pointList_.empty() )
        return;

    IT_PointVector itPoint;

    GFX_Color colorButton( GFX_Tools::GetColorForTeam( nCamp_ ) );
    MT_Float rSizeBorder = 10.0;
    MT_Float rSizeButton = 150.;

    for( itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
    {

        MT_Rect rect( itPoint->rX_ - rSizeButton, itPoint->rY_ - rSizeButton, itPoint->rX_ + rSizeButton, itPoint->rY_ + rSizeButton );

        GFX_Tools::CreateGLButton( rect, colorButton, rSizeBorder );
    }

    MT_Float rSizeName = 500.;
    
    GFX_Color colorName( 0,255,255 );
    for ( itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
    {
        MT_Vector2D vPos( (*itPoint) );
        vPos.rX_ += rSizeButton / 2.;
        vPos.rY_ += rSizeButton / 2.;
        GFX_Tools::PrintWithShadows( vPos, rSizeName, 4., 8., colorName, false, strName_.c_str() );
    }

    if( pointList_.size() == 1 )
        return;

    GFX_Color color( 255,255,255 );

    MT_Vector2D lastPoint;
    MT_Vector2D firstPoint = pointList_.front();
    for( itPoint = pointList_.begin(); itPoint != pointList_.end() ; ++itPoint )
    {
        if( itPoint != pointList_.begin() )
        {
            GFX_Tools::CreateGLLine( lastPoint, (*itPoint), 2.0, color );
        }
        lastPoint = (*itPoint);
    }

    if( pointList_.size() > 2 )
        GFX_Tools::CreateGLLine( lastPoint, firstPoint, 2.0, color );    
}

