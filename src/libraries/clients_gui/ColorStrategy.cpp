// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ColorStrategy.h"
#include "clients_kernel/GlTools_ABC.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/TacticalLine_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ColorStrategy constructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
ColorStrategy::ColorStrategy( Controllers& controllers, GlTools_ABC& tools )
    : controllers_       ( controllers )
    , tools_             ( tools )
    , selectedObject_    ( controllers )
    , selectedAgent_     ( controllers )
    , selectedAutomat_   ( controllers )
    , selectedPopulation_( controllers )
    , selectedLine_      ( controllers )
    , selectedKnowledge_ ( controllers )
    , alpha_             ( 1 )
{
    InitializeColors();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy destructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
ColorStrategy::~ColorStrategy()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::BeforeSelection
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::BeforeSelection()
{
    selectedObject_ = 0; selectedAgent_ = 0; selectedAutomat_ = 0;
    selectedPopulation_ = 0; selectedLine_ = 0;
    selectedKnowledge_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const Agent_ABC& element )
{
    selectedAgent_   = &element;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const Automat_ABC& element )
{
    selectedAutomat_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const Object_ABC& element )
{
    selectedObject_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const Population_ABC& element )
{
    selectedPopulation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const kernel::TacticalLine_ABC& element )
{
    selectedLine_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const kernel::Knowledge_ABC& element )
{
    selectedKnowledge_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::AfterSelection
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ColorStrategy::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SetAlpha
// Created: AGE 2007-04-26
// -----------------------------------------------------------------------------
void ColorStrategy::SetAlpha( float alpha )
{
    alpha_ = alpha;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Agent_ABC& agent )
{
    QColor color = FindColor( agent );
    if( selectedAgent_ == &agent )
    {
        tools_.Select( true );
        color = SelectedColor( color );
    }
    else
    {
        const kernel::Entity_ABC* superior = agent.Get< CommunicationHierarchies >().GetSuperior();
        if(    ( superior && superior == selectedAutomat_ )
            || ( selectedAgent_ && selectedAgent_->Get< CommunicationHierarchies >().GetSuperior() == superior ) )
        {
            tools_.Select( false );
            color = SuperiorSelectedColor( color );
        }
        else
            tools_.Select( false );
    }
        
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const kernel::Automat_ABC& automat )
{
    QColor color = FindColor( automat );
    if( selectedAutomat_ == &automat )
        color = SelectedColor( color );
    tools_.Select( selectedAutomat_ == &automat );
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::FindColor
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
QColor ColorStrategy::FindColor( const Entity_ABC& entity )
{
    const Hierarchies* hierarchies = entity.Retrieve< TacticalHierarchies >();
    if( ! hierarchies )
        hierarchies = entity.Retrieve< CommunicationHierarchies >();
    const Entity_ABC& team = hierarchies ? hierarchies->GetTop() : entity;
    return FindTeamColor( team );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::FindColor
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
QColor ColorStrategy::FindColor( const kernel::Knowledge_ABC& knowledge )
{
    if( const kernel::Entity_ABC* realEntity = knowledge.GetRecognizedEntity() )
        return FindColor( *realEntity );
    return QColor( 255, 220, 000 );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Object_ABC& object )
{
    QColor color = FindColor( object );
    if( selectedObject_ == &object )
        color = SelectedColor( color );
    tools_.Select( selectedObject_ == &object );
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Population_ABC& population )
{
    QColor color = FindColor( population );
    if( selectedPopulation_ == &population )
        color = SelectedColor( color );
    tools_.Select( selectedPopulation_ == &population );
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Knowledge_ABC& knowledge )
{
    QColor color( FindColor( knowledge ) );
    if( selectedKnowledge_ == &knowledge )
        color = SelectedColor( color );
    color = KnowledgeColor( color );
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const kernel::TacticalLine_ABC& line )
{
    QColor color = FindColor( line );
    if( selectedLine_ == &line )
        color = SelectedColor( color );
    tools_.Select( selectedLine_ == &line );
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectedColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
QColor ColorStrategy::SelectedColor( const QColor& base ) const
{
    return base.light( 150 );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SuperiorSelectedColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
QColor ColorStrategy::SuperiorSelectedColor( const QColor& base ) const
{
    return base.light( 120 );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::KnowledgeColor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
QColor ColorStrategy::KnowledgeColor( const QColor& base ) const
{
    return base.dark( 120 );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::ApplyColor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void ColorStrategy::ApplyColor( const QColor& color ) const
{
    glColor4f( color.red()   / 255.f,
               color.green() / 255.f,
               color.blue()  / 255.f,
               alpha_ );
}

namespace 
{
    int RandomInt()
    {
        return 63 + rand() % 192;
    }
};

// -----------------------------------------------------------------------------
// Name: ColorStrategy::RandomColor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
QColor ColorStrategy::RandomColor() const
{
    return QColor( RandomInt(), RandomInt(), RandomInt() );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::NotifyCreated
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::NotifyCreated( const Team_ABC& team )
{
    FindTeamColor( team ); // $$$$ SBO 2007-02-22: should not be needed
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::NotifyUpdated
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ColorStrategy::NotifyUpdated( const Team_ABC& team )
{
    NotifyDeleted( team );
    NotifyCreated( team );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::NotifyDeleted
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::NotifyDeleted( const Team_ABC& team )
{
    T_TeamColors::iterator it = teamColors_.find( &team );
    if( it != teamColors_.end() )
    {
        if( it->second.second )
            it->second.second->push_back( it->second.first );
        teamColors_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::FindTeamColor
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
QColor ColorStrategy::FindTeamColor( const kernel::Entity_ABC& entity )
{
    T_TeamColors::const_iterator it = teamColors_.find( &entity );
    if( it != teamColors_.end() )
        return it->second.first;

    T_Colors* available = 0;
    const kernel::Team_ABC& team = static_cast< const kernel::Team_ABC& >( entity );
    if( team.IsFriend() )
        available = &friendlyAvailable_;
    else if( team.IsEnemy() )
        available = &enemyAvailable_;
    else /*if( team.IsNeutral() )*/
        available = &neutralAvailable_;

    if( available->empty() )
        teamColors_[ &entity ] = std::make_pair( RandomColor(), (T_Colors*)0 );
    else
    {
        teamColors_[ &entity ] = std::make_pair( available->back(), available );
        available->pop_back();
    }
    return teamColors_[ &entity ].first;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::InitializeColors
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::InitializeColors()
{
    enemyAvailable_.push_back( QColor( 170, 90, 40 ) );   // marron
    enemyAvailable_.push_back( QColor( 255, 150, 10 ) );  // orange
    enemyAvailable_.push_back( QColor( 255, 0, 255 ) ); // magenta
    enemyAvailable_.push_back( QColor( 255, 50, 50 ) );   // rouge  

    friendlyAvailable_.push_back( QColor( 170, 100, 60 ) ); // vert d'eau
    friendlyAvailable_.push_back( QColor( 50, 50, 128 ) ); // cobalt
    friendlyAvailable_.push_back( QColor( 0, 230, 230 ) ); // cyan
    friendlyAvailable_.push_back( QColor( 100, 125, 255 ) ); // bleu

    neutralAvailable_.push_back( QColor( 255, 255, 255 ) ); // blanc
    neutralAvailable_.push_back( QColor( 80, 80, 80 ) );  /// noir
    neutralAvailable_.push_back( QColor( 140, 140, 140 ) ); // gris
    neutralAvailable_.push_back( QColor( 0, 170, 0 ) ); // vert
}
