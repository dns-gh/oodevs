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
    list.append( "" );
    tools::Iterator< const DictionaryEntryType& > it = CreateIterator();
    while( it.HasMoreElements() )
        list.append( it.NextElement().GetLabel( kind, language ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: DictionaryType::GetLabel
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
std::string DictionaryType::GetLabel( const std::string& key, const std::string& kind, const std::string& language ) const
{
    tools::Iterator< const DictionaryEntryType& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const DictionaryEntryType& entry = it.NextElement();
        if( entry.GetKey() == key )
            return entry.GetLabel( kind, language );
    }
    return "";
}

// -----------------------------------------------------------------------------
// Name: DictionaryType::GetKey
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
std::string DictionaryType::GetKey( const std::string& label, const std::string& kind, const std::string& language ) const
{
    tools::Iterator< const DictionaryEntryType& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const DictionaryEntryType& entry = it.NextElement();
        if( entry.GetLabel( kind, language ) == label )
            return entry.GetKey();
    }
    return "";
}

// -----------------------------------------------------------------------------
// Name: DictionaryType::ReadEntry
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void DictionaryType::ReadEntry( xml::xistream& xis )
{
    Register( xis.attribute< std::string >( "name" ), *new DictionaryEntryType( xis ) );
}
