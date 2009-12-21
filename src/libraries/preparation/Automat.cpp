// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Automat.h"
#include "IdManager.h"
#include "Tools.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const AutomatType& type, Controller& controller, IdManager& idManager )
    : EntityImplementation< Automat_ABC >( controller, idManager.GetNextId(), "" )
    , type_         ( &type )
{
    name_ = type.GetName().c_str() + QString( " [%1]" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
Automat::Automat( xistream& xis, Controller& controller, IdManager& idManager, const AgentTypes& agentTypes )
    : EntityImplementation< Automat_ABC >( controller, ReadId( xis ), ReadName( xis ) )
{
    std::string type;
    xis >> attribute( "type", type );
    type_ = &agentTypes.Resolver< AutomatType, std::string >::Get( type );
    RegisterSelf( *this );
    CreateDictionary( controller );
    idManager.Lock( id_ );
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
// Name: Automat::ReadId
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
unsigned long Automat::ReadId( xml::xistream& xis )
{
    int id;
    xis >> attribute( "id", id );
    return id;
}

// -----------------------------------------------------------------------------
// Name: Automat::ReadName
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
QString Automat::ReadName( xml::xistream& xis )
{
    std::string name;
    xis >> attribute( "name", name );
    return name.c_str();
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
const AutomatType& Automat::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: Automat::Rename
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Automat::Rename( const QString& name )
{
    name_ = name;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Automat::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
    {
        InitializeSymbol();
        tools.DrawApp6Symbol( symbol_, where, 2 );
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
    kernel::App6Symbol::SetKarma( symbol_, hierarchies.GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
}

// -----------------------------------------------------------------------------
// Name: Automat::CreateDictionary
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void Automat::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Automat", "Info/Identifier" ), (const unsigned long)id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Automat", "Info/Name" ), name_ );
}

// -----------------------------------------------------------------------------
// Name: Automat::SerializeAttributes
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void Automat::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "name", name_.ascii() )
        << attribute( "type", type_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: Automat::SerializeLogistics
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void Automat::SerializeLogistics( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) );
}

// -----------------------------------------------------------------------------
// Name: Automat::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Automat::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Automat::Register( kernel::Automat_ABC& automat )
{
    throw std::exception( __FUNCTION__ " not implemented" );  
}
// -----------------------------------------------------------------------------
// Name: Automat::Remove
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Automat::Remove( kernel::Automat_ABC& automat )
{
    throw std::exception( __FUNCTION__ " not implemented" );  
}
// -----------------------------------------------------------------------------
// Name: Automat::GetAutomats
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
const tools::Resolver< kernel::Automat_ABC >& Automat::GetAutomats() const
{
    throw std::exception( __FUNCTION__ " not implemented" );  
}
// -----------------------------------------------------------------------------
// Name: Automat::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Automat::Register( kernel::Agent_ABC& automat )
{
    throw std::exception( __FUNCTION__ " not implemented" );  
}
// -----------------------------------------------------------------------------
// Name: Automat::Remove
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Automat::Remove( kernel::Agent_ABC& automat )
{
    throw std::exception( __FUNCTION__ " not implemented" );  
}
// -----------------------------------------------------------------------------
// Name: Automat::GetAgents
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
const tools::Resolver< kernel::Agent_ABC >& Automat::GetAgents() const
{
    throw std::exception( __FUNCTION__ " not implemented" );  
}

