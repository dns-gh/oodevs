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

#include "MainWindow.h"
#include "Options.h"


// -----------------------------------------------------------------------------
// Name: UnitToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
UnitToolbar::UnitToolbar( QMainWindow* pParent )
    : QToolBar( pParent, "unit toolbar" )
{
    this->setLabel( tr( "Options unité" ) );

    Options& options = MainWindow::GetMainWindow().GetOptions();

    MT_ToolListButton* pVisionLinesButton = new MT_ToolListButton( MAKE_ICON( visionlines ), tr( "Afficher lines de vision" ), "", this, SLOT( OnVisionLines( float ) ), this );
    this->FillButton( *pVisionLinesButton, options.nDrawVisionLines_ );

    MT_ToolListButton* pVisionConesButton = new MT_ToolListButton( MAKE_ICON( visioncones ), tr( "Afficher cones de vision" ), "", this, SLOT( OnVisionCones( float ) ), this );
    this->FillButton( *pVisionConesButton, options.nDrawVisionCones_ );

    MT_ToolListButton* pVisionSurfacesButton = new MT_ToolListButton( MAKE_ICON( visionsurfaces ), tr( "Afficher surfaces de vision" ), "", this, SLOT( OnVisionSurfaces( float ) ), this );
    this->FillButton( *pVisionSurfacesButton, options.nDrawVisionSurfaces_ );

    MT_ToolListButton* pPathsButton = new MT_ToolListButton( MAKE_ICON( path ), tr( "Afficher les itinéraires" ), "", this, SLOT( OnPaths( float ) ), this );
    this->FillButton( *pPathsButton, options.nDrawPaths_ );

    pOldPathsButton_ = new QToolButton( MAKE_ICON( oldpath ), tr( "Afficher chemin parcouru" ), "", this, SLOT( OnOldPathsToggled() ), this );
    pOldPathsButton_->setToggleButton( true );
    pOldPathsButton_->setOn( MainWindow::GetMainWindow().GetOptions().bDrawOldPaths_ );
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
/** @param  pButton 
    @param  iconSet 
    @param  strToolTip 
*/
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
    Options& options = MainWindow::GetMainWindow().GetOptions();
    options.nDrawVisionLines_ = (Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar::OnVisionCones
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void UnitToolbar::OnVisionCones( float rValue )
{
    Options& options = MainWindow::GetMainWindow().GetOptions();
    options.nDrawVisionCones_ = (Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar::OnVisionSurfaces
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void UnitToolbar::OnVisionSurfaces( float rValue )
{
    Options& options = MainWindow::GetMainWindow().GetOptions();
    options.nDrawVisionSurfaces_ = (Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar::OnPaths
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void UnitToolbar::OnPaths( float rValue )
{
    Options& options = MainWindow::GetMainWindow().GetOptions();
    options.nDrawPaths_ = (Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: UnitToolbar::OnOldPathsToggled
// Created: APE 2004-06-15
// -----------------------------------------------------------------------------
void UnitToolbar::OnOldPathsToggled()
{
    Options& options = MainWindow::GetMainWindow().GetOptions();
    options.bDrawOldPaths_ = pOldPathsButton_->isOn();
}
