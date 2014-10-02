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

#include "ColorModifier_ABC.h"
#include "GlTools_ABC.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanColor_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ColorStrategy constructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
ColorStrategy::ColorStrategy( Controllers& controllers, GlTools_ABC& tools, ColorModifier_ABC& colorController )
    : controllers_    ( controllers )
    , tools_          ( tools )
    , colorController_( colorController )
    , alpha_          ( 1 )
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
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Add
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
void ColorStrategy::Add( std::unique_ptr< ColorModifier_ABC > modifier )
{
    modifiers_.push_back( modifier.release() );
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
// Name: ColorStrategy::ApplyModifiers
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
QColor ColorStrategy::ApplyModifiers( const kernel::Entity_ABC& entity, const QColor& color )
{
    size_t size = modifiers_.size();
    QColor result = color;
    for( size_t i = 0; i < size; ++i )
        result = modifiers_[i].Apply( entity, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::ApplyModifiers
// Created: LGY 2013-03-06
// -----------------------------------------------------------------------------
float ColorStrategy::ApplyModifiers( const kernel::Entity_ABC& entity, float alpha )
{
    size_t size = modifiers_.size();
    float result = alpha;
    for( size_t i = 0; i < size; ++i )
        result = modifiers_[i].Apply( entity, result );
    return result;
}

// ---------------------------------------------------------------------------
// Name: ColorStrategy::Process
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void ColorStrategy::Process( const Entity_ABC& entity )
{
    ApplyColor( ApplyModifiers( entity, FindColor( entity ) ) );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Process
// Created: LGY 2011-06-29
// -----------------------------------------------------------------------------
void ColorStrategy::Process( const kernel::Entity_ABC& entity, QColor color )
{
    ApplyColor( ApplyModifiers( entity, color ) );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Agent_ABC& agent )
{
    Process( agent );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Automat_ABC& automat )
{
    Process( automat );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Formation_ABC& formation )
{
    Process( formation );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const kernel::Ghost_ABC& ghost )
{
    Process( ghost );
}

void ColorStrategy::SelectColor( const kernel::Pathfind_ABC& pathfind )
{
    ApplyColor( ApplyModifiers( pathfind, QColor( "#00B3FD" ) ) );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::FindBaseColor
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
QColor ColorStrategy::FindBaseColor( const kernel::Entity_ABC& entity )
{
    const Hierarchies* hierarchies = entity.Retrieve< TacticalHierarchies >();
    if( ! hierarchies )
        hierarchies = entity.Retrieve< CommunicationHierarchies >();
    const Entity_ABC& team = hierarchies ? hierarchies->GetTop() : entity;
    if( dynamic_cast< const kernel::Team_ABC* >( &team ) ) // $$$$ SBO 2008-10-03: depends on extensions polishing order...
        return FindTeamColor( team );
    return Qt::black;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::FindColor
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
QColor ColorStrategy::FindColor( const Entity_ABC& entity )
{
    return colorController_.Apply( entity, FindBaseColor( entity ) );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::FindColorWithModifiers
// Created: ABR 2013-01-29
// -----------------------------------------------------------------------------
QColor ColorStrategy::FindColorWithModifiers( const kernel::Entity_ABC& entity )
{
    return ApplyModifiers( entity, FindColor( entity ) );
}

namespace
{
    QColor GetKarmaColor( const Karma& karma )
    {
        if( karma == Karma::friend_ )
            return QColor( 100, 125, 255 );
        if( karma == Karma::enemy_ )
            return QColor( 255, 50, 50 );
        if( karma == Karma::neutral_ )
            return QColor( 0, 170, 0 );
        return QColor( 255, 220, 000 );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::FindColor
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
QColor ColorStrategy::FindColor( const Knowledge_ABC& knowledge )
{
    if( const Team_ABC* team = knowledge.GetTeam() )
    {
        if( const Entity_ABC* entity = knowledge.GetEntity() )
            return FindColor( *entity );
        return FindColor( *team );
    }
    return QColor( 255, 220, 000 );
}

namespace
{
    QColor FindSuperiorColor( const kernel::Entity_ABC& entity, ColorModifier_ABC& colorController, QColor color )
    {
        if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
            if( const kernel::Entity_ABC* superior = hierarchies->GetSuperior() )
                return colorController.Apply( *superior, color );
        return color;
    }
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Object_ABC& object )
{
    Process( object );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Population_ABC& population )
{
    QColor color = FindSuperiorColor( population, colorController_, FindBaseColor( population ) );
    Process( population, colorController_.Apply( population, color ) );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Inhabitant_ABC& inhabitant )
{
    QColor base = FindSuperiorColor( inhabitant, colorController_, FindBaseColor( inhabitant ) );
    QColor color = ApplyModifiers( inhabitant, base );
    if( base == color )
        ApplyColor( color, 0 );
    else
        ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Knowledge_ABC& knowledge )
{
    ApplyColor( KnowledgeColor( ApplyModifiers( knowledge, FindColor( knowledge ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const TacticalLine_ABC& line )
{
    ApplyColor( ApplyModifiers( line, FindSuperiorColor( line, colorController_, FindBaseColor( line ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const kernel::Team_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const kernel::Drawing_ABC& drawing )
{
    ApplyColor( drawing.GetColor() );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const UrbanObject_ABC& object )
{
    if( const kernel::UrbanColor_ABC* attribute = object.Retrieve< kernel::UrbanColor_ABC >() )
    {
        const float alpha = ApplyModifiers( object, attribute->Alpha() );
        ApplyColor( QColor( attribute->Red(), attribute->Green(), attribute->Blue() ), alpha_ * alpha );
    }
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
    tools_.SetCurrentColor( color.red()   / 255.f,
                            color.green() / 255.f,
                            color.blue()  / 255.f,
                            alpha_ );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::ApplyColor
// Created: LGY 2011-01-04
// -----------------------------------------------------------------------------
void ColorStrategy::ApplyColor( const QColor& color, float alpha ) const
{
    tools_.SetCurrentColor( color.red()   / 255.f,
                            color.green() / 255.f,
                            color.blue()  / 255.f,
                            alpha );
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
    auto it = teamColors_.find( &team );
    if( it == teamColors_.end() )
        return;
    if( it->second.second )
        it->second.second->push_back( it->second.first );
    teamColors_.erase( it );
    if( teamColors_.empty() )
        InitializeColors();
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::FindTeamColor
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
QColor ColorStrategy::FindTeamColor( const Entity_ABC& entity )
{
    auto it = teamColors_.find( &entity );
    if( it != teamColors_.end() )
        return it->second.first;

    T_Colors* available = 0;
    const Karma& karma = entity.Get< Diplomacies_ABC >().GetKarma();
    if( karma == Karma::friend_ )
        available = &friendlyAvailable_;
    else if( karma == Karma::enemy_ )
        available = &enemyAvailable_;
    else /*if( karma == Karma::neutral_ )*/
        available = &neutralAvailable_;

    if( available->empty() )
        teamColors_[ &entity ] = std::make_pair( RandomColor(), nullptr );
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
    enemyAvailable_.clear();
    enemyAvailable_.push_back( QColor( 240, 60, 150 ) );   // framboise
    enemyAvailable_.push_back( QColor( 170, 90, 40 ) );   // marron
    enemyAvailable_.push_back( QColor( 255, 150, 10 ) );  // orange
    enemyAvailable_.push_back( QColor( 255, 0, 255 ) ); // magenta
    enemyAvailable_.push_back( QColor( 255, 50, 50 ) );   // rouge

    friendlyAvailable_.clear();
    friendlyAvailable_.push_back( QColor( 60, 100, 170 ) ); // bleu metal 2
    friendlyAvailable_.push_back( QColor( 80, 80, 240 ) ); // cobalt
    friendlyAvailable_.push_back( QColor( 110, 230, 230 ) ); // cyan
    friendlyAvailable_.push_back( QColor( 75, 190, 220 ) ); // bleu metal
    friendlyAvailable_.push_back( QColor( 100, 125, 255 ) ); // bleu

    neutralAvailable_.clear();
    neutralAvailable_.push_back( QColor( 255, 255, 255 ) ); // blanc
    neutralAvailable_.push_back( QColor( 80, 80, 80 ) );  /// noir
    neutralAvailable_.push_back( QColor( 140, 140, 140 ) ); // gris
    neutralAvailable_.push_back( QColor( 60, 240, 95 ) ); // vert flashy
    neutralAvailable_.push_back( QColor( 0, 170, 0 ) ); // vert
}
