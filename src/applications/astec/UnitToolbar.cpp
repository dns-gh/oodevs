// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/UnitToolbar.cpp $
// $Author: Ape $
// $Modtime: 21/09/04 16:12 $
// $Revision: 1 $
// $Workfile: UnitToolbar.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "UnitToolbar.h"
#include "moc_UnitToolbar.cpp"

#include "App.h"

// -----------------------------------------------------------------------------
// Name: UnitToolbar constructor
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
UnitToolbar::UnitToolbar( QMainWindow* pParent )
    : QToolBar( pParent, "unit toolbar" )
    , options_( App::GetApp().GetOptions() )
{
    setLabel( tr( "Options unité" ) );

    MT_ToolListButton* pVisionLinesButton = new MT_ToolListButton( MAKE_ICON( visionlines ), tr( "Afficher lines de vision" ), "", this, SLOT( OnVisionLines( float ) ), this );
    this->FillButton( *pVisionLinesButton, options_.nDrawVisionLines_ );

    MT_ToolListButton* pVisionConesButton = new MT_ToolListButton( MAKE_ICON( visioncones ), tr( "Afficher cones de vision" ), "", this, SLOT( OnVisionCones( float ) ), this );
    this->FillButton( *pVisionConesButton, options_.nDrawVisionCones_ );

    MT_ToolListButton* pVisionSurfacesButton = new MT_ToolListButton( MAKE_ICON( visionsurfaces ), tr( "Afficher surfaces de vision" ), "", this, SLOT( OnVisionSurfaces( float ) ), this );
    this->FillButton( *pVisionSurfacesButton, options_.nDrawVisionSurfaces_ );

    MT_ToolListButton* pPathsButton = new MT_ToolListButton( MAKE_ICON( path ), tr( "Afficher les itinéraires" ), "", this, SLOT( OnPaths( float ) ), this );
    this->FillButton( *pPathsButton, options_.nDrawPaths_ );

    pOldPathsButton_ = new QToolButton( MAKE_ICON( oldpath ), tr( "Afficher chemin parcouru" ), "", this, SLOT( OnOldPathsToggled() ), this );
    pOldPathsButton_->setToggleButton( true );
    pOldPathsButton_->setOn( options_.bDrawOldPaths_ );
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar destructor
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
UnitToolbar::~UnitToolbar()
{
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar::FillButton
// Created: APE 2004-03-17
// -----------------------------------------------------------------------------
void UnitToolbar::FillButton( MT_ToolListButton& button, Options::E_State nState )
{
    button.AddItem( tr( "Sel." ),  (float)Options::eAuto, nState == Options::eAuto );
    button.AddItem( tr( "Tous" ),  (float)Options::eOn,   nState == Options::eOn );
    button.AddItem( tr( "Aucun" ), (float)Options::eOff,  nState == Options::eOff );
    button.setMinimumWidth( 65 );
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar::OnVisionLines
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void UnitToolbar::OnVisionLines( float rValue )
{
    options_.nDrawVisionLines_ = (Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar::OnVisionCones
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void UnitToolbar::OnVisionCones( float rValue )
{
    options_.nDrawVisionCones_ = (Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar::OnVisionSurfaces
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void UnitToolbar::OnVisionSurfaces( float rValue )
{
    options_.nDrawVisionSurfaces_ = (Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar::OnPaths
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void UnitToolbar::OnPaths( float rValue )
{
    options_.nDrawPaths_ = (Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar::OnOldPathsToggled
// Created: APE 2004-06-15
// -----------------------------------------------------------------------------
void UnitToolbar::OnOldPathsToggled()
{
    options_.bDrawOldPaths_ = pOldPathsButton_->isOn();
}
