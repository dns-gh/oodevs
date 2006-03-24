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
// $Archive: /MVW_v10/Build/SDK/Light2/src/MapToolbar.cpp $
// $Author: Age $
// $Modtime: 13/05/05 17:48 $
// $Revision: 2 $
// $Workfile: MapToolbar.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "MapToolbar.h"
#include "moc_MapToolbar.cpp"
#include "Controllers.h"
#include "Options.h"
#include "OptionVariant.h"

#include "MT/MT_Qt/MT_ToolListButton.h"

#include "resources.h"

// -----------------------------------------------------------------------------
// Name: MapToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MapToolbar::MapToolbar( QMainWindow* pParent, Controllers& controllers )
    : QToolBar    ( pParent, "map toolbar" )
    , controllers_( controllers )
    , options_    ( controllers_.options_ )
{
    setLabel( tr( "Options carte" ) );

    pSmallTextButton_ = new MT_ToolListButton( MAKE_ICON( textsmall ), tr( "Afficher petits textes" ), "", this, SLOT( OnSmallText( float ) ), this );
    FillButton( *pSmallTextButton_ );

    pLargeTextButton_ = new MT_ToolListButton( MAKE_ICON( textbig ), tr( "Afficher grands textes" ), "", this, SLOT( OnLargeText( float ) ), this );
    FillButton( *pLargeTextButton_ );

    pDetailsButton_ = new MT_ToolListButton( MAKE_ICON( crossings ), tr( "Afficher détails" ), "", this, SLOT( OnDetails( float ) ), this );
    FillButton( *pDetailsButton_ );

    pTacticalLinesButton_ = new MT_ToolListButton( MAKE_ICON( tacticallines ), tr( "Afficher lines tactiques" ), "", this, SLOT( OnTacticalLines( float ) ), this );
    FillButton( *pTacticalLinesButton_ );

    pWeatherButton_ = new QToolButton( MAKE_ICON( weather ), tr( "Afficher météo" ), "", this, SLOT( OnWeatherToggled() ), this );
    pWeatherButton_->setToggleButton( true );

    addSeparator();

    new QLabel( tr( "Grille:" ), this );
    pGridStep_ = new MT_ToolListButton( this );
    connect( pGridStep_, SIGNAL( ItemSelected( float ) ), this, SLOT( OnGrid( float ) ) );
    pGridStep_->AddItem( "0.1km",  0.1f,  false );
    pGridStep_->AddItem( "0.25km", 0.25f, false );
    pGridStep_->AddItem( "0.5km",  0.5f,  false );
    pGridStep_->AddItem( "1.0km",  1.0f,  false );
    pGridStep_->AddItem( "2.5km",  2.5f,  false );
    pGridStep_->AddItem( "5.0km",  5.0f,  false );
    pGridStep_->AddItem( "10.0km", 10.0f, true  );

    addSeparator();
    p3DButton_ = new QToolButton( MAKE_ICON( threed ), tr( "3D" ), "", this, SLOT( On3DToggle() ), this ); // $$$$ AGE 2005-05-13: change icon
    p3DButton_->setToggleButton( true );
    p3DButton_->setOn( false );

    controllers_.Register( *this );
}


// -----------------------------------------------------------------------------
// Name: MapToolbar destructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MapToolbar::~MapToolbar()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: MapToolbar::FillButton
// Created: APE 2004-03-17
// -----------------------------------------------------------------------------
void MapToolbar::FillButton( MT_ToolListButton& button )
{
    button.AddItem( tr( "Auto" ), (float)2, true );
    button.AddItem( tr( "Oui" ),  (float)1, false );
    button.AddItem( tr( "Non" ),  (float)0, false );
    button.setMinimumWidth( 55 );
}

// -----------------------------------------------------------------------------
// Name: MapToolbar::OptionChanged
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void MapToolbar::OptionChanged( const std::string& name, const OptionVariant& value )
{
    // $$$$ AGE 2006-03-24: 
    if( name == "SmallText" )
        ;
    else if( name == "LargeText" )
        ;
    else if( name == "TacticalLines" )
        ;
    else if( name == "Details" )
        ;
    else if( name == "GridSize" )
        ;
    else if( name == "Weather" ) 
        ;
    else if( name == "3D" )
        ;
}

// -----------------------------------------------------------------------------
// Name: MapToolbar::OnSmallText
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MapToolbar::OnSmallText( float rValue )
{
    options_.Change( "SmallText", (int)(rValue) );
}

// -----------------------------------------------------------------------------
// Name: MapToolbar::OnLargeText
/** @param  rValue 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MapToolbar::OnLargeText( float rValue )
{
    options_.Change( "LargeText", (int)(rValue) );
}


// -----------------------------------------------------------------------------
// Name: MapToolbar::OnTacticalLines
// Created: APE 2004-09-22
// -----------------------------------------------------------------------------
void MapToolbar::OnTacticalLines( float rValue )
{
    options_.Change( "TacticalLines", (int)(rValue) );
}


// -----------------------------------------------------------------------------
// Name: MapToolbar::OnDetails
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MapToolbar::OnDetails( float rValue )
{
    options_.Change( "Details", (int)(rValue) );
}


// -----------------------------------------------------------------------------
// Name: MapToolbar::OnGrid
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MapToolbar::OnGrid( float rValue )
{
    options_.Change( "GridSize", rValue );
}

// -----------------------------------------------------------------------------
// Name: MapToolbar::OnWeatherToggled
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
void MapToolbar::OnWeatherToggled()
{
    options_.Change( "Weather", pWeatherButton_->isOn() );
}

// -----------------------------------------------------------------------------
// Name: MapToolbar::On3DToggle
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
void MapToolbar::On3DToggle()
{
    options_.Change( "3D", p3DButton_->isOn() );
}
