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
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SymbolRule constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolRule::SymbolRule( xml::xistream& xis )
{
    xis >> xml::attribute( "name", name_ );
    xis >> xml::list( "case", *this, & SymbolRule::ReadCase );
}

// -----------------------------------------------------------------------------
// Name: SymbolRule destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolRule::~SymbolRule()
{
    for( auto it = cases_.begin(); it != cases_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: SymbolRule::ReadCase
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void SymbolRule::ReadCase( xml::xistream& xis )
{
    std::string name;
    xis >> xml::attribute( "name", name );
    SymbolCase*& sCase = cases_[ name ];
    if( sCase )
        throw std::runtime_error( "Case '" + name + "' already regsitered" );
    sCase = new SymbolCase( xis );
}

// -----------------------------------------------------------------------------
// Name: SymbolRule::GetCases
// Created: RPD 2011-01-26
// -----------------------------------------------------------------------------
const SymbolRule::T_Cases& SymbolRule::GetCases() const
{
    return cases_;
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
// Name: SymbolRule::ConvertToNature
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
std::string SymbolRule::ConvertToNature( std::string symbol ) const
{
    std::string result = "";
    if( symbol.find( "symbols/" ) != std::string::npos )
        symbol.erase( 0, 8 ); // remove symbols/
    symbol.erase( 0, 5 ); // remove s*gpu
    InternalConvertToNature( symbol, result );
    result.erase( result.size() - 1, 1 ); // remove last /
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolRule::InternalConvertToNature
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
void SymbolRule::InternalConvertToNature( std::string& symbol, std::string& result ) const
{
    for( auto it = cases_.begin(); !symbol.empty() && it != cases_.end(); ++it )
        if( it->second->GetValue() == symbol.substr( 0, 1 ) )
        {
            result = result + it->first + '/';
            symbol.erase( 0, 1 ); // remove first symbol
            if( it->second->GetRule() != 0 )
                it->second->GetRule()->InternalConvertToNature( symbol, result );
            break;
        }
}

// -----------------------------------------------------------------------------
// Name: SymbolRule::Accept
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void SymbolRule::Accept( SymbolVisitor_ABC& visitor ) const
{
    visitor.StartCategory( name_ );
    for( auto it = cases_.begin(); it != cases_.end(); ++it )
        it->second->Accept( visitor );
    visitor.EndCategory();
}