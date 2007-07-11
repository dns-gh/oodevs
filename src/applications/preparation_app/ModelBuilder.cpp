// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ModelBuilder.h"
#include "moc_ModelBuilder.cpp"
#include "preparation/Model.h"
#include "preparation/TeamsModel.h"
#include "preparation/AgentsModel.h"
#include "preparation/FormationModel.h"
#include "preparation/LimitsModel.h"
#include "preparation/Team.h"
#include "preparation/Formation.h"
#include "preparation/KnowledgeGroup.h"
#include "preparation/Automat.h"
#include "preparation/Agent.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/Level.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Controllers.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ModelBuilder constructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
ModelBuilder::ModelBuilder( Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
    , selectedTeam_( controllers )
    , selectedGroup_( controllers )
    , selectedAgent_( controllers )
    , selectedAutomat_( controllers )
    , selectedFormation_( controllers )
    , toDelete_( controllers )
{
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder destructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
ModelBuilder::~ModelBuilder()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreateFormation
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreateFormation( int levelId )
{
    if( selectedTeam_ )
        model_.formations_.Create( *selectedTeam_.ConstCast(), levelId );
    else if( selectedFormation_ )
        model_.formations_.Create( *selectedFormation_.ConstCast(), levelId );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreate
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreate()
{
    model_.teams_.CreateTeam();
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreateCommunication
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreateCommunication()
{
    if( selectedTeam_ )
        model_.teams_.CreateKnowledgeGroup( *selectedTeam_ );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CanCreateLine
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
bool ModelBuilder::CanCreateLine() const
{
    return selectedFormation_ || selectedAutomat_;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CreateLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void ModelBuilder::CreateLimit( const T_PointVector& points )
{
    const Entity_ABC* element = selectedFormation_ ? (const Entity_ABC*)selectedFormation_ : (const Entity_ABC*)selectedAutomat_;
    if( element )
        model_.limits_.CreateLimit( points, *const_cast< kernel::Entity_ABC* >( element ) );
//    else
//        throw std::exception( tr( "Cannot create a limit at the selected tactical level." ) );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CreateLima
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void ModelBuilder::CreateLima( const T_PointVector& points )
{
    const Entity_ABC* element = selectedFormation_ ? (const Entity_ABC*)selectedFormation_ : (const Entity_ABC*)selectedAutomat_;
    if( element )
        model_.limits_.CreateLima( points, *const_cast< kernel::Entity_ABC* >( element ) );
//    else
//        throw std::exception( tr( "Cannot create a lima at the selected tactical level." ) );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-11-28
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    toDelete_ = &entity;
    menu.InsertItem( "Command", tr( "Delete" ), this, SLOT( OnDelete() ) );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnDelete
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::OnDelete()
{
    if( toDelete_ )
        DeleteEntity( *toDelete_ );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::DeleteEntity
// Created: AGE 2006-11-28
// -----------------------------------------------------------------------------
void ModelBuilder::DeleteEntity( const Entity_ABC& entity )
{
    if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        Iterator< const Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
        while( it.HasMoreElements() )
            DeleteEntity( it.NextElement() );
    }
    if( const kernel::CommunicationHierarchies* hierarchies = entity.Retrieve< kernel::CommunicationHierarchies >() )
    {
        Iterator< const Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
        while( it.HasMoreElements() )
            DeleteEntity( it.NextElement() );
    }
    delete &entity;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::ClearSelection
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void ModelBuilder::ClearSelection()
{
    selectedTeam_ = 0;
    selectedGroup_ = 0;
    selectedAgent_ = 0;
    selectedAutomat_ = 0;
    selectedFormation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::BeforeSelection
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::BeforeSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::AfterSelection
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Team_ABC& element )
{
    ClearSelection();
    selectedTeam_ = &element;
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const KnowledgeGroup_ABC& element )
{
    ClearSelection();
    selectedGroup_ = &element;
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Agent_ABC& element )
{
    ClearSelection();
    selectedAgent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Formation_ABC& element )
{
    ClearSelection();
    selectedFormation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: AGE 2006-10-10
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::Automat_ABC& element )
{
    ClearSelection();
    selectedAutomat_ = &element;
}

namespace
{
    template< typename Concrete, typename T >
    void Rename( T& entity, const QString& text )
    {
        if( Concrete* concrete = dynamic_cast< Concrete* >( entity.ConstCast() ) )
            concrete->Rename( text );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnRename
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void ModelBuilder::OnRename( QListViewItem*, int, const QString& text )
{
    if( selectedTeam_ )
        Rename< Team >( selectedTeam_, text );
    else if( selectedAgent_ )
        Rename< Agent >( selectedAgent_, text );
    else if( selectedAutomat_ )
        Rename< Automat >( selectedAutomat_, text );
    else if( selectedFormation_ )
        Rename< Formation >( selectedFormation_, text );
    else if( selectedGroup_ )
        Rename< KnowledgeGroup >( selectedGroup_, text );
}
