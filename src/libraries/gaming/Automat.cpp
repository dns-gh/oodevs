// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Automat.h"
#include "Diplomacies.h"
#include "Tools.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const sword::AutomatCreation& message, Controller& controller, const tools::Resolver_ABC< kernel::AutomatType >& resolver )
    : EntityImplementation< Automat_ABC >( controller, message.automat().id(), QString( message.name().c_str() ) )
    , type_( resolver.Get( message.type().id() ) )
    , logisticLevel_ ( &kernel::LogisticLevel::Resolve( message.logistic_level() ) )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName().c_str() ).arg( message.automat().id() );
    RegisterSelf( *this );
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Automat::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void Automat::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "Automat", "Info/Identifier" ), id_ );
    dico.Register( *this, tools::translate( "Automat", "Info/Name" ), name_ );
    if(type_.IsTC2())
        dico.Register( *this, tools::translate( "Automat", "Info/LogisticLevel" ), *logisticLevel_ );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
const kernel::AutomatType& Automat::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Automat::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
    {
        InitializeSymbol();
        tools.DrawApp6Symbol( symbol_, where, 2 );
        tools.DrawApp6Symbol( level_, where, 2 );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::InitializeSymbol
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void Automat::InitializeSymbol() const
{
    const kernel::TacticalHierarchies& hierarchies = Get< kernel::TacticalHierarchies >();
    const std::string symbol = hierarchies.GetSymbol();
    const std::string level = hierarchies.GetLevel();
    if( symbol_ == symbol && level_ == level )
        return;
    symbol_ = symbol;
    level_ = level;
    const Entity_ABC& team = hierarchies.GetTop();
    const Diplomacies_ABC* diplo = team.Retrieve< Diplomacies_ABC >();
    App6Symbol::SetKarma( symbol_, diplo ? diplo->GetKarma() : Karma::unknown_ );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetLogisticLevel
// Created: AHC 2010-10-08
// -------------------------------------------------------------------------------
const kernel::LogisticLevel& Automat::GetLogisticLevel() const
{
    return *logisticLevel_;
}
