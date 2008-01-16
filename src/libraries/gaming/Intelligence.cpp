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
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "ASN_Messages.h"
#include "Tools.h"

using namespace kernel;

namespace
{
    const Karma& ConvertToKarma( const ASN1T_EnumDiplomacy& diplomacy )
    {
        switch( diplomacy )
        {
        case EnumDiplomacy::ami:
            return Karma::friend_;
        case EnumDiplomacy::ennemi:
            return Karma::enemy_;
        case EnumDiplomacy::neutre:
            return Karma::neutral_;
        }
        return Karma::unknown_;
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
Intelligence::Intelligence( const ASN1T_MsgIntelligenceCreation& message, Controller& controller, const Resolver_ABC< Formation_ABC >& formations, const Resolver_ABC< HierarchyLevel_ABC >& levels, Publisher_ABC& publisher )
    : EntityImplementation< Intelligence_ABC >( controller, message.oid, message.intelligence.name )
    , levels_   ( levels )
    , formation_( formations.Get( message.intelligence.formation ) )
    , symbol_   ( message.intelligence.nature )
    , level_    ( &levels_.Get( message.intelligence.level ) )
    , karma_    ( &ConvertToKarma( message.intelligence.diplomacy ) )
    , mounted_  ( message.intelligence.embarked )
    , publisher_( publisher )
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
        tools.DrawApp6Symbol( symbol_, where );
        tools.DrawApp6Symbol( level_->GetSymbol(), where );
    }
}

namespace
{
    const Karma& ComputeKarma( const Karma& karma, const Entity_ABC& parent )
    {
        const Karma& parentKarma = parent.Get< IntelligenceHierarchies >().GetTop().Get< Diplomacies >().GetKarma();
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
    ASN_MsgIntelligenceDestructionRequest message;
    message().oid = id_;
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Rename
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void Intelligence::Rename( const QString& name )
{
    ASN_MsgIntelligenceUpdateRequest message;
    message().oid = id_;
    message().m.namePresent = 1;
    message().name = name.ascii();
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::DoUpdate
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Intelligence::DoUpdate( const ASN1T_MsgIntelligenceUpdate& message )
{
    if( message.m.namePresent )
        name_ = message.name;
    if( message.m.naturePresent )
        symbol_ = message.nature;
    if( message.m.levelPresent )
        level_ = &levels_.Get( message.level );
    if( message.m.diplomacyPresent )
        karma_ = &ConvertToKarma( message.diplomacy );
    if( message.m.embarkedPresent )
        mounted_ = message.embarked ? true : false;
//    if( message.m.formationPresent ) // $$$$ SBO 2007-10-23: someday if needed maybe...
//        formation_ = formations_.Find( message.formation );
}
