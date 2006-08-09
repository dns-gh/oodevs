// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticToolbar.h"
#include "moc_LogisticToolbar.cpp"

#include "astec_kernel/Controllers.h"
#include "astec_kernel/TristateOption.h"
#include "astec_kernel/Agent_ABC.h"
#include "astec_gui/AgentsLayer.h"
#include "astec_gui/ToolListButton.h"
#include "astec_gui/resources.h"

namespace
{
    void Populate( ToolListButton< TristateOption >& button )
    {
        button.AddItem( qApp->tr( TristateOption::auto_ ), TristateOption::auto_ );
        button.AddItem( qApp->tr( TristateOption::on_ ),   TristateOption::on_ );
        button.AddItem( qApp->tr( TristateOption::off_ ),  TristateOption::off_ );
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
    setLabel( tr( "Options logistique" ) );

    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( loglink ),     tr( "Afficher les liens logistiques" ),               this, controllers.options_, "LogisticLinks" ) );
    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( missinglog ),  tr( "Afficher les liens logistiques manquant" ),      this, controllers.options_, "MissingLogisticLinks" ) );
    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( realtimelog ), tr( "Affichage temps réel des actions logistiques" ), this, controllers.options_, "RealTimeLogistic" ) );
    
    new QToolButton( MAKE_ICON( aggregate ),    tr( "Aggréger tous les automates" ), "", this,    SLOT( AggregateAll() ), this );
    new QToolButton( MAKE_ICON( desaggregate ), tr( "Désaggréger tous les automates" ), "", this, SLOT( DisaggregateAll() ), this );

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
void LogisticToolbar::NotifyCreated( const Agent_ABC& agent )
{
    if( agent.GetAutomatType() && std::find( automats_.begin(), automats_.end(), &agent ) == automats_.end() )
        automats_.push_back( &agent );
}
 
// -----------------------------------------------------------------------------
// Name: LogisticToolbar::NotifyDeleted
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticToolbar::NotifyDeleted( const Agent_ABC& agent )
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
