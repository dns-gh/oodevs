// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 6 $
// $Workfile: PHY_RolePion_Surrender.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Surrender.h"
#include "SurrenderNotificationHandler_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/LodgingAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/VisionConeNotificationHandler_ABC.h"
#include "simulation_kernel/MoveComputer_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( surrender::PHY_RolePion_Surrender )

namespace surrender
{

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Surrender* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Surrender* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_Surrender( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Surrender::PHY_RolePion_Surrender( MIL_AgentPion& pion )
    : pion_                      ( pion )
    , pPrison_                   ( 0 )
    , bPrisoner_                 ( false )
    , bHasChanged_               ( true )
    , bSurrendered_              ( false )
    , nbrHumansLodgingManaged_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Surrender::~PHY_RolePion_Surrender()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Surrender::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Surrender >( *this )
         & bPrisoner_
         & pPrison_
         & nbrHumansLodgingManaged_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::Update( bool /*bIsDead*/ )
{
    if( pPrison_ && pPrison_->IsMarkedForDestruction() )
    {
        LodgingAttribute* pLodgingAttribute = pPrison_->RetrieveAttribute< LodgingAttribute >();
        if ( pLodgingAttribute )
            pLodgingAttribute->UnmanageResident( pion_ );

        pPrison_ = 0;
    }

    if( HasChanged() )
    {
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
        pion_.Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::NotifySurrendered
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::NotifySurrendered()
{
    if( !bSurrendered_ )
    {
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_Surrendered );
        bHasChanged_ = true;
        pion_.Apply(&SurrenderNotificationHandler_ABC::NotifySurrendered);
        bSurrendered_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::NotifySurrenderCanceled
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::NotifySurrenderCanceled()
{
    if( bSurrendered_ )
    {
        Release();
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_CancelSurrender );
        bHasChanged_ = true;
        pion_.Apply(&SurrenderNotificationHandler_ABC::NotifySurrenderCanceled);
        bSurrendered_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Capture
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::Capture( const MIL_AgentPion& pionTakingPrisoner )
{
    if( !IsSurrendered() )
        return false;

    pPrison_     = 0;
    bPrisoner_   = true;
    bHasChanged_ = true;
    pion_.Apply(&SurrenderNotificationHandler_ABC::NotifyCaptured);
    return pion_.GetAutomate().NotifyCaptured( pionTakingPrisoner );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Release
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::Release()
{
    if( !IsSurrendered() || !bPrisoner_ )
        return false;

    if ( pPrison_ )
    {
        LodgingAttribute* pLodgingAttribute = pPrison_->RetrieveAttribute< LodgingAttribute >();
        if ( pLodgingAttribute )
            pLodgingAttribute->UnmanageResident( pion_ );
    }

    pPrison_     = 0;
    bPrisoner_   = false;
    bHasChanged_ = true;
    pion_.Apply(&SurrenderNotificationHandler_ABC::NotifyReleased);

    return pion_.GetAutomate().NotifyReleased();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Imprison
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::Imprison( const MIL_Object_ABC& camp, MIL_AgentPion& transporter )
{
    if( !IsSurrendered() || !bPrisoner_ )
        return false;
    pPrison_ = const_cast< MIL_Object_ABC* >( &camp );

    LodgingAttribute* pLodgingAttribute = pPrison_->RetrieveAttribute< LodgingAttribute >();
    if ( pLodgingAttribute )
        pLodgingAttribute->ManageResident( pion_, transporter );

    pion_.GetAutomate().NotifyImprisoned( camp );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::SetAsPrisoner
// Created: JSR 2012-05-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::SetAsPrisoner()
{
    if( !bPrisoner_ )
    {
        bPrisoner_ = true;
        bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::SetAsFree
// Created: LMT 2012-06-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::SetAsFree()
{
    if( bPrisoner_ )
    {
        bPrisoner_ = false;
        bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::IsImprisoned
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::IsImprisoned( const MIL_Object_ABC& camp )
{
    if( !pPrison_ )
        return false;
    return pPrison_ == &camp; //$$$$
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::IsSurrendered() const
{
    return pion_.GetAutomate().IsSurrendered();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::GetArmySurrenderedTo
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
const MIL_Army_ABC* PHY_RolePion_Surrender::GetArmySurrenderedTo() const
{
    return pion_.GetAutomate().GetArmySurrenderedTo();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::SendFullState( client::UnitAttributes& msg ) const
{
    msg().set_prisoner( IsPrisoner() );
    const MIL_Army_ABC* pArmySurrenderedTo = GetArmySurrenderedTo();
    msg().mutable_surrendered_unit()->set_id( pArmySurrenderedTo ? pArmySurrenderedTo->GetID() : 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::UpdateLodging
// Created: MMC 2011-09-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::UpdateLodging( unsigned int nbrHumansCampManaged )
{
    nbrHumansLodgingManaged_ = nbrHumansCampManaged;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::IsPrisoner
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::IsPrisoner() const
{
    return bPrisoner_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Execute
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::Execute(moving::MoveComputer_ABC& algorithm) const
{
    if(IsSurrendered())
        algorithm.NotifySurrendered();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::GetNbrHumansCampManaged
// Created: MMC 2011-09-14
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Surrender::GetNbrHumansCampManaged() const
{
    return nbrHumansLodgingManaged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::GetNbrHumansCampUnmanaged
// Created: MMC 2011-09-14
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Surrender::GetNbrHumansCampUnmanaged() const
{
    const PHY_RolePion_Composantes& composantes = pion_.GetRole< PHY_RolePion_Composantes >();
    unsigned int nbrUsableHumans = composantes.GetNbrUsableHumans();
    if ( nbrUsableHumans == 0 )
        return 0;
    if ( nbrHumansLodgingManaged_ > nbrUsableHumans )
        return nbrUsableHumans;

    return (nbrUsableHumans - nbrHumansLodgingManaged_);
}

} // namespace surrender
