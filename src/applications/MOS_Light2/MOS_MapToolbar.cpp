// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MapToolbar.cpp $
// $Author: Age $
// $Modtime: 13/05/05 17:48 $
// $Revision: 2 $
// $Workfile: MOS_MapToolbar.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_MapToolbar.h"
#include "moc_MOS_MapToolbar.cpp"

#include "MOS_App.h"
#include "MOS_Options.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_Team.h"
#include "MOS_SelectedElement.h"
#include "MOS_MainWindow.h"
#include "MOS_Report_ABC.h"

#include "resources.h"

// -----------------------------------------------------------------------------
// Name: MOS_MapToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MOS_MapToolbar::MOS_MapToolbar( QMainWindow* pParent )
    : QToolBar( pParent, "map toolbar" )
{
    this->setLabel( tr( "Options carte" ) );

    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();

    MT_ToolListButton* pSmallTextButton = new MT_ToolListButton( MAKE_ICON( textsmall ), tr( "Afficher petits textes" ), "", this, SLOT( OnSmallText( float ) ), this );
    this->FillButton( *pSmallTextButton, options.nDrawSmallText_ );

    MT_ToolListButton* pLargeTextButton = new MT_ToolListButton( MAKE_ICON( textbig ), tr( "Afficher grands textes" ), "", this, SLOT( OnLargeText( float ) ), this );
    this->FillButton( *pLargeTextButton, options.nDrawLargeText_ );

    MT_ToolListButton* pDetailsButton = new MT_ToolListButton( MAKE_ICON( crossings ), tr( "Afficher détails" ), "", this, SLOT( OnDetails( float ) ), this );
    this->FillButton( *pDetailsButton, options.nDrawDetails_ );

    MT_ToolListButton* pTacticalLinesButton = new MT_ToolListButton( MAKE_ICON( tacticallines ), tr( "Afficher lines tactiques" ), "", this, SLOT( OnTacticalLines( float ) ), this );
    this->FillButton( *pTacticalLinesButton, options.nDrawTacticalLines_ );

    pWeatherButton_ = new QToolButton( MAKE_ICON( weather ), tr( "Afficher météo" ), "", this, SLOT( OnWeatherToggled() ), this );
    pWeatherButton_->setToggleButton( true );
    pWeatherButton_->setOn( MOS_MainWindow::GetMainWindow().GetOptions().bDisplayWeather_ );

    this->addSeparator();

    new QLabel( tr( "Grille:" ), this );
    MT_ToolListButton* pGridStep = new MT_ToolListButton( this );
    connect( pGridStep, SIGNAL( ItemSelected( float ) ), this, SLOT( OnGrid( float ) ) );
    MT_Float rGridStep = options.rGridStep_;
    pGridStep->AddItem( "0.1km",  0.1f,  rGridStep == 0.1 );
    pGridStep->AddItem( "0.25km", 0.25f, rGridStep == 0.25 );
    pGridStep->AddItem( "0.5km",  0.5f,  rGridStep == 0.5 );
    pGridStep->AddItem( "1.0km",  1.0f,  rGridStep == 1.0 );
    pGridStep->AddItem( "2.5km",  2.5f,  rGridStep == 2.5 );
    pGridStep->AddItem( "5.0km",  5.0f,  rGridStep == 5.0 );
    pGridStep->AddItem( "10.0km", 10.0f, rGridStep == 10.0 );

    addSeparator();
    p3DButton_ = new QToolButton( MAKE_ICON( threed ), tr( "3D" ), "", this, SLOT( On3DToggle() ), this ); // $$$$ AGE 2005-05-13: change icon
    p3DButton_->setToggleButton( true );
    p3DButton_->setOn( MOS_MainWindow::GetMainWindow().GetOptions().b3D_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_MapToolbar destructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MOS_MapToolbar::~MOS_MapToolbar()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MapToolbar::FillButton
/** @param  pButton 
    @param  iconSet 
    @param  strToolTip 
*/
// Created: APE 2004-03-17
// -----------------------------------------------------------------------------
void MOS_MapToolbar::FillButton( MT_ToolListButton& button, MOS_Options::E_State nState )
{
    button.AddItem( tr( "Auto" ), (float)MOS_Options::eAuto, nState == MOS_Options::eAuto );
    button.AddItem( tr( "Oui" ),  (float)MOS_Options::eOn,   nState == MOS_Options::eOn );
    button.AddItem( tr( "Non" ),  (float)MOS_Options::eOff,  nState == MOS_Options::eOff );
    button.setMinimumWidth( 55 );
}


// -----------------------------------------------------------------------------
// Name: MOS_MapToolbar::OnSmallText
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MOS_MapToolbar::OnSmallText( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.nDrawSmallText_ = (MOS_Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: MOS_MapToolbar::OnLargeText
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MOS_MapToolbar::OnLargeText( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.nDrawLargeText_ = (MOS_Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: MOS_MapToolbar::OnTacticalLines
/** @param  rValue 
*/
// Created: APE 2004-09-22
// -----------------------------------------------------------------------------
void MOS_MapToolbar::OnTacticalLines( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.nDrawTacticalLines_ = (MOS_Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: MOS_MapToolbar::OnDetails
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MOS_MapToolbar::OnDetails( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.nDrawDetails_ = (MOS_Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: MOS_MapToolbar::OnGrid
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MOS_MapToolbar::OnGrid( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.rGridStep_ = rValue;
}


// -----------------------------------------------------------------------------
// Name: MOS_MapToolbar::OnWeatherToggled
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
void MOS_MapToolbar::OnWeatherToggled()
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.bDisplayWeather_ = pWeatherButton_->isOn();
}

// -----------------------------------------------------------------------------
// Name: MOS_MapToolbar::On3DToggle
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
void MOS_MapToolbar::On3DToggle()
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.b3D_ = p3DButton_->isOn();
    MOS_MainWindow::GetMainWindow().DimensionsChanged();
}
