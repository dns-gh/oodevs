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
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_kernel/App6Symbol.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
Intelligence::Intelligence( Controller& controller, IdManager& idManager, const std::string& symbol, const HierarchyLevel_ABC& level, bool mounted, const Karma& karma )
    : EntityImplementation< Intelligence_ABC >( controller, idManager.GetNextId(), "" )
    , controller_( controller )
    , symbol_    ( symbol )
    , level_     ( &level )
    , mounted_   ( mounted )
    , karma_     ( &karma )
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
    : EntityImplementation< Intelligence_ABC >( controller, idManager.GetNextId(), attribute< std::string >( xis, "name" ).c_str() )
    , controller_( controller )
    , symbol_    ( attribute< std::string >( xis, "nature" ) )
    , level_     ( &levels.Get( attribute< std::string >( xis, "level" ).c_str() ) )
    , mounted_   ( attribute< bool >( xis, "embarked" ) ) // $$$$ AGE 2008-01-16: odb : embarked => mounted
    , karma_     ( &Karma::ResolveId( attribute< std::string >( xis, "karma" ) ) )
{
    RegisterSelf( *this );
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
        const std::string style( "stroke:black;fill:currentColor;stroke-width:4;stroke-dasharray:10,10px" );
        tools.DrawApp6Symbol( GetSymbol(), style, where );
        tools.DrawApp6Symbol( level_->GetSymbol(), style, where );
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence::DoUpdate
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
void Intelligence::DoUpdate( const kernel::InstanciationComplete& /*ic*/ )
{
    SetKarma( karma_ );
}

namespace
{
    const Karma& ComputeKarma( const Karma& karma, const Entity_ABC& parent )
    {
        const Karma& teamKarma = parent.Get< IntelligenceHierarchies >().GetTop().Get< Diplomacies_ABC >().GetKarma();
        if( karma == Karma::friend_ )
            return teamKarma;
        if( karma == Karma::enemy_ )
            return !teamKarma;
        return karma;
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence::GetKarma
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
const kernel::Karma& Intelligence::GetKarma() const
{
    return ComputeKarma( *karma_, *this );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::GetSymbol
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
std::string Intelligence::GetSymbol() const
{
    std::string symbol = symbol_;
    App6Symbol::SetKarma( symbol, GetKarma() );
    return symbol;
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
// Created: SBO 2007-11-02
// -----------------------------------------------------------------------------
bool Intelligence::IsMounted() const
{
    return mounted_;
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SerializeIntelligences
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void Intelligence::SerializeIntelligences( xml::xostream& xos ) const
{
    xos << attribute( "name", GetName().ascii() )
        << attribute( "karma", karma_->GetId() )
        << attribute( "level", level_->GetName() )
        << attribute( "embarked", mounted_ ) // $$$$ AGE 2008-01-16: odb
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
    dictionary.Register( *(      Entity_ABC*)this, tools::translate( "Intelligence", "Info/Mounted" ), mounted_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Intelligence", "Info/Level" ), level_ );
    dictionary.Register( *(      Entity_ABC*)this, tools::translate( "Intelligence", "Info/Karma" ), karma_, *this, &Intelligence::SetKarma );
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

// -----------------------------------------------------------------------------
// Name: Intelligence::SetKarma
// Created: SBO 2007-11-02
// -----------------------------------------------------------------------------
void Intelligence::SetKarma( const IntelligenceKarma& karma )
{
    karma_ = karma;
    Touch();
    Get< IntelligenceHierarchies >().UpdateSymbol( false );
}
