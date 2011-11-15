// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::CoordinateSystemsPanel */

#include "clients_gui_pch.h"
#include "CoordinateSystemsPanel.h"
#include "moc_CoordinateSystemsPanel.cpp"
#include "clients_kernel/CoordinateSystems.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

using namespace gui;
// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel constructor
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
CoordinateSystemsPanel::CoordinateSystemsPanel( QWidget* parent, kernel::Controllers& controllers, kernel::CoordinateSystems& coordSystems )
    : PreferencePanel_ABC( parent, "CoordinateSystemsPanel" )
    , controllers_ ( controllers )
    , options_  ( controllers.options_ )
    , coordinateSystems_( coordSystems )
{
    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Vertical, tr( "Coordinate System" ), this );
    new QLabel( tr( "Select current coordinate system:" ), box );
    listCoordSys_ = new QComboBox( box );
    listCoordSys_->setEditable( false );

    for( kernel::CoordinateSystems::CIT_spatialReference it = coordinateSystems_.systems_.begin(); it != coordinateSystems_.systems_.end(); it++ )
        listCoordSys_->insertItem( it->second->c_str(), it->first );

    setWidget( box );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel destructor
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
CoordinateSystemsPanel::~CoordinateSystemsPanel()
{
     controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel::Commit
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
void CoordinateSystemsPanel::Commit()
{
    coordinateSystems_.defaultCoordinateSystem_ = listCoordSys_->currentItem();
    options_.Change( "CoordSystem", coordinateSystems_.defaultCoordinateSystem_ );
    previousCoordinateSystem_ = coordinateSystems_.defaultCoordinateSystem_;
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel::Reset
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
void CoordinateSystemsPanel::Reset()
{
    listCoordSys_->setCurrentItem( previousCoordinateSystem_ );
    coordinateSystems_.defaultCoordinateSystem_ = previousCoordinateSystem_;
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel::OptionChanged
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
void CoordinateSystemsPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    QString option ( name.c_str() );
    if( option == "CoordSystem" )
    {
        listCoordSys_->setCurrentItem( value.To< int >() );
        coordinateSystems_.defaultCoordinateSystem_ = listCoordSys_->currentItem();
    }
}

