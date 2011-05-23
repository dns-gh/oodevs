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
#include "PHY_StockConvoy.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"

#include "simulation_kernel/LocationActionNotificationHandler_ABC.h"
#include "simulation_kernel/SpeedComputer_ABC.h"

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
    , pConvoy_( 0 )
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
    file & boost::serialization::base_object< PHY_RoleInterface_Supply >( *this )
         & pConvoy_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::Update
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::Update( bool bIsDead )
{
    if( bIsDead )
        pion_.Apply( &location::LocationActionNotificationHandler_ABC::Follow, pion_.GetAutomate().GetPionPC() );
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
void PHY_RolePionLOGConvoy_Supply::AssignConvoy( PHY_StockConvoy& convoy )
{
    assert( !pConvoy_ );
    pConvoy_ = &convoy;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::UnassignConvoy
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::UnassignConvoy( PHY_StockConvoy& convoy )
{
    assert( pConvoy_ == &convoy );
    pConvoy_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyLoad
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOGConvoy_Supply::ConvoyLoad() const
{
    if( !pConvoy_ )
        return true;
    return pConvoy_->Load();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyUnload
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOGConvoy_Supply::ConvoyUnload() const
{
    if( !pConvoy_ )
        return true;
    return pConvoy_->Unload();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyIsLoadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_RolePionLOGConvoy_Supply::ConvoyIsLoadingDone() const
{
    if( !pConvoy_ )
        return true;
    return pConvoy_->IsLoadingDone();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyIsUnloadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_RolePionLOGConvoy_Supply::ConvoyIsUnloadingDone() const
{
    if( !pConvoy_ )
        return true;
    return pConvoy_->IsUnloadingDone();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyEndMission
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::ConvoyEndMission()
{
    if( pConvoy_ )
        pConvoy_->EndMission();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ModifySpeed
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::Execute(moving::SpeedComputer_ABC& algorithm) const
{
    if( pConvoy_ )
        algorithm.AddModifier(pConvoy_->ModifySpeed( 1 ), false); // TODO change SpeedComputer_ABC interface
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::NotifyComposanteChanged
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::NotifyComposanteChanged( PHY_ComposantePion& composante )
{
    if( !pConvoy_ || composante.GetState() != PHY_ComposanteState::dead_ )
        return;

    pConvoy_->NotifyConveyorDestroyed( composante );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetSupplier
// Created: NLD 2006-07-31
// Deprecated by MGD : Give the first PC in the brainlogistic and not the exact supplier
// -----------------------------------------------------------------------------
const MIL_AgentPion* PHY_RolePionLOGConvoy_Supply::ConvoyGetSupplier() const
{
    if( !pConvoy_ )
        return 0;
    return pConvoy_->GetSupplier().GetPC();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetStockSupplier
// Created: MGD 2009-02-11
// Give the PC corresponding to sector supplier automata s
// -----------------------------------------------------------------------------
const MIL_AgentPion* PHY_RolePionLOGConvoy_Supply::ConvoyGetStockSupplier() const
{
    if( !pConvoy_ )
        return 0;
    return &pConvoy_->GetStockSupplier().GetPionPC();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetConvoyer
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
const MIL_AgentPion* PHY_RolePionLOGConvoy_Supply::ConvoyGetConvoyer() const
{
    if( !pConvoy_ )
        return 0;
    return &pion_.GetAutomate().GetPionPC(); // Pion PC de l'automate fournissant les moyens au convoi (= l'automate de ce pion)
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetSupplied
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
const MIL_AgentPion* PHY_RolePionLOGConvoy_Supply::ConvoyGetSupplied() const
{
    if( !pConvoy_ )
        return 0;
    return &pConvoy_->GetSupplied().GetPionPC();
}
