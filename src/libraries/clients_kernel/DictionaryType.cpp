// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DictionaryType.h"
#include "DictionaryEntryType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DictionaryType constructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
DictionaryType::DictionaryType( xml::xistream& xis )
{
    if( xis.has_attribute( "alias" ) )
        alias_ = xis.attribute< std::string >( "alias" );
    xis >> xml::list( "entry", *this, &DictionaryType::ReadEntry );
}
    
// -----------------------------------------------------------------------------
// Name: DictionaryType destructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
DictionaryType::~DictionaryType()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: DictionaryType::GetStringList
// Created: JSR 2010-10-05
// -----------------------------------------------------------------------------
void DictionaryType::GetStringList( QStringList& list, const std::string& kind, const std::string& language ) const
{
    tools::Iterator< const DictionaryEntryType& > it = CreateIterator();
    while( it.HasMoreElements() )
        list.append( it.NextElement().GetLabel( kind, language ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: DictionaryType::GetString
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
const std::string& DictionaryType::GetString( unsigned int id, const std::string& kind, const std::string& language ) const
{
    static const std::string defaultString;
    tools::Iterator< const DictionaryEntryType& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const DictionaryEntryType& entry = it.NextElement();
        if( entry.GetId() == id )
            return entry.GetLabel( kind, language );
    }
    return defaultString;
}

// -----------------------------------------------------------------------------
// Name: DictionaryType::GetId
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
int DictionaryType::GetId( const std::string& string, const std::string& kind, const std::string& language ) const
{
    tools::Iterator< const DictionaryEntryType& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const DictionaryEntryType& entry = it.NextElement();
        if( entry.GetLabel( kind, language ) == string )
            return entry.GetId();
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Name: DictionaryType::ReadEntry
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void DictionaryType::ReadEntry( xml::xistream& xis )
{
    Register( xis.attribute< unsigned int >( "id" ), *new DictionaryEntryType( xis ) );
}
