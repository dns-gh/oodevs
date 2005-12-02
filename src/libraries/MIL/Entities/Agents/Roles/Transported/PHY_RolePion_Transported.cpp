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

#include "MIL_pch.h"

#include "PHY_RolePion_Transported.h"

#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Messages.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Transported, "PHY_RolePion_Transported" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Transported::PHY_RolePion_Transported( MT_RoleContainer& role )
    : PHY_RoleInterface_Transported( role )
    , bHasChanged_                 ( true )
    , pTransporter_                ( 0 )
    , vLoadingPosition_            ()
    , vHumanTransporterPosition_   ()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RolePion_Transported::PHY_RolePion_Transported()
    : PHY_RoleInterface_Transported()
    , bHasChanged_                 ( true )
    , pTransporter_                ( 0 )
    , vLoadingPosition_            ()
    , vHumanTransporterPosition_   ()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Transported::~PHY_RolePion_Transported()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::load
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Transported >( *this )
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
    file << boost::serialization::base_object< PHY_RoleInterface_Transported >( *this )
         << const_cast< MIL_Agent_ABC*& >( pTransporter_ )
         << vLoadingPosition_
         << vHumanTransporterPosition_;
}

// =============================================================================
// OPERATIONS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::CancelTransport
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::CancelTransport()
{
    if( !pTransporter_ )
        return;

    GetRole< PHY_RolePion_Location >().Show( vLoadingPosition_ );
    pTransporter_ = 0;
    bHasChanged_ = true;
    vLoadingPosition_.Reset();  
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::LoadForTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::LoadForTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable )
{
    if( pTransporter_ == &transporter )
        return;

    assert( !pTransporter_ );
    pTransporter_ = &transporter;

    GetRole< PHY_RolePion_Reinforcement >().CancelReinforcement();     
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    while( !reinforcements.empty() )
        (**reinforcements.begin()).GetRole< PHY_RolePion_Reinforcement >().CancelReinforcement();
    
    GetRole< PHY_RoleAction_Loading >().ForceUnloadedState ();
    GetRole< PHY_RolePion_Location  >().Hide               ();   
    vLoadingPosition_= transporter.GetRole< PHY_RoleInterface_Location >().GetPosition();
    if( bTransportOnlyLoadable && vHumanTransporterPosition_.IsZero() )
        vHumanTransporterPosition_ = vLoadingPosition_;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::DisableHumanTransporters
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::DisableHumanTransporters( const MT_Vector2D& vPos )
{
    if ( vHumanTransporterPosition_.IsZero() )
    {
        GetRole< PHY_RoleAction_Loading >().ForceUnloadedState();
        vHumanTransporterPosition_ = vPos;
        bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::UnloadFromTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::UnloadFromTransport( bool bTransportOnlyLoadable )
{
    assert( pTransporter_ );
    GetRole< PHY_RoleAction_Loading >().ForceUnloadedState ();
    GetRole< PHY_RolePion_Location  >().Show               ( pTransporter_->GetRole< PHY_RoleInterface_Location >().GetPosition() );
    pTransporter_ = 0;
    bHasChanged_  = true;
    vLoadingPosition_.Reset();
    if( !bTransportOnlyLoadable )
        vHumanTransporterPosition_.Reset();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::RecoverHumanTransporters
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::RecoverHumanTransporters()
{
    assert( !pTransporter_ );
    vHumanTransporterPosition_.Reset();
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::ComputeTimes
// Created: NLD 2004-09-13
// Modified: JVT 2005-02-03
// -----------------------------------------------------------------------------
struct sTransportedTimesFunctor
{
    sTransportedTimesFunctor( const MT_Vector2D& vSrcPos, const MT_Vector2D& vDestPos )
        : nApproximatedTravelTime_    ( std::numeric_limits< uint >::max() )
        , vSrcPos_                    ( vSrcPos )
        , vDestPos_                   ( vDestPos )
    {
    }

    void operator() ( const PHY_ComposantePion& composante )
    {
        if ( composante.GetState().IsUsable() && !composante.CanBeLoaded() )
            nApproximatedTravelTime_ = std::min( nApproximatedTravelTime_, composante.ApproximateTravelTime( vSrcPos_, vDestPos_ ) );
    }

    uint nApproximatedTravelTime_;
    
private:
    const MT_Vector2D& vSrcPos_;
    const MT_Vector2D& vDestPos_;
    
    MT_COPYNOTALLOWED( sTransportedTimesFunctor );
};

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::ComputeHumanTransportersRecoveringTime
// Created: NLD 2004-09-13
// Modified: JVT 2005-02-03
// -----------------------------------------------------------------------------
uint PHY_RolePion_Transported::ComputeHumanTransportersRecoveringTime() const
{
    if ( vHumanTransporterPosition_.IsZero() )
        return 0;

    sTransportedTimesFunctor func( vHumanTransporterPosition_, GetRole< PHY_RolePion_Location >().GetPosition() );
    GetRole< PHY_RolePion_Composantes >().Apply( func );
 
    return func.nApproximatedTravelTime_ == std::numeric_limits< uint >::max() ? 0 : func.nApproximatedTravelTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::ComputeHumanTransportersRecoveringTime
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
uint PHY_RolePion_Transported::ComputeHumanTransportersRecoveringTime( const MT_Vector2D& vRecoveringPosition ) const
{
    if ( vHumanTransporterPosition_.IsZero() )
        return 0;
        
    sTransportedTimesFunctor func( vHumanTransporterPosition_, vRecoveringPosition );
    GetRole< PHY_RolePion_Composantes >().Apply( func );
 
    return func.nApproximatedTravelTime_ == std::numeric_limits< uint >::max() ? 0 : func.nApproximatedTravelTime_;
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::ComputeTimes
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
struct sTransporterComposantePresent
{
    sTransporterComposantePresent() : bComposantePresent_( false ) {}

    void operator () ( const PHY_ComposantePion& composante )
    {
        bComposantePresent_ |= ( composante.GetState().IsUsable() && !composante.CanBeLoaded() ); //?? Utiliser CanTransportHumans()
    }

    bool bComposantePresent_;
};

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::HasHumanTransportersReady
// Created: JVT 2005-01-31
// Modified: JVT 2005-02-03
// -----------------------------------------------------------------------------
bool PHY_RolePion_Transported::HasHumanTransportersReady() const
{
    sTransporterComposantePresent func;
    GetRole< PHY_RolePion_Composantes >().Apply( func );
    
    return vHumanTransporterPosition_.IsZero() && func.bComposantePresent_;
}

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::Update( bool /*bIsDead*/ )
{
    if( !pTransporter_ )
        return;
    GetRole< PHY_RolePion_Location >().Follow( *pTransporter_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::Clean
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::Clean()
{
    bHasChanged_ = false;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::SendFullState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg.GetAsnMsg().m.pion_transporteurPresent = 1;
    msg.GetAsnMsg().pion_transporteur          = pTransporter_ ? pTransporter_->GetID() : 0;

    msg.GetAsnMsg().m.transporteurs_disponiblesPresent  = 1;
    msg.GetAsnMsg().transporteurs_disponibles           = !HasHumanTransportersToRecover();
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


