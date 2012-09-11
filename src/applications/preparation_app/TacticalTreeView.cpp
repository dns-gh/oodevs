// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TacticalTreeView.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Positions.h"
#include "preparation/AgentsModel.h"
#include "preparation/EntityCommunications.h"
#include "preparation/Model.h"
#include "preparation/TacticalHierarchies.h"

// -----------------------------------------------------------------------------
// Name: TacticalTreeView constructor
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
TacticalTreeView::TacticalTreeView( QWidget* parent,kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, Model& model )
    : gui::TacticalTreeView( controllers, profile, modelObserver, symbols, parent )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView destructor
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
TacticalTreeView::~TacticalTreeView()
{
    // NOTHING
}

namespace
{
    kernel::Entity_ABC& GetFirstCommunicationChild( const kernel::Entity_ABC& entity )
    {
        tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::CommunicationHierarchies >().CreateSubordinateIterator();
        if( it.HasMoreElements() )
            return const_cast< kernel::Entity_ABC& >( it.NextElement() );
        throw std::runtime_error( "No communication child found" );
    }

    void UpdateCommunicationHierarchies( kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior )
    {
        const kernel::Entity_ABC& tacticalTop = superior.Get< kernel::TacticalHierarchies >().GetTop();
        kernel::CommunicationHierarchies* com = entity.Retrieve< kernel::CommunicationHierarchies >();

        if( com && &com->GetTop() != &tacticalTop )
            static_cast< ::EntityCommunications* >( com )->ChangeSuperior( GetFirstCommunicationChild( tacticalTop ) );

        tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
        while( it.HasMoreElements() )
            UpdateCommunicationHierarchies( const_cast< kernel::Entity_ABC& >( it.NextElement() ), superior );
    }

    void ChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) // $$$$ SBO 2006-09-28: cast-machine
    {
        kernel::Entity_ABC& ent = const_cast< kernel::Entity_ABC& >( entity );
        if( kernel::TacticalHierarchies* hierarchies = ent.Retrieve< kernel::TacticalHierarchies >() )
        {
            UpdateCommunicationHierarchies( ent, superior );
            static_cast< ::TacticalHierarchies* >( hierarchies )->ChangeSuperior( const_cast< kernel::Entity_ABC& >( superior ) );
        }
    }
}

namespace
{
    bool IsSubordinateOf( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior )
    {
        if( const kernel::TacticalHierarchies* pTactical = superior.Retrieve< kernel::TacticalHierarchies >() )
            return pTactical->IsSubordinateOf( entity );
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::CanChangeSuperior
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
bool TacticalTreeView::CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const
{
    if( dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        return dynamic_cast< const kernel::Automat_ABC* >( &superior ) != 0;
    if( dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        return dynamic_cast< const kernel::Formation_ABC* >( &superior ) != 0;
    if( dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        return ( !IsSubordinateOf( entity, superior ) && dynamic_cast< const kernel::Formation_ABC* >( &superior ) != 0 ) || dynamic_cast< const kernel::Team_ABC* >( &superior ) != 0 ;
    if( const kernel::Ghost_ABC* ghost = dynamic_cast< const kernel::Ghost_ABC* >( &entity ) )
        return ( ghost->GetGhostType() == eGhostType_Automat && dynamic_cast< const kernel::Formation_ABC* >( &superior ) != 0 )
        || ( ghost->GetGhostType() == eGhostType_Agent && dynamic_cast< const kernel::Team_ABC* >( &superior ) != 0 );
    if( dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity ) )
        return dynamic_cast< const kernel::Formation_ABC* >( &superior ) != 0;
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target )
{
    if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &target ) )
    {
        ChangeSuperior( item, target );
        kernel::CommunicationHierarchies* com = const_cast< kernel::Agent_ABC& >( item ).Retrieve< kernel::CommunicationHierarchies >();
        if( com )
            static_cast< ::EntityCommunications* >( com )->ChangeSuperior( const_cast< kernel::Entity_ABC& >( target ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target)
{
    if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &target ) )
        ChangeSuperior( item, target );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target )
{
    if( dynamic_cast< const kernel::Formation_ABC* >( &target ) || dynamic_cast< const kernel::Team_ABC* >( &target ) )
        ChangeSuperior( item, target );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Ghost_ABC& item, const kernel::Entity_ABC& target )
{
    const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &target );
    const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &target );
    if( ( formation && item.GetGhostType() == eGhostType_Automat )
     || ( automat && item.GetGhostType() == eGhostType_Agent ) )
        ChangeSuperior( item, target );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target )
{
    if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &target ) )
    {
        tools::Iterator< const kernel::Entity_ABC& > children = item.Get< kernel::CommunicationHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
            ChangeSuperior( children.NextElement(), *formation );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::AgentType& item, kernel::Entity_ABC& target )
{
    const geometry::Point2f position = target.Get< kernel::Positions >().GetPosition();
    if( kernel::Entity_ABC* result = model_.agents_.CreateAgent( static_cast< kernel::Ghost_ABC& >( target ), item, position ) )
    {
        delete static_cast< const kernel::Ghost_ABC* >( &target );
        setFocus(); // utile ?
        result->Select( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::AutomatType& item, kernel::Entity_ABC& target )
{
    const geometry::Point2f position = target.Get< kernel::Positions >().GetPosition();
    if( kernel::Entity_ABC* result = model_.agents_.CreateAutomatInsteadOf( target, item, position ) )
    {
        delete static_cast< const kernel::Ghost_ABC* >( &target );
        setFocus();
        result->Select( controllers_.actions_ );
    }
}
