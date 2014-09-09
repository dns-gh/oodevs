// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DictionaryExtensions.h"
#include "protocol/ClientSenders.h"
#include "Tools/NET_AsnException.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_DictionaryExtensions )

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions constructor
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
MIL_DictionaryExtensions::MIL_DictionaryExtensions()
    : hasChanged_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions constructor
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
MIL_DictionaryExtensions::MIL_DictionaryExtensions( const MIL_DictionaryExtensions& other )
    : extensions_( other.extensions_ )
    , hasChanged_( false )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions constructor
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
MIL_DictionaryExtensions::MIL_DictionaryExtensions( xml::xistream& xis )
    : hasChanged_( false )
{
    xis >> xml::optional
        >> xml::start( "extensions" )
            >> xml::list( "entry", *this, &MIL_DictionaryExtensions::ReadExtension )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions destructor
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
MIL_DictionaryExtensions::~MIL_DictionaryExtensions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::ReadExtension
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void MIL_DictionaryExtensions::ReadExtension( xml::xistream& xis )
{
    extensions_[ xis.attribute< std::string >( "key" ) ] = xis.attribute< std::string >( "value", "" );
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::WriteODB
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void MIL_DictionaryExtensions::WriteODB( xml::xostream& xos ) const
{
    if( extensions_.empty() )
        return;
    bool found = false;
    for( auto it = extensions_.begin(); it != extensions_.end(); ++it )
    {
        if( it->second != "" )
        {
            if( !found )
            {
                found =  true;
                xos << xml::start( "extensions" );
            }
            xos << xml::start( "entry" )
                    << xml::attribute( "key", it->first );
            if( !it->second.empty() )
                xos << xml::attribute( "value", it->second );
            xos << xml::end;
        }
    }
    if( found )
        xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::load
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void MIL_DictionaryExtensions::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::size_t size;
    file >> size;
    {
        std::string first;
        std::string second;
        for( std::size_t i = 0; i < size; ++i )
        {
            file >> first;
            file >> second;
            extensions_[ first ] = second;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::save
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void MIL_DictionaryExtensions::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    std::size_t size = extensions_.size();
    file << size;
    for( auto it = extensions_.begin(); it != extensions_.end(); ++it )
    {
        file << it->first;
        file << it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::OnReceiveMsgChangeExtensions
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void MIL_DictionaryExtensions::OnReceiveMsgChangeExtensions( const sword::UnitMagicAction& msg )
{
    if( msg.type() != sword::UnitMagicAction::change_extension || !msg.has_parameters() || msg.parameters().elem_size() != 1 )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter,
            "invalid parameters count, one parameter expected" );
    const google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& list = msg.parameters().elem( 0 ).value();
    if( list.size() != 1 || !list.Get( 0 ).has_extensionlist() )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter,
            "parameters[0] must be an ExtensionList" );

    const sword::Extension& extensions = list.Get( 0 ).extensionlist();
    hasChanged_ = false;
    for( int i = 0; i < extensions.entries().size(); ++i )
    {
        const sword::Extension::Entry& entry = extensions.entries().Get( i );
        hasChanged_ = hasChanged_ || extensions_[ entry.name() ] != entry.value();
        extensions_[ entry.name() ] = entry.value();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::HasChanged
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
bool MIL_DictionaryExtensions::HasChanged() const
{
    return hasChanged_;
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::IsEmpty
// Created: LDC 2011-08-19
// -----------------------------------------------------------------------------
bool MIL_DictionaryExtensions::IsEmpty() const
{
    return extensions_.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::ReadExtensions
// Created: AHC 2012-10-01
// -----------------------------------------------------------------------------
void MIL_DictionaryExtensions::ReadExtensions( const sword::Extension& extensions )
{
    for( int i = 0; i < extensions.entries().size(); ++i )
    {
        const sword::Extension::Entry& entry = extensions.entries().Get( i );
        hasChanged_ = hasChanged_ || extensions_[ entry.name() ] != entry.value();
        extensions_[ entry.name() ] = entry.value();
    }
}

void MIL_DictionaryExtensions::ReadExtensions( const std::vector< Extension >& extensions )
{
    for( auto it = extensions.cbegin(); it != extensions.cend(); ++it )
    {
        hasChanged_ = hasChanged_ || extensions_[ it->first ] != it->second;
        extensions_[ it->first ] = it->second;
    }
}
