// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Transported/PHY_RolePion_Transported.cpp $
// $Author: Jvt $
// $Modtime: 12/05/05 17:48 $
// $Revision: 9 $
// $Workfile: PHY_RolePion_Transported.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Messages.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Transported, "PHY_RolePion_Transported" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Transported* role, const unsigned int /*version*/ )
{
    archive << role->pPion_;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Transported* role, const unsigned int /*version*/ )
{
	MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_Transported( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Transported::PHY_RolePion_Transported( MIL_AgentPion& pion )
    : pPion_                       ( &pion )
    , bHasChanged_                 ( true )
    , pTransporter_                ( 0 )
    , vLoadingPosition_            ()
    , vHumanTransporterPosition_   ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Transported::~PHY_RolePion_Transported()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::load
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< MIL_Agent_ABC*& >( pTransporter_ )
         >> vLoadingPosition_
         >> vHumanTransporterPosition_;
         
//    if ( IsTransported() )
//        CancelTransport();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::save
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << pTransporter_
         << vLoadingPosition_
         << vHumanTransporterPosition_;
}

namespace
{
    struct sTransportedData : private boost::noncopyable
    {
        sTransportedData( const MIL_AgentPion& transported, const bool bTransportOnlyLoadable )
            : rTotalWeight_             ( 0. )
            , rHeaviestComposanteWeight_( 0. )
            , bTransportOnlyLoadable_   ( bTransportOnlyLoadable )
        {
            transported.GetRole< PHY_RolePion_Composantes >().Apply( *this );
        }

        void operator() ( const PHY_ComposantePion& composante )
        {
            if( composante.CanBeTransported() && ( !bTransportOnlyLoadable_ || composante.CanBeLoaded() ) )
            {
                rTotalWeight_             += composante.GetWeight();
                rHeaviestComposanteWeight_ = std::max( rHeaviestComposanteWeight_, composante.GetWeight() );
            }
        }

              MT_Float rTotalWeight_;
              MT_Float rHeaviestComposanteWeight_;
        const bool     bTransportOnlyLoadable_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::GetTransportWeight
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::GetTransportWeight( bool bTransportOnlyLoadable, MT_Float& rTotalWeight, MT_Float& rHeaviestComposanteWeight ) const
{
    assert( pPion_ );
    sTransportedData data( *pPion_, bTransportOnlyLoadable );

    rTotalWeight              = data.rTotalWeight_;
    rHeaviestComposanteWeight = data.rHeaviestComposanteWeight_;    
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::CancelTransport
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
bool PHY_RolePion_Transported::CancelTransport( const MIL_Agent_ABC& transporter )
{
    assert( pPion_ );
    if( pTransporter_ != &transporter )
        return false;
    pPion_->GetRole< PHY_RoleInterface_Location >().Show( vLoadingPosition_ );
    pTransporter_ = 0;
    bHasChanged_ = true;
    vLoadingPosition_.Reset();  
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::LoadForTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
bool PHY_RolePion_Transported::LoadForTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable )
{
    assert( pPion_ );
    if( pTransporter_ && pTransporter_ == &transporter)
        return true;
    if( pTransporter_ )
        return false;

    pTransporter_ = &transporter;

    pPion_->GetRole< PHY_RoleInterface_Reinforcement >().CancelReinforcement();     
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pPion_->GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    while( !reinforcements.empty() )
        (**reinforcements.begin()).GetRole< PHY_RoleInterface_Reinforcement >().CancelReinforcement();
    
    pPion_->GetRole< PHY_RoleAction_Loading >().ForceUnloadedState ();
    pPion_->GetRole< PHY_RoleInterface_Location  >().Hide               ();   
    vLoadingPosition_= transporter.GetRole< PHY_RoleInterface_Location >().GetPosition();
    if( bTransportOnlyLoadable && vHumanTransporterPosition_.IsZero() )
        vHumanTransporterPosition_ = vLoadingPosition_;
    bHasChanged_ = true;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::UnloadFromTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
bool PHY_RolePion_Transported::UnloadFromTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable )
{
    assert( pPion_ );
    if( pTransporter_ != &transporter )
        return false;

    assert( pTransporter_ );
    pPion_->GetRole< PHY_RoleAction_Loading >().ForceUnloadedState ();
    pPion_->GetRole< PHY_RoleInterface_Location  >().Show( pTransporter_->GetRole< PHY_RoleInterface_Location >().GetPosition() );
    pTransporter_ = 0;
    bHasChanged_  = true;
    vLoadingPosition_.Reset();
    if( !bTransportOnlyLoadable )
        vHumanTransporterPosition_.Reset();
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::DisableHumanTransporters
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::DisableHumanTransporters( const MT_Vector2D& vPos )
{
    assert( pPion_ );
    if ( vHumanTransporterPosition_.IsZero() )
    {
        pPion_->GetRole< PHY_RoleAction_Loading >().ForceUnloadedState();
        vHumanTransporterPosition_ = vPos;
        bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::RecoverHumanTransporters
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::RecoverHumanTransporters()
{
    if( pTransporter_ )
        return;

    vHumanTransporterPosition_.Reset();
    bHasChanged_ = true;
}

namespace
{
    struct sTransporterComposantePresent
    {
        sTransporterComposantePresent() : bComposantePresent_( false ) {}

        void operator () ( const PHY_ComposantePion& composante )
        {
            bComposantePresent_ |= composante.CanTransportHumans();
        }

        bool bComposantePresent_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::HasHumanTransportersReady
// Created: JVT 2005-01-31
// Modified: JVT 2005-02-03
// -----------------------------------------------------------------------------
bool PHY_RolePion_Transported::HasHumanTransportersReady() const
{
    assert( pPion_ );
    sTransporterComposantePresent func;
    pPion_->GetRole< PHY_RolePion_Composantes >().Apply( func );
    
    return vHumanTransporterPosition_.IsZero() && func.bComposantePresent_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::Update( bool /*bIsDead*/ )
{
    assert( pPion_ );
    if( !pTransporter_ )
        return;
    pPion_->GetRole< PHY_RoleInterface_Location >().Follow( *pTransporter_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::Clean
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::SendFullState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg().m.pion_transporteurPresent = 1;
    msg().pion_transporteur          = pTransporter_ ? pTransporter_->GetID() : 0;

    msg().m.transporteurs_disponiblesPresent  = 1;
    msg().transporteurs_disponibles           = !HasHumanTransportersToRecover();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::SendChangedState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}   

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::HasChanged
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Transported::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::IsTransported
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Transported::IsTransported() const
{
    return pTransporter_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::HasHumanTransportersToRecover
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Transported::HasHumanTransportersToRecover() const
{
    return !vHumanTransporterPosition_.IsZero();
}
