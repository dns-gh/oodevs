// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "SymbolRule.h"
#include "SymbolCase.h"
#include "SymbolVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: SymbolRule constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolRule::SymbolRule( xml::xistream& xis )
{
    xis >> attribute( "name", name_ );
    xis >> list( "case", *this, & SymbolRule::ReadCase );
}
    
// -----------------------------------------------------------------------------
// Name: SymbolRule destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolRule::~SymbolRule()
{
    for( CIT_Cases it = cases_.begin(); it != cases_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: SymbolRule::ReadCase
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void SymbolRule::ReadCase( xml::xistream& xis )
{
    std::string name;
    xis >> attribute( "name", name );
    SymbolCase*& sCase = cases_[ name ];
    if( sCase )
        throw std::runtime_error( "Case '" + name + "' already regsitered" );
    sCase = new SymbolCase( xis );
}

// -----------------------------------------------------------------------------
// Name: SymbolRule::Evaluate
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void SymbolRule::Evaluate( const std::string& request, std::string& result ) const
{
    const std::string::size_type pos = request.find_first_of( '/' );

    std::string head = request;
    std::string tail;
    if( pos != std::string::npos )
    {
       head = request.substr( 0, pos );
       tail = request.substr( pos + 1, request.length() - pos  - 1 );
    }
    CIT_Cases it = cases_.find( head );
    if( it != cases_.end() )
        it->second->Evaluate( tail, result );
}

// -----------------------------------------------------------------------------
// Name: SymbolRule::Accept
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void SymbolRule::Accept( SymbolVisitor_ABC& visitor ) const
{
    visitor.StartCategory( name_ );
    for( CIT_Cases it = cases_.begin(); it != cases_.end(); ++it )
        it->second->Accept( visitor );
}
