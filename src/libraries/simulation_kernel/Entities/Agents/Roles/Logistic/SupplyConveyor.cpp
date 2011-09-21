//******************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyConveyor.h"
#include "SupplyConvoyEventsObserver_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include <boost/foreach.hpp>

using namespace logistic;

// =============================================================================
// Constructor / destructor 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConveyor constructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConveyor::SupplyConveyor( PHY_ComposantePion& composante, MIL_AgentPion& pion )
    : composante_( composante )
    , pion_      ( pion )
    , borrower_  ( 0 )
{
    composante_.GetStockTransporterCapacity( weightCapacity_, volumeCapacity_ );
    assert( weightCapacity_ > 0. && volumeCapacity_ > 0. );
    pion_.GetRole< PHY_RoleInterface_Supply >().StartUsingForLogistic( composante );
}

// -----------------------------------------------------------------------------
// Name: SupplyConveyor destructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConveyor::~SupplyConveyor()
{
    UndoLend();
    pion_.GetRole< PHY_RoleInterface_Supply >().StopUsingForLogistic( composante_ );
}

// =============================================================================
// Operations
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConveyor::Convoy
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
double SupplyConveyor::Convoy( SupplyConvoyEventsObserver_ABC& eventsObserver, const PHY_DotationCategory& dotationCategory, double quantity )
{
    if( !composante_.GetType().CanTransportStock( dotationCategory ) )
        return 0.;

    double volumeToConvoy = quantity * dotationCategory.GetVolume();
    double weightToConvoy = quantity * dotationCategory.GetWeight();

    if( volumeToConvoy > volumeCapacity_ )
    {
        volumeToConvoy = volumeCapacity_;
        weightToConvoy = ( volumeToConvoy / dotationCategory.GetVolume() ) * dotationCategory.GetWeight();
    }

    if( weightToConvoy > weightCapacity_ )
    {
        weightToConvoy = weightCapacity_;
        volumeToConvoy = ( weightToConvoy / dotationCategory.GetWeight() ) * dotationCategory.GetVolume();
    }

    assert( volumeToConvoy <= volumeCapacity_ );
    assert( weightToConvoy <= weightCapacity_ );

    volumeCapacity_ -= volumeToConvoy;
    weightCapacity_ -= weightToConvoy;

    assert( weightToConvoy / dotationCategory.GetWeight() == volumeToConvoy / dotationCategory.GetVolume() );

    const double quantityConvoyed = std::min( quantity, std::max( weightToConvoy / dotationCategory.GetWeight(), volumeToConvoy / dotationCategory.GetVolume() ) );
    resourcesConvoyed_[ &dotationCategory ] += quantityConvoyed;
    eventsObserver.OnResourceAssignedToConvoy( dotationCategory, quantityConvoyed );
    return quantityConvoyed;
}

// -----------------------------------------------------------------------------
// Name: SupplyConveyor::IsFull
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
bool SupplyConveyor::IsFull() const
{
    return weightCapacity_ <= 0. || volumeCapacity_ <= 0.;
}

// -----------------------------------------------------------------------------
// Name: SupplyConveyor::IsEmpty
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
bool SupplyConveyor::IsEmpty() const
{
    return resourcesConvoyed_.empty();
}

// -----------------------------------------------------------------------------
// Name: SupplyConveyor::LendTo
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
void SupplyConveyor::LendTo( MIL_AgentPion& borrower )
{
    if( borrower_ )
        UndoLend();

    borrower_ = &borrower;
    pion_.GetRole< PHY_RoleInterface_Supply      >().StopUsingForLogistic( composante_ );
    pion_.GetRole< PHY_RoleInterface_Composantes >().LendComposante      ( *borrower_, composante_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyConveyor::UndoLend
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
void SupplyConveyor::UndoLend()
{
    if( borrower_ )
    {
        pion_.GetRole< PHY_RoleInterface_Composantes >().RetrieveLentComposante( *borrower_, composante_ );
        pion_.GetRole< PHY_RoleInterface_Supply      >().StartUsingForLogistic ( composante_ );
        borrower_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConveyor::ApproximateTravelTime
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
unsigned SupplyConveyor::ApproximateTravelTime( const MT_Vector2D& startPos, const MT_Vector2D& endPos ) const
{
    return composante_.ApproximateTravelTime( startPos, endPos );
}

// -----------------------------------------------------------------------------
// Name: SupplyConveyor::Supply
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
double SupplyConveyor::Supply( const PHY_DotationCategory& dotationCategory, double quantity )
{
    T_Resources::iterator it = resourcesConvoyed_.find( &dotationCategory );
    if( it != resourcesConvoyed_.end() )
    {
        double tmp = std::min( quantity, it->second );
        quantity -= tmp;
        it->second -= tmp;
    }
    return quantity;
}

// -----------------------------------------------------------------------------
// Name: SupplyConveyor::Destroy
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConveyor::Destroy( SupplyConvoyEventsObserver_ABC& eventsObserver )
{
    BOOST_FOREACH( const T_Resources::value_type& data, resourcesConvoyed_ )
        eventsObserver.OnResourceRemovedFromConvoy( *data.first, data.second );
    resourcesConvoyed_.clear();
}