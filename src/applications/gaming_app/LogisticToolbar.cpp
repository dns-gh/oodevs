// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticToolbar.h"
#include "moc_LogisticToolbar.cpp"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_gui/AgentsLayer.h"
#include "clients_gui/ToolListButton.h"
#include "clients_gui/resources.h"

using namespace kernel;
using namespace gui;

namespace
{
    void Populate( ToolListButton< TristateOption >& button )
    {
        button.AddItem( TristateOption::AutoName(), TristateOption::Auto() );
        button.AddItem( TristateOption::OnName(),   TristateOption::On() );
        button.AddItem( TristateOption::OffName(),  TristateOption::Off() );
        button.setMinimumWidth( 55 );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar constructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
LogisticToolbar::LogisticToolbar( QMainWindow* pParent, Controllers& controllers, AgentsLayer& agents )
    : QToolBar( pParent, "logistic toolbar" )
    , controllers_( controllers )
    , agents_( agents )
{
    setLabel( tr( "Logistic options" ) );

    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( loglink ),     tr( "Display logistic links" ),             this, controllers.options_, "LogisticLinks" ) );
    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( missinglog ),  tr( "Display missing logistic links" ),     this, controllers.options_, "MissingLogisticLinks" ) );
    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( realtimelog ), tr( "Real-time logistic actions display" ), this, controllers.options_, "RealTimeLogistic" ) );
    
    new QToolButton( MAKE_ICON( aggregate ),    tr( "Aggregate all automats" ), "", this,    SLOT( AggregateAll() ), this );
    new QToolButton( MAKE_ICON( desaggregate ), tr( "Disaggregate all automats" ), "", this, SLOT( DisaggregateAll() ), this );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar destructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
LogisticToolbar::~LogisticToolbar()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar::NotifyCreated
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticToolbar::NotifyCreated( const Automat_ABC& agent )
{
    if( std::find( automats_.begin(), automats_.end(), &agent ) == automats_.end() )
        automats_.push_back( &agent );
}
 
// -----------------------------------------------------------------------------
// Name: LogisticToolbar::NotifyDeleted
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticToolbar::NotifyDeleted( const Automat_ABC& agent )
{
    IT_Agents it = std::find( automats_.begin(), automats_.end(), &agent );
    if( it != automats_.end() )
    {
        std::swap( *it, automats_.back() );
        automats_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar::AggregateAll
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticToolbar::AggregateAll()
{
    for( CIT_Agents it = automats_.begin(); it != automats_.end(); ++it )
        agents_.Aggregate( **it );
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar::DisaggregateAll
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticToolbar::DisaggregateAll()
{
    for( CIT_Agents it = automats_.begin(); it != automats_.end(); ++it )
        agents_.Disaggregate( **it );
}
