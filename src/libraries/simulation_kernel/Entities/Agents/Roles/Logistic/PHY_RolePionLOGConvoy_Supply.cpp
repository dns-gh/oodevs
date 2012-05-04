// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RolePionLOGConvoy_Supply.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 3 $
// $Workfile: PHY_RolePionLOGConvoy_Supply.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePionLOGConvoy_Supply.h"
#include "Entities/Agents/Roles/Logistic/SupplyConvoyReal_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "LocationActionNotificationHandler_ABC.h"
#include "SpeedComputer_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePionLOGConvoy_Supply )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePionLOGConvoy_Supply* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePionLOGConvoy_Supply* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RolePionLOGConvoy_Supply( *pion );
    pion->RegisterRole( *role );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply constructor
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
PHY_RolePionLOGConvoy_Supply::PHY_RolePionLOGConvoy_Supply( MIL_AgentPion& pion )
    : pion_   ( pion )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply destructor
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
PHY_RolePionLOGConvoy_Supply::~PHY_RolePionLOGConvoy_Supply()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePionLOGConvoy_Supply::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Supply >( *this );
//         & pConvoy_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::Update
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::Update( bool bIsDead )
{
    if( bIsDead )
        if( MIL_AgentPion* pionPc = pion_.GetAutomate().GetPionPC() )
            pion_.Apply( &location::LocationActionNotificationHandler_ABC::Follow, *pionPc );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::Clean
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::Clean()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::AssignConvoy
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::AssignConvoy( boost::shared_ptr< logistic::SupplyConvoyReal_ABC > convoy )
{
    assert( !convoy_.get() );
    convoy_ = convoy;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::UnassignConvoy
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::UnassignConvoy()
{
    assert( convoy_.get() );
    convoy_.reset();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyNotifyMovedToSupplier
// Created: NLD 2011-08-02
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::ConvoyNotifyMovedToSupplier()
{
    if( convoy_ )
        convoy_->NotifyMovedToSupplier();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyNotifyMovedToTransportersProvider
// Created: NLD 2011-08-02
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::ConvoyNotifyMovedToTransportersProvider()
{
    if( convoy_ )
        convoy_->NotifyMovedToTransportersProvider();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyNotifyMovedToSupplyRecipient
// Created: NLD 2011-08-02
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::ConvoyNotifyMovedToSupplyRecipient()
{
    if( convoy_ )
        convoy_->NotifyMovedToSupplyRecipient();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyEndMission
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::ConvoyEndMission()
{
    if( convoy_ )
        convoy_->NotifyConvoyEndMission();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetCurrentAction
// Created: NLD 2011-08-02
// -----------------------------------------------------------------------------
int PHY_RolePionLOGConvoy_Supply::ConvoyGetCurrentAction() const
{
    return convoy_ ? convoy_->GetCurrentAction() : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetSupplier
// Created: NLD 2011-08-02
// -----------------------------------------------------------------------------
logistic::SupplySupplier_ABC* PHY_RolePionLOGConvoy_Supply::ConvoyGetSupplier() const
{
    return convoy_ ? &convoy_->GetSupplier() : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetTransportersProvider
// Created: NLD 2011-08-02
// -----------------------------------------------------------------------------
logistic::SupplySupplier_ABC* PHY_RolePionLOGConvoy_Supply::ConvoyGetTransportersProvider() const
{
    return convoy_ ? &convoy_->GetTransportersProvider() : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetCurrentSupplyRecipient
// Created: NLD 2011-08-02
// -----------------------------------------------------------------------------
logistic::SupplyRecipient_ABC* PHY_RolePionLOGConvoy_Supply::ConvoyGetCurrentSupplyRecipient() const
{
    return convoy_ ? convoy_->GetCurrentSupplyRecipient() : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetPathToNextDestination
// Created: NLD 2011-08-02
// -----------------------------------------------------------------------------
const T_PointVector* PHY_RolePionLOGConvoy_Supply::ConvoyGetPathToNextDestination() const
{
    return convoy_ ? convoy_->GetPathToNextDestination() : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ModifySpeed
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    if( convoy_ )
        convoy_->Execute( algorithm );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::NotifyComposanteChanged
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::NotifyComposanteChanged( PHY_ComposantePion& composante )
{
    if( !convoy_ || composante.GetState() != PHY_ComposanteState::dead_ )
        return;

    convoy_->NotifyTransporterDestroyed( composante );
}
