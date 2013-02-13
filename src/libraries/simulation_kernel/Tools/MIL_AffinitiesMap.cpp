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
#include "Tools/MIL_Config.h"
#include "protocol/ClientSenders.h"
#include "Network/NET_AsnException.h"


BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AffinitiesMap )

double MIL_AffinitiesMap::interrogateDelayForMinimumAffinity_ = 7200.; // 2h
double MIL_AffinitiesMap::interrogateDelayForNeutralAffinity_ = 3600.; // 1h
double MIL_AffinitiesMap::interrogateDelayForMaximumAffinity_ = 1800.; // 30m

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap constructor
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
MIL_AffinitiesMap::MIL_AffinitiesMap()
    : hasChanged_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap constructor
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
MIL_AffinitiesMap::MIL_AffinitiesMap( xml::xistream& xis, const MIL_Config& config )
    : hasChanged_( false )
{
    xis >> xml::optional
        >> xml::start( "adhesions" )
            >> xml::list( "adhesion", *this, &MIL_AffinitiesMap::ReadAffinity, boost::cref( config ) )
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
void MIL_AffinitiesMap::ReadAffinity( xml::xistream& xis, const MIL_Config& config )
{
    unsigned int id = xis.attribute< unsigned long >( "party" );
    if( config.CanCreateParty( id ) )
        affinities_[ id ] = xis.attribute< float >( "value" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::WriteODB
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
void MIL_AffinitiesMap::WriteODB( xml::xostream& xos ) const
{
    if( affinities_.empty() )
        return;
    xos << xml::start( "adhesions" );
    for( auto it = affinities_.begin(); it != affinities_.end(); ++it )
        xos << xml::start( "adhesion" )
                << xml::attribute( "party", it->first )
                << xml::attribute( "value", it->second )
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::load
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
void MIL_AffinitiesMap::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::size_t size;
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
    std::size_t size = affinities_.size();
    file << size;
    for( auto it = affinities_.begin(); it != affinities_.end(); ++it )
        file << it->first
             << it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::HasChanged
// Created: JSR 2011-03-09
// -----------------------------------------------------------------------------
bool MIL_AffinitiesMap::HasChanged() const
{
    return hasChanged_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::IsEmpty
// Created: LDC 2011-08-19
// -----------------------------------------------------------------------------
bool MIL_AffinitiesMap::IsEmpty() const
{
    return affinities_.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::GetAffinity
// Created: ABR 2011-03-28
// -----------------------------------------------------------------------------
float MIL_AffinitiesMap::GetAffinity( unsigned long teamID ) const
{
    CIT_Affinities it = affinities_.find( teamID );
    if( it == affinities_.end() )
        return 0.f;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::OnReceiveMsgChangeAffinities
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
void MIL_AffinitiesMap::OnReceiveMsgChangeAffinities( const sword::UnitMagicAction& msg )
{
    if( !msg.has_parameters() || msg.parameters().elem_size() != 1 )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    const ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& list = msg.parameters().elem( 0 ).value();
    affinities_.clear();
    for( int i = 0; i < list.size(); ++i )
    {
        sword::MissionParameter_Value element = list.Get( i );
        affinities_[ element.list( 0 ).identifier() ] = element.list( 1 ).areal();
    }
    hasChanged_ = true;
}
