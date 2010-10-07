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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DictionaryEntryType constructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
DictionaryEntryType::DictionaryEntryType( xml::xistream& xis )
    : name_( xis.attribute< std::string >( "name" ) )
    , id_  ( xis.attribute< unsigned int >( "id" ) )
{
    if( xis.has_attribute( "alias" ) )
        alias_ = xis.attribute< std::string >( "alias" );
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
const std::string& DictionaryEntryType::GetLabel( const std::string& kind, const std::string& language ) const
{
    if( kind.empty() )
        return name_;
    for( CIT_Labels it = labels_.begin(); it != labels_.end(); ++it )
        if( ( *it )->Matches( language, kind ) )
            return ( *it )->GetText();
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DictionaryEntryType::GetId
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
unsigned int DictionaryEntryType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: DictionaryEntryType::ReadLabel
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void DictionaryEntryType::ReadLabel( xml::xistream& xis )
{
    labels_.push_back( new EntryLabelType( xis ) );
}
