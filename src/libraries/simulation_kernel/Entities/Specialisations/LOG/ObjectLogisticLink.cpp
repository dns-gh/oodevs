// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ObjectLogisticLink.h"
#include "MIL_AutomateLOG.h"

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( ObjectLogisticLink )

template< typename Archive >
void logistic::save_construct_data( Archive& archive, const logistic::ObjectLogisticLink* link, const unsigned int )
{
    archive << link->superior_;
}

template< typename Archive >
void logistic::load_construct_data( Archive& archive, logistic::ObjectLogisticLink* link, const unsigned int )
{
    MIL_AutomateLOG* superior;
    archive >> superior;
    ::new(link)ObjectLogisticLink( *superior );
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticLink constructor
// Created: NLD 2011-01-05
// -----------------------------------------------------------------------------
ObjectLogisticLink::ObjectLogisticLink( MIL_AutomateLOG& superior )
    : superior_( &superior )
{
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticLink destructor
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
ObjectLogisticLink::~ObjectLogisticLink()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ObjectLogisticLink::ConsumeQuota
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
double ObjectLogisticLink::ConsumeQuota( const PHY_DotationCategory& dotationCategory, double quantity, const T_Requesters& /*requesters*/ )
{
    if( superior_->SupplyHasStock( dotationCategory ) )
        return quantity;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticLink::ReturnQuota
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
void ObjectLogisticLink::ReturnQuota( const PHY_DotationCategory&, double )
{
    // NOTHING
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ObjectLogisticLink::SendFullState
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void ObjectLogisticLink::SendFullState( unsigned int /*context*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticLink::SendChangedState
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void ObjectLogisticLink::SendChangedState() const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticLink::OnReceiveChangeQuotas
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
std::set< const PHY_DotationCategory* > ObjectLogisticLink::OnReceiveChangeQuotas( const sword::MissionParameter& )
{
    return std::set< const PHY_DotationCategory* >();
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticLink::GetSuperior
// Created: MCO 2012-12-11
// -----------------------------------------------------------------------------
MIL_AutomateLOG& ObjectLogisticLink::GetSuperior() const
{
    return *superior_;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ObjectLogisticLink::SendFullState
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void ObjectLogisticLink::WriteODB( xml::xostream& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::serialize
// Created: NLD 2011-02-07
// -----------------------------------------------------------------------------
template < typename Archive >
void ObjectLogisticLink::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< LogisticLink_ABC >( *this );
}
