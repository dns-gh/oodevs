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
#include "Controllers.h"
#include "Entity_ABC.h"
#include "ExtensionType.h"
#include "ExtensionTypes.h"
#include "DictionaryType.h"
#include "DictionaryEntryType.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions constructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
DictionaryExtensions::DictionaryExtensions( kernel::Controllers& controllers, const std::string& extensionType, const ExtensionTypes& resolver )
    : controllers_  ( controllers )
    , enabled_      ( false )
    , extensionType_( extensionType )
    , resolver_     ( resolver )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions constructor
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
DictionaryExtensions::DictionaryExtensions( kernel::Controllers& controllers, const std::string& extensionType, xml::xistream& xis, const ExtensionTypes& resolver )
    : controllers_  ( controllers )
    , enabled_      ( xis.has_child( "extensions" ) )
    , extensionType_( extensionType )
    , resolver_     ( resolver )
{
    if( enabled_ )
        xis >> xml::start( "extensions" )
                >> xml::list( "entry", *this, &DictionaryExtensions::ReadExtension )
            >> xml::end;
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions destructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
DictionaryExtensions::~DictionaryExtensions()
{
    controllers_.Unregister( *this );
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
        for( CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
            xos << xml::start( "entry" )
                    << xml::attribute( "key", it->first )
                    << xml::attribute( "value", GetValueWithDictionnaryLink( it->first ) )
                << xml::end;
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::ReadExtension
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void DictionaryExtensions::ReadExtension( xml::xistream& xis )
{
    std::string key = xis.attribute< std::string >( "key" );
    std::string value = xis.attribute< std::string >( "value" );
    SetValueWithDictionnaryLink( key, value );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::GetValueWithDictionnaryLink
// Created: ABR 2011-05-17
// -----------------------------------------------------------------------------
std::string DictionaryExtensions::GetValueWithDictionnaryLink( const std::string& name ) const
{
    std::string value = GetValue( name );
    try
    {
        ExtensionType* type = resolver_.tools::StringResolver< ExtensionType >::Find( extensionType_ );
        if( !type )
            return "";
        tools::Iterator< const AttributeType& > attributeIt = type->CreateIterator();
        while( attributeIt.HasMoreElements() )
        {
            const AttributeType& attribute = attributeIt.NextElement();
            if( attribute.GetType() == AttributeType::ETypeDictionary && attribute.GetName() == name )
            {
                std::string dictionary;
                std::string kind;
                std::string language;
                attribute.GetDictionaryValues( dictionary, kind, language );
                DictionaryType* dico = resolver_.tools::StringResolver< DictionaryType >::Find( dictionary );
                if( !dico )
                    continue;
                DictionaryEntryType* entry = dico->Find( value );
                if( entry )
                {
                    value = boost::lexical_cast< std::string >( entry->GetId() );
                    break;
                }
            }
        }
    }
    catch( ... )
    {
        // NOTHING
    }
    return value;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::SetValueWithDictionnaryLink
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void DictionaryExtensions::SetValueWithDictionnaryLink( const std::string& name, std::string value )
{
    bool extensionFound = false;
    try
    {
        ExtensionType* type = resolver_.tools::StringResolver< ExtensionType >::Find( extensionType_ );
        if( type )
        {
            tools::Iterator< const AttributeType& > attributeIt = type->CreateIterator();
            while( !extensionFound && attributeIt.HasMoreElements() )
            {
                const AttributeType& attribute = attributeIt.NextElement();
                if( attribute.GetName() == name )
                {
                    extensionFound = true;
                    if( attribute.GetType() == AttributeType::ETypeDictionary )
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
                                break;
                            }
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
    if( extensionFound )
        extensions_[ name ] = value;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::NotifyDeleted
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void DictionaryExtensions::NotifyDeleted( const kernel::Entity_ABC& element )
{
    std::string& values = extensions_[ resolver_.GetNameByType( AttributeType::ETypeDiffusionList ) ];
    if( !values.empty() )
    {
        const std::string id = boost::lexical_cast< std::string >( element.GetId() );
        std::vector< std::string > valuesVector;
        boost::split( valuesVector, values, boost::algorithm::is_any_of( ";" ) );
        std::vector< std::string >::iterator founded = std::find( valuesVector.begin(), valuesVector.end(), id );
        if( founded != valuesVector.end() )
        {
            valuesVector.erase( founded );
            values.clear();
            for( std::vector< std::string >::const_iterator it = valuesVector.begin(); it != valuesVector.end(); ++it )
            {
                values += ( values.empty() ) ? "" : ";";
                values += *it;
            }
        }
    }
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
// Name: DictionaryExtensions::Reset
// Created: JSR 2012-01-31
// -----------------------------------------------------------------------------
void DictionaryExtensions::Reset( const std::string& name )
{
    extensions_.erase( name );
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
