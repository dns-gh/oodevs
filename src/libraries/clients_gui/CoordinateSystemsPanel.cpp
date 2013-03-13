// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CoordinateSystemsPanel.h"
#include "moc_CoordinateSystemsPanel.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

using namespace gui;

typedef kernel::CoordinateSystems::Projection Projection;

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel constructor
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
CoordinateSystemsPanel::CoordinateSystemsPanel( QWidget* parent, kernel::Controllers& controllers, kernel::CoordinateSystems& coordSystems )
    : PreferencePanel_ABC( parent, "CoordinateSystemsPanel" )
    , controllers_( controllers )
    , options_( controllers.options_ )
    , coordinateSystems_( coordSystems )
    , previousCoordinateSystem_( kernel::CoordinateSystems::E_Mgrs )
{
    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Vertical, tr( "Coordinate System" ), this );
    new QLabel( tr( "Select current coordinate system:" ), box );
    listCoordSys_ = new QComboBox( box );
    listCoordSys_->setEditable( false );

    const kernel::CoordinateSystems::T_SpatialReference& systems = coordinateSystems_.GetSystems();
    for( auto it = systems.begin(); it != systems.end(); ++it )
        listCoordSys_->insertItem( it->second, it->first );

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
    coordinateSystems_.SetDefault( static_cast< Projection >( listCoordSys_->currentItem() ) );
    options_.Change( "CoordSystem", coordinateSystems_.GetDefault() );
    previousCoordinateSystem_ = coordinateSystems_.GetDefault();
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel::Reset
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
void CoordinateSystemsPanel::Reset()
{
    listCoordSys_->setCurrentItem( previousCoordinateSystem_ );
    coordinateSystems_.SetDefault( previousCoordinateSystem_ );
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel::OptionChanged
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
void CoordinateSystemsPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "CoordSystem" )
    {
        listCoordSys_->setCurrentItem( value.To< int >() );
        coordinateSystems_.SetDefault( static_cast< Projection >( listCoordSys_->currentItem() ) );
    }
}
