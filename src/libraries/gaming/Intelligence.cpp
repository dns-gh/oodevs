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
#include "clients_kernel/TacticalHierarchies.h"
#include "Tools.h"

using namespace kernel;

namespace
{
    const Karma& ComputeFriendship( const Karma& team, const ASN1T_EnumDiplomacy& diplomacy )
    {
        switch( diplomacy )
        {
        case EnumDiplomacy::ami:
            return team;
        case EnumDiplomacy::ennemi:
            return !team;
        case EnumDiplomacy::neutre:
            return Karma::neutral_;
        }
        return Karma::unknown_;
    }

    const Karma& GetKarma( const ASN1T_IntelligenceDiffusion& asn, const Resolver_ABC< Formation_ABC >& formations )
    {
        const Hierarchies* hierarchies = 0;
        switch( T_IntelligenceDiffusion_formation )
        {
        default:
            hierarchies = &formations.Get( asn.u.formation ).Get< kernel::TacticalHierarchies >();
        }
        return hierarchies->GetTop().Get< Diplomacies >().GetKarma();
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
Intelligence::Intelligence( const ASN1T_MsgIntelligenceCreation& message, Controller& controller, const Resolver_ABC< Formation_ABC >& formations, const Resolver_ABC< HierarchyLevel_ABC >& levels )
    : EntityImplementation< Intelligence_ABC >( controller, message.oid, message.intelligence.name )
    , symbol_( message.intelligence.nature )
    , level_ ( levels.Get( message.intelligence.level ) )
    , karma_ ( ComputeFriendship( ::GetKarma( message.intelligence.diffusion, formations ), message.intelligence.diplomacy ) )
    , embarked_( message.intelligence.embarked )
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
        tools.DrawApp6Symbol( level_.GetSymbol(), where );
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence::GetKarma
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
const Karma& Intelligence::GetKarma() const
{
    return karma_;
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
const kernel::HierarchyLevel_ABC& Intelligence::GetLevel() const
{
    return level_;
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
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Name" ) , name_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Type" ) , symbol_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Embarked" ) , embarked_ );
//    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Level" ), level_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Karma" ), karma_ );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Delete
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void Intelligence::Delete()
{
    // $$$$ SBO 2007-10-19: ASN_DeleteFusse
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Rename
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void Intelligence::Rename( const QString& name )
{
    // $$$$ SBO 2007-10-19: ASN_UpdateFusse
}
