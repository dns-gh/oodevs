// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Intelligence.h"
#include "IdManager.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
Intelligence::Intelligence( Controller& controller, IdManager& idManager, const std::string& symbol, const HierarchyLevel_ABC& level, bool embarked, const Karma& karma )
    : EntityImplementation< Intelligence_ABC >( controller, idManager.GetNextId(), "" )
    , symbol_   ( symbol )
    , level_    ( level )
    , embarked_ ( embarked )
    , karma_    ( karma )
{
    RegisterSelf( *this );
    name_ = tools::translate( "Intelligence", "Intelligence [%1]" ).arg( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
Intelligence::Intelligence( kernel::Controller& controller, IdManager& idManager, xml::xistream& xis, const Resolver_ABC< HierarchyLevel_ABC, QString >& levels )
    : EntityImplementation< Intelligence_ABC >( controller, attribute< int >( xis, "id" ), attribute< std::string >( xis, "name" ).c_str() )
    , symbol_   ( attribute< std::string >( xis, "nature" ) )
    , level_    ( levels.Get( attribute< std::string >( xis, "level" ).c_str() ) )
    , embarked_ ( attribute< bool >( xis, "embarked" ) )
    , karma_    ( Karma::ResolveId( attribute< std::string >( xis, "karma" ) ) )
{
    RegisterSelf( *this );
    idManager.Lock( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Intelligence destructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
Intelligence::~Intelligence()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Draw
// Created: SBO 2007-10-15
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
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
const kernel::Karma& Intelligence::GetKarma() const
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
const HierarchyLevel_ABC& Intelligence::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SerializeIntelligences
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void Intelligence::SerializeIntelligences( xml::xostream& xos ) const
{
    xos << attribute( "id", id_ )
        << attribute( "name", GetName().ascii() )
        << attribute( "karma", karma_.GetId() )
        << attribute( "level", level_.GetName() )
        << attribute( "embarked", embarked_ )
        << attribute( "nature" , symbol_ );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::CreateDictionary
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void Intelligence::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Identifier" ), (const unsigned long)id_ );
    dictionary.Register( *(      Entity_ABC*)this, tools::translate( "Intelligence", "Info/Name" ) , name_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Nature" ) , symbol_ );
    dictionary.Register( *(      Entity_ABC*)this, tools::translate( "Intelligence", "Info/Embarked" ), embarked_ );
//    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Level" ), level_ );
    dictionary.Register( *(      Entity_ABC*)this, tools::translate( "Intelligence", "Info/Karma" ), karma_ );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Delete
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void Intelligence::Delete()
{
    // $$$$ SBO 2007-10-19: delete this; ?
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Rename
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void Intelligence::Rename( const QString& name )
{
    name_ = name;
    Touch();
}
