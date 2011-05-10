// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DictionaryExtensions.h"
#include "AttributeType.h"
#include "ExtensionType.h"
#include "ExtensionTypes.h"
#include "DictionaryType.h"
#include "DictionaryEntryType.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions constructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
DictionaryExtensions::DictionaryExtensions( const std::string& extensionType, const ExtensionTypes& resolver )
    : enabled_      ( false )
    , extensionType_( extensionType )
    , resolver_     ( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions constructor
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
DictionaryExtensions::DictionaryExtensions( const std::string& extensionType, xml::xistream& xis, const ExtensionTypes& resolver )
    : enabled_      ( true )
    , extensionType_( extensionType )
    , resolver_     ( resolver )
{
    xis >> xml::list( "entry", *this, &DictionaryExtensions::ReadExtension );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions destructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
DictionaryExtensions::~DictionaryExtensions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::SerializeAttributes
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void DictionaryExtensions::SerializeAttributes( xml::xostream& xos ) const
{
    if( enabled_ && !extensions_.empty() )
    {
        xos << xml::start( "extensions" );
        std::string value;
        for( CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
        {
            value = it->second;
            ExtensionType* type = resolver_.tools::StringResolver< ExtensionType >::Find( extensionType_ );
            if( !type )
                continue;
            tools::Iterator< const AttributeType& > attributeIt = type->CreateIterator();
            while( attributeIt.HasMoreElements() )
            {
                const AttributeType& attribute = attributeIt.NextElement();
                if( attribute.GetType() == AttributeType::ETypeDictionary && attribute.GetName() == it->first )
                {
                    std::string dictionary;
                    std::string kind;
                    std::string language;
                    attribute.GetDictionaryValues( dictionary, kind, language );
                    DictionaryType* dico = resolver_.tools::StringResolver< DictionaryType >::Find( dictionary );
                    if( !dico )
                        continue;
                    DictionaryEntryType* entry = dico->Find( it->second );
                    if( entry )
                    {
                        value = boost::lexical_cast< std::string >( entry->GetId() );
                        break;
                    }
                }
            }
            xos << xml::start( "entry" )
                    << xml::attribute( "key", it->first )
                    << xml::attribute( "value", value )
                << xml::end;
        }
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::ReadExtension
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void DictionaryExtensions::ReadExtension( xml::xistream& xis )
{
    std::string value = xis.attribute< std::string >( "value" );
    std::string key = xis.attribute< std::string >( "key" );
    ExtensionType* type = resolver_.tools::StringResolver< ExtensionType >::Find( extensionType_ );
    try
    {
        if( type )
        {
            tools::Iterator< const AttributeType& > attributeIt = type->CreateIterator();
            bool found = false;
            while( !found && attributeIt.HasMoreElements() )
            {
                const AttributeType& attribute = attributeIt.NextElement();
                if( attribute.GetType() == AttributeType::ETypeDictionary && attribute.GetName() == key )
                {
                    unsigned int id = boost::lexical_cast< unsigned int >( value );
                    std::string dictionary;
                    std::string kind;
                    std::string language;
                    attribute.GetDictionaryValues( dictionary, kind, language );
                    DictionaryType* dico = resolver_.tools::StringResolver< DictionaryType >::Find( dictionary );
                    if( !dico )
                        continue;
                    tools::Iterator< const DictionaryEntryType& > dicoIt = dico->CreateIterator();
                    while( dicoIt.HasMoreElements() )
                    {
                        const DictionaryEntryType& entry = dicoIt.NextElement();
                        if( entry.GetId() == id )
                        {
                            value = entry.GetKey();
                            found = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    catch( ... )
    {
        // NOTHING
    }
    extensions_[ key ] = value;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::SetEnabled
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void DictionaryExtensions::SetEnabled( bool enabled )
{
    enabled_ = enabled;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::IsEnabled
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
bool DictionaryExtensions::IsEnabled() const
{
    return enabled_;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::SetValue
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void DictionaryExtensions::SetValue( const std::string& name, const std::string& value )
{
    extensions_[ name ] = value;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::GetValue
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
const std::string& DictionaryExtensions::GetValue( const std::string& name ) const
{
    static const std::string defaultString;
    CIT_Extensions it = extensions_.find( name );
    if( it != extensions_.end() )
        return it->second;
    return defaultString;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::GetExtensions
// Created: JSR 2010-10-11
// -----------------------------------------------------------------------------
const DictionaryExtensions::T_Extensions& DictionaryExtensions::GetExtensions() const
{
    return extensions_;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::GetExtensionTypes
// Created: ABR 2011-05-03
// -----------------------------------------------------------------------------
const ExtensionTypes& DictionaryExtensions::GetExtensionTypes() const
{
    return resolver_;
}
