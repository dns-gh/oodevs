//*****************************************************************************
//
// $Created: AGN 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Genie/DEC_Gen_Object.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 17:00 $
// $Revision: 3 $
// $Workfile: DEC_Gen_Object.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Gen_Object.h"
#include "MIL_AgentServer.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_AsnException.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( const Common::MsgPlannedWork& asn, const MIL_EntityManager_ABC& entityManager )
    : pType_             ( &entityManager.FindObjectType( asn.type() ) )
    , localisation_      ()
    , pObstacleType_     ( asn.type_obstacle() )
    , rDensity_          ( asn.densite() )
    , nMinesActivityTime_( asn.activity_time() )
    , pTC2_              ( 0 )    
{
    if( !pType_ )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_mission_parameters );

    if( !NET_ASN_Tools::ReadLocation( asn.position(), localisation_ ) )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_mission_parameters );

    if( asn.tc2() != 0 )
    {
        pTC2_ = entityManager.FindAutomate( asn.tc2() );
        if( !pTC2_ )
            throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_mission_parameters );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( const DEC_Gen_Object& rhs )
    : pType_             ( rhs.pType_ )
    , localisation_      ( rhs.localisation_ )
    , pObstacleType_     ( rhs.pObstacleType_ )
    , rDensity_          ( rhs.rDensity_ )
    , nMinesActivityTime_( rhs.nMinesActivityTime_ )
    , pTC2_              ( rhs.pTC2_ )      
{

}

//-----------------------------------------------------------------------------
// Name: DEC_Gen_Object destructor
// Created: AGN 03-08-27
//-----------------------------------------------------------------------------
DEC_Gen_Object::~DEC_Gen_Object()
{    
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::operator=
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
DEC_Gen_Object& DEC_Gen_Object::operator=( const DEC_Gen_Object& rhs )
{
    pType_              = rhs.pType_;
    localisation_       = rhs.localisation_;
    pObstacleType_      = rhs.pObstacleType_;
    rDensity_           = rhs.rDensity_;
    nMinesActivityTime_ = rhs.nMinesActivityTime_;
    pTC2_               = rhs.pTC2_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::Serialize
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
void DEC_Gen_Object::Serialize( Common::MsgPlannedWork& asn ) const
{
    asn.set_type( pType_->GetName().c_str() );
    asn.set_type_obstacle( pObstacleType_ );
    asn.set_tc2( pTC2_ ? pTC2_->GetID() : 0 );
    asn.set_densite( rDensity_ );
    asn.set_activity_time( nMinesActivityTime_ );
    NET_ASN_Tools::WriteLocation( localisation_, *asn.mutable_position() );
}
