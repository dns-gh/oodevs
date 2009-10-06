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

#include "simulation_kernel/TransportPermissionComputer_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/MoveComputer_ABC.h"

BOOST_CLASS_EXPORT_GUID( transport::PHY_RolePion_Transported, "PHY_RolePion_Transported" )

namespace transport
{

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Transported* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
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
    : pion_                        ( pion )
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
    file >> ::boost::serialization::base_object< PHY_RoleInterface_Transported >( *this )
         >> const_cast< MIL_Agent_ABC*& >( pTransporter_ )
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
    file << ::boost::serialization::base_object< PHY_RoleInterface_Transported >( *this )
         << pTransporter_
         << vLoadingPosition_
         << vHumanTransporterPosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::CancelTransport
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::CancelTransport( const MIL_Agent_ABC& transporter )
{

    if( pTransporter_ != &transporter )
        return ;//false;
    pion_.GetRole< PHY_RoleInterface_Location >().Show( vLoadingPosition_ );
    pTransporter_ = 0;
    bHasChanged_ = true;
    vLoadingPosition_.Reset();  
    return ;//true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::LoadForTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::LoadForTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable )
{

    if( pTransporter_ && pTransporter_ == &transporter)
        return ;//true;
    if( pTransporter_ )
        return ;//false;

    pTransporter_ = &transporter;

    pion_.GetRole< PHY_RoleInterface_Location  >().Hide               ();   
    vLoadingPosition_= transporter.GetRole< PHY_RoleInterface_Location >().GetPosition();
    if( bTransportOnlyLoadable && vHumanTransporterPosition_.IsZero() )
        vHumanTransporterPosition_ = vLoadingPosition_;
    bHasChanged_ = true;
    return ;//true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::UnloadFromTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::UnloadFromTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable )
{

    if( pTransporter_ != &transporter )
        return ; //false;

    assert( pTransporter_ );
    pion_.GetRole< PHY_RoleInterface_Location  >().Show( pTransporter_->GetRole< PHY_RoleInterface_Location >().GetPosition() );
    pTransporter_ = 0;
    bHasChanged_  = true;
    vLoadingPosition_.Reset();
    if( !bTransportOnlyLoadable )
        vHumanTransporterPosition_.Reset();
    return ;//true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::DisableHumanTransporters
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::DisableHumanTransporters( const MT_Vector2D& vPos )
{

    if ( vHumanTransporterPosition_.IsZero() )
    {
        pion_.GetRole< PHY_RoleAction_Loading >().ForceUnloadedState();
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

    sTransporterComposantePresent func;
    pion_.GetRole< PHY_RolePion_Composantes >().Apply( func );
    
    return vHumanTransporterPosition_.IsZero() && func.bComposantePresent_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::Update( bool /*bIsDead*/ )
{

    if( !pTransporter_ )
        return;
    pion_.GetRole< PHY_RoleInterface_Location >().Follow( *pTransporter_ );

    if( HasChanged() )
    {
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
        pion_.Apply( &transport::TransportChangeNotificationHandler_ABC::NotifyTransportHasChanged );
    }
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::Execute
// Created: AHC 2009-09-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::Execute(TransportPermissionComputer_ABC& alg) const
{
    alg.AllowLoading(!IsTransported());
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::Execute
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::Execute(moving::MoveComputer_ABC& algorithm) const
{
    if(IsTransported())
        algorithm.NotifyTransported();
}
} // namespace transport
