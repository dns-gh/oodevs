// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticToolbar.h"
#include "moc_LogisticToolbar.cpp"

#include "Controllers.h"
#include "TristateOption.h"
#include "ToolListButton.h"
#include "Agent.h"
#include "AgentsLayer.h"

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

    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( loglink ),     tr( "Afficher les liens logistiques" ),               this, controllers.options_, "LogisticLinks" ) );
    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( missinglog ),  tr( "Afficher les liens logistiques manquant" ),      this, controllers.options_, "MissingLogisticLinks" ) );
    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( realtimelog ), tr( "Affichage temps r�el des actions logistiques" ), this, controllers.options_, "RealTimeLogistic" ) );
    
    new QToolButton( MAKE_ICON( aggregate ),    tr( "Aggr�ger tous les automates" ), "", this,    SLOT( AggregateAll() ), this );
    new QToolButton( MAKE_ICON( desaggregate ), tr( "D�saggr�ger tous les automates" ), "", this, SLOT( DisaggregateAll() ), this );

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
void LogisticToolbar::NotifyCreated( const Agent& agent )
{
    if( agent.GetAutomatType() && std::find( automats_.begin(), automats_.end(), &agent ) == automats_.end() )
        automats_.push_back( &agent );
}
 
// -----------------------------------------------------------------------------
// Name: LogisticToolbar::NotifyDeleted
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void LogisticToolbar::NotifyDeleted( const Agent& agent )
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
