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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_UnitToolbar.cpp $
// $Author: Ape $
// $Modtime: 21/09/04 16:12 $
// $Revision: 1 $
// $Workfile: MOS_UnitToolbar.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_UnitToolbar.h"
#include "moc_MOS_UnitToolbar.cpp"

#include "MOS_MainWindow.h"
#include "MOS_Options.h"


// -----------------------------------------------------------------------------
// Name: MOS_UnitToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
MOS_UnitToolbar::MOS_UnitToolbar( QMainWindow* pParent )
    : QToolBar( pParent, "unit toolbar" )
{
    this->setLabel( tr( "Options unité" ) );

    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();

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
    pOldPathsButton_->setOn( MOS_MainWindow::GetMainWindow().GetOptions().bDrawOldPaths_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitToolbar destructor
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
MOS_UnitToolbar::~MOS_UnitToolbar()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitToolbar::FillButton
/** @param  pButton 
    @param  iconSet 
    @param  strToolTip 
*/
// Created: APE 2004-03-17
// -----------------------------------------------------------------------------
void MOS_UnitToolbar::FillButton( MT_ToolListButton& button, MOS_Options::E_State nState )
{
    button.AddItem( tr( "Sel." ),  (float)MOS_Options::eAuto, nState == MOS_Options::eAuto );
    button.AddItem( tr( "Tous" ),  (float)MOS_Options::eOn,   nState == MOS_Options::eOn );
    button.AddItem( tr( "Aucun" ), (float)MOS_Options::eOff,  nState == MOS_Options::eOff );
    button.setMinimumWidth( 65 );
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitToolbar::OnVisionLines
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MOS_UnitToolbar::OnVisionLines( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.nDrawVisionLines_ = (MOS_Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitToolbar::OnVisionCones
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MOS_UnitToolbar::OnVisionCones( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.nDrawVisionCones_ = (MOS_Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitToolbar::OnVisionSurfaces
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MOS_UnitToolbar::OnVisionSurfaces( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.nDrawVisionSurfaces_ = (MOS_Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitToolbar::OnPaths
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MOS_UnitToolbar::OnPaths( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.nDrawPaths_ = (MOS_Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitToolbar::OnOldPathsToggled
// Created: APE 2004-06-15
// -----------------------------------------------------------------------------
void MOS_UnitToolbar::OnOldPathsToggled()
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.bDrawOldPaths_ = pOldPathsButton_->isOn();
}
