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
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
Intelligence::Intelligence( Controller& controller, IdManager& idManager, const std::string& symbol, const std::string& level, const Karma& karma )
    : EntityImplementation< Intelligence_ABC >( controller, idManager.GetNextId(), symbol.c_str() ) // $$$$ SBO 2007-10-12: name ?
    , symbol_( symbol )
    , level_( level )
    , karma_( karma )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

namespace
{
    template< typename T >
    T ReadAttribute( xml::xistream& xis, const std::string& name )
    {
        T value;
        xis >> attribute( name, value );
        return value;
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
Intelligence::Intelligence( kernel::Controller& controller, IdManager& idManager, xml::xistream& xis )
    : EntityImplementation< Intelligence_ABC >( controller, ReadAttribute< int >( xis, "id" ), ReadAttribute< std::string >( xis, "name" ).c_str() )
    , symbol_( ReadAttribute< std::string >( xis, "type" ) )
    , level_ ( ReadAttribute< std::string >( xis, "level" ) )
    , karma_ ( Karma::ResolveId( ReadAttribute< std::string >( xis, "karma" ) ) )
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
        tools.DrawApp6Symbol( level_ , where );
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
std::string Intelligence::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SerializeAttributes
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void Intelligence::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "id", id_ )
        << attribute( "name", GetName().ascii() )
        << attribute( "karma", karma_.GetId() )
        << attribute( "level", level_ )
        << attribute( "type" , symbol_ );
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
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Name" ) , name_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Type" ) , symbol_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Level" ), level_ );
    dictionary.Register( *(      Entity_ABC*)this, tools::translate( "Intelligence", "Info/Karma" ), karma_ );
}
