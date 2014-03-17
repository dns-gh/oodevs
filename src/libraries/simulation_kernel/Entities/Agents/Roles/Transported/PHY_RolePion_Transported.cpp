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
#include "AlgorithmsFactories.h"
#include "LocationActionNotificationHandler_ABC.h"
#include "MoveComputer_ABC.h"
#include "NetworkNotificationHandler_ABC.h"
#include "TransportPermissionComputer_ABC.h"
#include "TransportChangeNotificationHandler_ABC.h"
#include "TransportNotificationHandler_ABC.h"
#include "VisionConeNotificationHandler_ABC.h"
#include "OnComponentFunctor_ABC.h"
#include "OnComponentFunctorComputer_ABC.h"
#include "OnComponentFunctorComputerFactory_ABC.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KS_DirectFire.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Vector2D.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( transport::PHY_RolePion_Transported )

using namespace transport;

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RolePion_Transported::PHY_RolePion_Transported()
    : owner_       ( 0 )
    , bHasChanged_ ( true )
    , pTransporter_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Transported::PHY_RolePion_Transported( MIL_AgentPion& pion )
    : owner_       ( &pion )
    , bHasChanged_ ( true )
    , pTransporter_( 0 )
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
void PHY_RolePion_Transported::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> ::boost::serialization::base_object< PHY_RoleInterface_Transported >( *this );
    file >> owner_;
    file >> const_cast< MIL_Agent_ABC*& >( pTransporter_ );
    file >> vLoadingPosition_;
    file >> vHumanTransporterPosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::save
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << ::boost::serialization::base_object< PHY_RoleInterface_Transported >( *this );
    file << owner_;
    file << pTransporter_;
    file << vLoadingPosition_;
    file << vHumanTransporterPosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::CancelTransport
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::CancelTransport( const MIL_Agent_ABC& transporter )
{
    if( pTransporter_ != &transporter )
        return ;//false;
    if( !pTransporter_ || !pTransporter_->GetKnowledge().GetKsDirectFire().IsAttacked() || pTransporter_->GetRole< PHY_RoleAction_Transport >().RemainingWeight( *owner_ ) > 0 )
        owner_->Apply( &location::LocationActionNotificationHandler_ABC::Show, vLoadingPosition_ );
    pTransporter_ = 0;
    bHasChanged_ = true;
    vLoadingPosition_.Reset();
    return ;//true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::LoadForTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::LoadForTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable, bool& bTransportedByAnother )
{
    if( pTransporter_ && pTransporter_ == &transporter)
        return ;//true;
    if( pTransporter_ )
    {
        bTransportedByAnother = true;
        return ;//false;
    }

    pTransporter_ = &transporter;

    owner_->Apply( &location::LocationActionNotificationHandler_ABC::Hide );
    vLoadingPosition_= transporter.GetRole< PHY_RoleInterface_Location >().GetPosition();
    if( bTransportOnlyLoadable && vHumanTransporterPosition_.IsZero() )
        vHumanTransporterPosition_ = vLoadingPosition_;
    bHasChanged_ = true;
    owner_->Apply( &transport::TransportChangeNotificationHandler_ABC::NotifyIsLoadedForTransport );
    return ;//true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::UnloadFromTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::UnloadFromTransport( const MIL_Agent_ABC& transporter, bool /*bTransportOnlyLoadable*/, MT_Vector2D* position )
{
    if( pTransporter_ != &transporter )
        return ; //false;

    assert( pTransporter_ );
    const MT_Vector2D& unloadPosition = position ? *position : pTransporter_->GetRole< PHY_RoleInterface_Location >().GetPosition();
    owner_->Apply( &location::LocationActionNotificationHandler_ABC::Show, unloadPosition );

    pTransporter_ = 0;
    bHasChanged_  = true;
    vLoadingPosition_.Reset();
    owner_->Apply( &transport::TransportChangeNotificationHandler_ABC::NotifyIsUnLoadedForTransport );
    owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    return ;//true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::DisableHumanTransporters
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::DisableHumanTransporters( const MT_Vector2D& vPos )
{
    if( vHumanTransporterPosition_.IsZero() )
    {
        owner_->Apply( &TransportNotificationHandler_ABC::ForceUnloadedState );
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
    owner_->Apply( &transport::TransportNotificationHandler_ABC::RecoverComposantes );
    vHumanTransporterPosition_.Reset();
    bHasChanged_ = true;
}

namespace
{
  class sTransporterComposantePresent : public OnComponentFunctor_ABC
    {
    public:
        sTransporterComposantePresent() : bComposantePresent_( false ) {}

        void operator () ( PHY_ComposantePion& composante )
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
    std::auto_ptr< OnComponentComputer_ABC > computer( owner_->GetAlgorithms().onComponentFunctorComputerFactory_->Create( func ) );
    owner_->Execute( *computer );

    return vHumanTransporterPosition_.IsZero() && func.bComposantePresent_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::Update( bool /*bIsDead*/ )
{
    if( HasChanged() )
    {
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
        owner_->Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
    }
    if( pTransporter_ )
    {
        owner_->Apply( &location::LocationActionNotificationHandler_ABC::Follow, *pTransporter_ );
        owner_->GetKnowledgeGroup()->UpdateKnowledgeFromTransported( *pTransporter_ );
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
void PHY_RolePion_Transported::SendFullState( client::UnitAttributes& msg ) const
{
    msg().mutable_transporting_unit()->set_id( pTransporter_ ? pTransporter_->GetID() : 0 );
    msg().set_transporters_available( !HasHumanTransportersToRecover() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::SendChangedState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::SendChangedState( client::UnitAttributes& msg ) const
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
void PHY_RolePion_Transported::Execute( TransportPermissionComputer_ABC& computer ) const
{
    computer.AllowLoading( !IsTransported() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::Execute
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Transported::Execute( moving::MoveComputer_ABC& computer ) const
{
    if( IsTransported() )
        computer.NotifyTransported();
}
