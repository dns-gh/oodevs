// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AutomatsLayer.h"
#include "moc_AutomatsLayer.cpp"
#include "AgentsLayer.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AutomatsLayer constructor
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
AutomatsLayer::AutomatsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const Profile_ABC& profile, AgentsLayer& agents )
    : EntityLayer< Automat_ABC >( controllers, tools, strategy, view, profile )
    , agents_( agents )
    , selected_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer destructor
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
AutomatsLayer::~AutomatsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Aggregate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Aggregate( const Automat_ABC& automat )
{
    Toggle( automat, true );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Disaggregate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Disaggregate( const Automat_ABC& automat )
{
    Toggle( automat, false );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Aggregate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Aggregate()
{
    if( selected_ )
        Aggregate( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Disaggregate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Disaggregate()
{
    if( selected_ )
        Disaggregate( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::NotifyContextMenu
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::NotifyContextMenu( const Automat_ABC& automat, kernel::ContextMenu& menu )
{
    selected_ = &automat;
    if( aggregated_.find( &automat ) == aggregated_.end() )
        menu.InsertItem( "Interface", tr( "Aggregate" ), this, SLOT( Aggregate() ) );
    else
        menu.InsertItem( "Interface", tr( "Disaggregate" ), this, SLOT( Disaggregate() ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Select
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Select( const Entity_ABC& entity, bool )
{
    entity.Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Toggle
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Toggle( const Automat_ABC& automat, bool aggregate )
{
    automat.Interface().Apply( & Aggregatable_ABC::Aggregate, aggregate );
    if( aggregate )
    {
        agents_.Aggregate( automat );
        aggregated_.insert( &automat );
    }
    else
    {
        agents_.Disaggregate( automat );
        aggregated_.erase( &automat );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::ContextMenu
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::ContextMenu( const Entity_ABC& entity, const geometry::Point2f& point, const QPoint& where )
{
    const Automat_ABC* automat = static_cast< const Automat_ABC* >( &entity );
    if( aggregated_.find( automat ) != aggregated_.end() )
        controllers_.actions_.ContextMenu( *automat, entity, point, where );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::ShouldDisplay
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
bool AutomatsLayer::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    return EntityLayer< Automat_ABC >::ShouldDisplay( entity ) && aggregated_.find( &entity ) != aggregated_.end();
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::NotifySelected
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void AutomatsLayer::NotifySelected( const kernel::Automat_ABC* automat )
{
    EntityLayer< Automat_ABC >::NotifySelected( automat );
    selected_ = automat;
}
