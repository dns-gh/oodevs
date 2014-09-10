// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_InhabitantSatisfactions.h"
#include "MIL_Time_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "protocol/ClientSenders.h"
#include "Urban/PHY_ResourceNetworkType.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_InhabitantSatisfactions )

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions constructor
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
MIL_InhabitantSatisfactions::MIL_InhabitantSatisfactions( xml::xistream& xis )
    : health_           ( 0 )
    , safety_           ( 1.f )
    , lodging_          ( 1.f )
    , lastSafety_       ( 1.f )
    , healthChanged_    ( false )
    , lodgingChanged_   ( false )
    , resourceChanged_  ( false )
    , motivationChanged_( false )
{
    xis >> xml::start( "health-need" )
            >> xml::attribute( "quantity", healthNeed_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions constructor
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
MIL_InhabitantSatisfactions::MIL_InhabitantSatisfactions()
    : healthNeed_       ( 0 )
    , health_           ( 0 )
    , safety_           ( 0 )
    , lodging_          ( 0 )
    , lastSafety_       ( 0 )
    , healthChanged_    ( false )
    , lodgingChanged_   ( false )
    , resourceChanged_  ( false )
    , motivationChanged_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions destructor
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
MIL_InhabitantSatisfactions::~MIL_InhabitantSatisfactions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::load
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> healthNeed_;
    file >> health_;
    file >> safety_;
    file >> lodging_;
    file >> motivations_;
    std::size_t size;
    file >> size;
    for( std::size_t i = 0; i < size; ++i )
    {
        unsigned int id;
        float satisfaction;
        file >> id >> satisfaction;
        if( const PHY_ResourceNetworkType* resource = PHY_ResourceNetworkType::Find( id ) )
            resources_[ resource ] = satisfaction;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::save
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << healthNeed_;
    file << health_;
    file << safety_;
    file << lodging_;
    file << motivations_;
    std::size_t size = resources_.size();
    file << size;
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
    {
        unsigned int id = it->first->GetId();
        file << id << it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::WriteODB
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "health-need" )
            << xml::attribute( "quantity", healthNeed_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::SendFullState
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::SendFullState( client::PopulationUpdate& msg ) const
{
    msg().mutable_satisfaction()->set_health( health_ );
    msg().mutable_satisfaction()->set_safety( safety_ );
    msg().mutable_satisfaction()->set_lodging( lodging_ );
    for( auto it = motivations_.begin(); it != motivations_.end(); ++it )
    {
        sword::PopulationUpdate_MotivationSatisfaction* motivation = msg().mutable_satisfaction()->add_motivations();
        motivation->set_motivation( it->first );
        motivation->set_percentage( it->second );
    }
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
    {
        sword::PopulationUpdate_ResourceSatisfaction* resource = msg().mutable_satisfaction()->add_resources();
        resource->mutable_resource()->set_id( it->first->GetDotationCategory().GetMosID() );
        resource->set_value( it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::UpdateNetwork
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::UpdateNetwork( client::PopulationUpdate& msg ) const
{
    if( healthChanged_ )
    {
        msg().mutable_satisfaction()->set_health( health_ );
        healthChanged_ = false;
    }
    if( lodgingChanged_ )
    {
        msg().mutable_satisfaction()->set_lodging( lodging_ );
        lodgingChanged_ = false;
    }
    if( std::abs( lastSafety_ - safety_ ) >= 0.01f )
    {
        msg().mutable_satisfaction()->set_safety( safety_ );
        lastSafety_ = safety_;
    }
    if( motivationChanged_ )
    {
        for( auto it = motivations_.begin(); it != motivations_.end(); ++it )
        {
            sword::PopulationUpdate_MotivationSatisfaction* motivation = msg().mutable_satisfaction()->add_motivations();
            motivation->set_motivation( it->first );
            motivation->set_percentage( it->second );
        }
        motivationChanged_ = false;
    }
    if( resourceChanged_ )
    {
        for( auto it = resources_.begin(); it != resources_.end(); ++it )
        {
            sword::PopulationUpdate_ResourceSatisfaction* resource = msg().mutable_satisfaction()->add_resources();
            resource->mutable_resource()->set_id( it->first->GetDotationCategory().GetMosID() );
            resource->set_value( it->second );
        }
        resourceChanged_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::ComputeHealthSatisfaction
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::ComputeHealthSatisfaction( float healthCount )
{
    float health = 1.f;
    if( healthNeed_ != 0 )
        health = std::min( 1.f, static_cast< float >( healthCount )/ healthNeed_ );
    if( std::abs( health - health_ ) >= 0.01f )
    {
        healthChanged_ = true;
        health_ = health;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::SetLodgingSatisfaction
// Created: JSR 2011-01-26
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::SetLodgingSatisfaction( float occupationFactor )
{
    if( std::abs( occupationFactor - lodging_ ) >= 0.01f )
    {
        lodgingChanged_ = true;
        lodging_ = occupationFactor;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::ComputeMotivationSatisfactions
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::ComputeMotivationSatisfactions( const std::map< std::string, unsigned int >& occupations, unsigned long living )
{
    for( auto it = occupations.begin(); it != occupations.end(); ++it )
    {
         float satisfaction = living == 0 ? 0 : std::min( 1.f, static_cast< float >( it->second ) / living );
         if( motivations_.find( it->first ) != motivations_.end() && motivations_[ it->first ] != satisfaction )
             motivationChanged_ = true;
         motivations_[ it->first ] = satisfaction;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::SetResourceSatisfaction
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::SetResourceSatisfaction( const PHY_ResourceNetworkType& resource, float satisfaction )
{
    if( resources_.find( &resource ) == resources_.end() || resources_[ &resource ] != satisfaction )
    {
        resources_[ &resource ] = satisfaction;
        resourceChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::IncreaseSafety
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::IncreaseSafety( float gainPerHour )
{
    safety_ = std::min( 1.f, safety_ + MIL_Time_ABC::GetTime().GetTickDuration() * gainPerHour / 3600 );
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantSatisfactions::DecreaseSafety
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_InhabitantSatisfactions::DecreaseSafety( float lossByFire )
{
    safety_ = std::max( 0.f, safety_ - lossByFire );
}
