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
{
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder destructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
ModelBuilder::~ModelBuilder()
{
    controllers_.Remove( *this );
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
// Name: ModelBuilder::OnDelete
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
bool ModelBuilder::OnDelete()
{
    if( selectedAgent_ )
        delete (const Agent_ABC*)selectedAgent_;
    else if( selectedAutomat_ )
        delete (const Automat_ABC*)selectedAutomat_;
    else if( selectedGroup_ )
    {
        const Team_ABC& team = static_cast< const Team_ABC& >( selectedGroup_->Get< CommunicationHierarchies >().GetUp() );
        Iterator< const Entity_ABC& > it = team.Get< CommunicationHierarchies >().CreateSubordinateIterator();
        while( it.HasMoreElements() )
            if( &it.NextElement() != selectedGroup_ )
            {
                delete (const KnowledgeGroup_ABC*)selectedGroup_;
                return true;
            }
        return false;
    }
    else if( selectedTeam_ )
        delete (const Team_ABC*)selectedTeam_;
    else if( selectedFormation_ )
        delete (const Formation_ABC*)selectedFormation_;
    else
        return false;
    return true;
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
    ClearSelection();
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
    selectedTeam_ = &element;
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const KnowledgeGroup_ABC& element )
{
    selectedGroup_ = &element;
}
    
// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Agent_ABC& element )
{
    selectedAgent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Formation_ABC& element )
{
    selectedFormation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: AGE 2006-10-10
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::Automat_ABC& element )
{
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
