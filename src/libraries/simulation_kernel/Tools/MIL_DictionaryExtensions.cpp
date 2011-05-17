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
#include "Network/NET_AsnException.h"

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
    extensions_[ xis.attribute< std::string >( "key" ) ] = xis.attribute< std::string >( "value" );
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::WriteODB
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void MIL_DictionaryExtensions::WriteODB( xml::xostream& xos ) const
{
    if( extensions_.empty() )
        return;
    xos << xml::start( "extensions" );
    for( CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
        xos << xml::start( "entry" )
                << xml::attribute( "key", it->first )
                << xml::attribute( "value", it->second )
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::load
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void MIL_DictionaryExtensions::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int size;
    file >> size;
    {
        std::string first;
        std::string second;
        for( unsigned int i = 0; i < size; ++i )
        {
            file >> first
                 >> second;
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
    unsigned int size = extensions_.size();
    file << size;
    for( CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
        file << it->first
             << it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::OnReceiveMsgChangeExtensions
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void MIL_DictionaryExtensions::OnReceiveMsgChangeExtensions( const sword::UnitMagicAction& msg )
{
    if( msg.type() != sword::UnitMagicAction::change_extension || !msg.has_parameters() || msg.parameters().elem_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck::ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    const google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& list = msg.parameters().elem( 0 ).value();
    if( list.size() != 1 || !list.Get( 0 ).has_extensionlist() )
        throw NET_AsnException< sword::UnitActionAck::ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
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
