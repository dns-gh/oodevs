// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamPoint.cpp $
// $Author: Ape $
// $Modtime: 27/08/04 17:15 $
// $Revision: 7 $
// $Workfile: MOS_ParamPoint.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamPoint.h"
#include "moc_MOS_ParamPoint.cpp"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ActionContext.h"

// -----------------------------------------------------------------------------
// Name: MOS_ParamPoint constructor
/** @param  asnListPoint 
    @param  pParent 
    @param  strName 
*/
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
MOS_ParamPoint::MOS_ParamPoint( ASN1T_Point& asnPoint, const std::string strLabel, const std::string strMenuText, QWidget* pParent )
    : QHBox         ( pParent )
    , MOS_Param_ABC ()
    , asnPoint_     ( asnPoint )
    , strMenuText_  ( strMenuText )
    , point_        ( 0.0, 0.0 )
{
    this->setSpacing( 5 );
    pLabel_ = new MT_ParameterLabel( strLabel.c_str(), false, this, "" );

    pPosLabel_ = new QLabel( "---", this );
    pPosLabel_->setMinimumWidth( 100 );
    pPosLabel_->setAlignment( Qt::AlignCenter );
    pPosLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPoint destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
MOS_ParamPoint::~MOS_ParamPoint()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPoint::CheckValidity
/** @return 
*/
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
bool MOS_ParamPoint::CheckValidity()
{
    if( pPosLabel_->text() == "---" )
    {
        pLabel_->TurnRed( 3000 );
        return false;
    }

    return true;
}

// -----------------------------------------------------------------------------
// Name: MOS_ParamPoint::WriteMsg
/** @param  sParam 
*/
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
void MOS_ParamPoint::WriteMsg( std::stringstream& strMsg )
{
    strMsg << pLabel_->text().latin1() << ": ";

    asnPoint_.type                  = EnumTypeLocalisation::point;
    asnPoint_.vecteur_point.n       = 1;
    asnPoint_.vecteur_point.elem    = &asnPos_;
    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( point_, strPos );
    asnPos_ = strPos.c_str();
    strMsg << strPos;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPoint::FillRemotePopupMenu
/** @param  popupMenu 
    @param  pAgent 
    @param  pPoint 
*/
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void MOS_ParamPoint::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( context.pPoint_ == 0 )
        return;

    const MT_Rect& rectWorld = MOS_App::GetApp().GetWorld().GetRect();
    if( ! rectWorld.IsInside( *(context.pPoint_) ) )
        return;

    popupMenuPoint_ = *(context.pPoint_);

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuPoint() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPoint::AcceptPopupMenuPoint
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void MOS_ParamPoint::AcceptPopupMenuPoint()
{
    point_ = popupMenuPoint_;
    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( point_, strPos );
    pPosLabel_->setText( strPos.c_str() );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPoint::Clear
// Created: APE 2004-08-27
// -----------------------------------------------------------------------------
void MOS_ParamPoint::Clear()
{
    pPosLabel_->setText( "---" );
}
