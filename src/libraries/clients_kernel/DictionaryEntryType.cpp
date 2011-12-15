// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DictionaryEntryType.h"
#include "EntryLabelType.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DictionaryEntryType constructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
DictionaryEntryType::DictionaryEntryType( xml::xistream& xis )
    : key_      ( xis.attribute< std::string >( "name" ) )
    , id_       ( xis.attribute< unsigned int >( "id" ) )
    , alias_    ( xis.attribute< std::string >( "alias", std::string() ) )
{
    std::string options = xis.attribute< std::string >( "option", std::string() );
    if( options != "" )
        boost::algorithm::split( options_, options, boost::algorithm::is_any_of( ";" ) );
    xis >> xml::optional >> xml::start( "labels" )
                           >> xml::list( "label", *this, &DictionaryEntryType::ReadLabel )
                         >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DictionaryEntryType destructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
DictionaryEntryType::~DictionaryEntryType()
{
    for( CIT_Labels it = labels_.begin(); it != labels_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: DictionaryEntryType::GetLabel
// Created: JSR 2010-10-05
// -----------------------------------------------------------------------------
std::string DictionaryEntryType::GetLabel( const std::string& kind, const std::string& language ) const
{
    if( kind.empty() )
        return key_;
    for( CIT_Labels it = labels_.begin(); it != labels_.end(); ++it )
        if( ( *it )->Matches( language, kind ) )
            return ( *it )->GetText();
    return key_;
}

// -----------------------------------------------------------------------------
// Name: DictionaryEntryType::GetKey
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
std::string DictionaryEntryType::GetKey() const
{
    return key_;
}

// -----------------------------------------------------------------------------
// Name: DictionaryEntryType::GetId
// Created: RPD 2010-12-29
// -----------------------------------------------------------------------------
unsigned int DictionaryEntryType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: DictionaryEntryType::GetOptions
// Created: LGY 2011-12-15
// -----------------------------------------------------------------------------
const std::set< std::string >& DictionaryEntryType::GetOptions() const
{
    return options_;
}

// Name: DictionaryEntryType::GetAlias
// Created: JSR 2011-09-13
// -----------------------------------------------------------------------------
std::string DictionaryEntryType::GetAlias() const
{
    if( alias_.IsSet() )
        return alias_;
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: DictionaryEntryType::ReadLabel
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void DictionaryEntryType::ReadLabel( xml::xistream& xis )
{
    labels_.push_back( new EntryLabelType( xis ) );
}
