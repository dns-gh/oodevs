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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamPoint.cpp $
// $Author: Ape $
// $Modtime: 27/08/04 17:15 $
// $Revision: 7 $
// $Workfile: ParamPoint.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamPoint.h"
#include "moc_ParamPoint.cpp"

#include "App.h"
#include "World.h"
#include "ActionContext.h"

// -----------------------------------------------------------------------------
// Name: ParamPoint constructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
ParamPoint::ParamPoint( ASN1T_Point& asnPoint, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional )
    : QHBox         ( pParent )
    , Param_ABC ( bOptional )
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
// Name: ParamPoint destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
ParamPoint::~ParamPoint()
{
}


// -----------------------------------------------------------------------------
// Name: ParamPoint::CheckValidity
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
bool ParamPoint::CheckValidity()
{
    if( pPosLabel_->text() == "---" )
    {
        pLabel_->TurnRed( 3000 );
        return false;
    }

    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::WriteMsg
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
void ParamPoint::WriteMsg( std::stringstream& strMsg )
{
    strMsg << pLabel_->text().latin1() << ": ";

    asnPoint_.type                  = EnumTypeLocalisation::point;
    asnPoint_.vecteur_point.n       = 1;
    asnPoint_.vecteur_point.elem    = &asnPos_;
    std::string strPos;
    App::GetApp().GetWorld().SimToMosMgrsCoord( point_, strPos );
    asnPos_ = strPos.c_str();
    strMsg << strPos;
}


// -----------------------------------------------------------------------------
// Name: ParamPoint::FillRemotePopupMenu
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void ParamPoint::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.pPoint_ == 0 )
        return;

    const MT_Rect& rectWorld = App::GetApp().GetWorld().GetRect();
    if( ! rectWorld.IsInside( *(context.pPoint_) ) )
        return;

    popupMenuPoint_ = *(context.pPoint_);

    popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( AcceptPopupMenuPoint() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamPoint::AcceptPopupMenuPoint
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void ParamPoint::AcceptPopupMenuPoint()
{
    point_ = popupMenuPoint_;
    std::string strPos;
    App::GetApp().GetWorld().SimToMosMgrsCoord( point_, strPos );
    pPosLabel_->setText( strPos.c_str() );
}


// -----------------------------------------------------------------------------
// Name: ParamPoint::Clear
// Created: APE 2004-08-27
// -----------------------------------------------------------------------------
void ParamPoint::Clear()
{
    pPosLabel_->setText( "---" );
}
