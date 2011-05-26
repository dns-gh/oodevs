// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Intelligence.h"
#include "Diplomacies.h"
#include "Tools.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/Styles.h"
#include "protocol/MessengerSenders.h"

using namespace kernel;

namespace
{
    const Karma& ConvertToKarma( const sword::EnumDiplomacy& diplomacy )
    {
        switch( diplomacy )
        {
        case sword::friendly:
            return Karma::friend_;
        case sword::enemy:
            return Karma::enemy_;
        case sword::neutral:
            return Karma::neutral_;
        default:
            return Karma::unknown_;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
Intelligence::Intelligence( const sword::IntelligenceCreation& message, Controller& controller, const tools::Resolver_ABC< Formation_ABC >& formations, const tools::Resolver_ABC< HierarchyLevel_ABC >& levels, Publisher_ABC& publisher, kernel::SymbolFactory& factory )
    : EntityImplementation< Intelligence_ABC >( controller, message.id().id(), QString( message.intelligence().name().c_str() ) )
    , levels_     ( levels )
    , formation_  ( formations.Get( message.intelligence().formation().id() ) )
    , symbol_     ( message.intelligence().nature() )
    , level_      ( &levels_.Get( message.intelligence().level() ) )
    , karma_      ( &ConvertToKarma( message.intelligence().diplomacy() ) )
    , mounted_    ( message.intelligence().embarked() != 0 )
    , publisher_  ( publisher )
    , levelSymbol_( factory.CreateLevelSymbol( *level_ ) )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Intelligence destructor
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
Intelligence::~Intelligence()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Draw
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
void Intelligence::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
    {
        const std::string style( "stroke:black;fill:currentColor;stroke-width:4;stroke-dasharray:10,10px" );
        tools.DrawApp6Symbol( symbol_, style, where, -1 );
        tools.DrawApp6Symbol( levelSymbol_, style, where, -1 );
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence::DisplayInTooltip
// Created: AGE 2008-01-21
// -----------------------------------------------------------------------------
void Intelligence::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( "" ).Start( Styles::bold )
                .Add( (Intelligence_ABC*)this )
             .End();
    DisplayInSummary( displayer );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::DisplayInSummary
// Created: AGE 2008-01-21
// -----------------------------------------------------------------------------
void Intelligence::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Intelligence", "Level:" ), level_ )
             .Display( tools::translate( "Intelligence", "Karma:" ), karma_ )
             .Display( tools::translate( "Intelligence", "Mounted:" ), mounted_ );
}

namespace
{
    const Karma& ComputeKarma( const Karma& karma, const Entity_ABC& parent )
    {
        const Karma& parentKarma = parent.Get< IntelligenceHierarchies >().GetTop().Get< Diplomacies_ABC >().GetKarma();
        if( karma == Karma::friend_ )
            return parentKarma;
        if( karma == Karma::enemy_ )
            return !parentKarma;
        return karma;
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence::GetKarma
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
const Karma& Intelligence::GetKarma() const
{
    return ComputeKarma( *karma_, formation_ );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::GetSymbol
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
std::string Intelligence::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: Intelligence::GetLevel
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC& Intelligence::GetLevel() const
{
    return *level_;
}

// -----------------------------------------------------------------------------
// Name: Intelligence::IsMounted
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
bool Intelligence::IsMounted() const
{
    return mounted_;
}

// -----------------------------------------------------------------------------
// Name: Intelligence::CreateDictionary
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
void Intelligence::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Identifier" ), (const unsigned long)id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Name" ), name_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Type" ), symbol_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Mounted" ), mounted_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Level" ), level_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Karma" ), karma_ );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Delete
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void Intelligence::Delete()
{
    plugins::messenger::IntelligenceDestructionRequest message;
    message().mutable_id()->set_id( id_ );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Rename
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void Intelligence::Rename( const QString& name )
{
    plugins::messenger::IntelligenceUpdateRequest message;
    message().mutable_intelligence()->set_id( id_ );
    message().set_name( name.ascii() );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::DoUpdate
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Intelligence::DoUpdate( const sword::IntelligenceUpdate& message )
{
    if( message.has_name()  )
        name_ = message.name().c_str();
    if( message.has_nature()  )
        symbol_ = message.nature();
    if( message.has_level()  )
        level_ = &levels_.Get( message.level() );
    if( message.has_diplomacy()  )
        karma_ = &ConvertToKarma( message.diplomacy() );
    if( message.has_embarked()  )
        mounted_ = message.embarked() ? true : false;
//    if( message.has_formation()  ) // $$$$ SBO 2007-10-23: someday if needed maybe...
//        formation_ = formations_.Find( message.formation );
}
