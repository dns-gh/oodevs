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
#include "ToolListButton.h"

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

    FillButton( *new ToolListButton< int >( MAKE_ICON( textsmall ),     tr( "Afficher petits textes" ),   this, options_, "SmallText" ) );
    FillButton( *new ToolListButton< int >( MAKE_ICON( textbig ),       tr( "Afficher grands textes" ),   this, options_, "BigText" ) );
    FillButton( *new ToolListButton< int >( MAKE_ICON( crossings ),     tr( "Afficher détails" ),         this, options_, "Details" ) );
    FillButton( *new ToolListButton< int >( MAKE_ICON( tacticallines ), tr( "Afficher lines tactiques" ), this, options_, "TacticalLines" ) );

    pWeatherButton_ = new QToolButton( MAKE_ICON( weather ), tr( "Afficher météo" ), "", this, SLOT( OnWeatherToggled() ), this );
    pWeatherButton_->setToggleButton( true );

    addSeparator();

    new QLabel( tr( "Grille:" ), this );
    ToolListButton< float >* pGridStep_ = new ToolListButton< float >( this, options_, "GridSize" );
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
void MapToolbar::FillButton( ToolListButton< int >& button )
{
    button.AddItem( tr( "Auto" ), 2, true ); // $$$$ AGE 2006-03-27: 
    button.AddItem( tr( "Oui" ),  1, false );
    button.AddItem( tr( "Non" ),  0, false );
    button.setMinimumWidth( 55 );
}

// -----------------------------------------------------------------------------
// Name: MapToolbar::OptionChanged
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void MapToolbar::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "Weather" )
        pWeatherButton_->setOn( value.To< bool >() );
    else if( name == "3D" )
        p3DButton_->setOn( value.To< bool >() );
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
