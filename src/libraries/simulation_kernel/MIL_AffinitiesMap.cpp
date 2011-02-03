// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AffinitiesMap.h"
#include "protocol/ClientSenders.h"
#include "Network/NET_AsnException.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AffinitiesMap )

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap constructor
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
MIL_AffinitiesMap::MIL_AffinitiesMap()
    : hasChanged_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap constructor
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
MIL_AffinitiesMap::MIL_AffinitiesMap( xml::xistream& xis )
    : hasChanged_ ( false )
{
    xis >> xml::optional
        >> xml::start( "affinities" )
            >> xml::list( "affinity", *this, &MIL_AffinitiesMap::ReadAffinity )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap destructor
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
MIL_AffinitiesMap::~MIL_AffinitiesMap()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::ReadAffinity
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
void MIL_AffinitiesMap::ReadAffinity( xml::xistream& xis )
{
    affinities_[ xis.attribute< unsigned long >( "id" ) ] = xis.attribute< float >( "value" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::WriteODB
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
void MIL_AffinitiesMap::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "affinities" );
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
        xos << xml::start( "affinity" )
                << xml::attribute( "id", it->first )
                << xml::attribute( "value", it->second )
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::SendFullState
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
void MIL_AffinitiesMap::SendFullState( client::PopulationUpdate& msg ) const
{
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
    {
        sword::PartyAdhesion& adhesion = *msg().add_adhesions();
        adhesion.mutable_party()->set_id( it->first );
        adhesion.set_value( it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::UpdateNetwork
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
void MIL_AffinitiesMap::UpdateNetwork( client::PopulationUpdate& msg )
{
    if( hasChanged_ )
    {
        for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
        {
            sword::PartyAdhesion& adhesion = *msg().add_adhesions();
            adhesion.mutable_party()->set_id( it->first );
            adhesion.set_value( it->second );
        }
        hasChanged_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::OnReceiveMsgChangeAffinities
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
void MIL_AffinitiesMap::OnReceiveMsgChangeAffinities( const sword::UnitMagicAction& msg )
{
    if( !msg.has_parameters() || msg.parameters().elem_size() != 1 )
        throw NET_AsnException< sword::ChangePopulationMagicActionAck_ErrorCode >( sword::ChangePopulationMagicActionAck::error_invalid_parameter );
    const ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& list = msg.parameters().elem( 0 ).value();
    for( int i = 0; i < list.size(); ++i )
    {
        sword::MissionParameter_Value element = list.Get( i );
        IT_Affinities affinity = affinities_.find( element.list( 0 ).identifier() );
        if ( affinity == affinities_.end() )
            throw NET_AsnException< sword::ChangePopulationMagicActionAck_ErrorCode >( sword::ChangePopulationMagicActionAck::error_invalid_parameter );
        affinity->second = element.list( 1 ).areal();
    }
    hasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::load
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
void MIL_AffinitiesMap::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int size;
    file >> size;
    {
        unsigned long first;
        float second;
        for( unsigned int i = 0; i < size; ++i )
        {
            file >> first
                 >> second;
            affinities_[ first ] = second;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::save
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
void MIL_AffinitiesMap::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned int size = affinities_.size();
    file << size;
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
        file << it->first
             << it->second;
}
