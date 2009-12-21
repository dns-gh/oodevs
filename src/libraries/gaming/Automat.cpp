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
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "Diplomacies.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const ASN1T_MsgAutomatCreation& message, Controller& controller, 
                  const tools::Resolver_ABC< AutomatType >& resolver )
    : EntityImplementation< Automat_ABC >( controller, message.oid, message.nom )
    , type_( resolver.Get( message.type_automate ) )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName().c_str() ).arg( message.oid );
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
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
const AutomatType& Automat::GetType() const
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
        tools.DrawApp6Symbol( symbol_, where, -2 );
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
    if( symbol_ == symbol )
        return;
    symbol_ = symbol;
    const Entity_ABC& team = hierarchies.GetTop();
    const Diplomacies_ABC* diplo = team.Retrieve< Diplomacies_ABC >();
    App6Symbol::SetKarma( symbol_, diplo ? diplo->GetKarma() : Karma::unknown_ );
}

// -----------------------------------------------------------------------------
// Name: Automat::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Automat::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
