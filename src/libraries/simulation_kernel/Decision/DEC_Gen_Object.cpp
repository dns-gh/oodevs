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

#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_EntityManager.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_AsnException.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( const ASN1T_PlannedWork& asn )
    : pType_             ( &MIL_AgentServer::GetWorkspace().GetEntityManager().FindObjectType( asn.type ) )
    , localisation_      ()
    , pObstacleType_     ( asn.type_obstacle )
    , rDensity_          ( asn.densite )
    , nMinesActivityTime_( asn.activity_time )
    , pTC2_              ( 0 )    
{
    if( !pType_ )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::ReadLocation( asn.position, localisation_ ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( asn.tc2 != 0 )
    {
        pTC2_ = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asn.tc2 );
        if( !pTC2_ )
            throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
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
void DEC_Gen_Object::operator=( const DEC_Gen_Object& rhs )
{
    pType_              = rhs.pType_;
    localisation_       = rhs.localisation_;
    pObstacleType_      = rhs.pObstacleType_;
    rDensity_           = rhs.rDensity_;
    nMinesActivityTime_ = rhs.nMinesActivityTime_;
    pTC2_               = rhs.pTC2_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::Serialize
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
void DEC_Gen_Object::Serialize( ASN1T_PlannedWork& asn ) const
{
    asn.type                 = pType_->GetName().c_str();
    asn.type_obstacle        = ( ASN1T_EnumDemolitionTargetType )pObstacleType_;
    asn.tc2                  = pTC2_ ? pTC2_->GetID() : 0;
    asn.densite              = rDensity_;
    asn.activity_time        = nMinesActivityTime_;
    NET_ASN_Tools::WriteLocation( localisation_, asn.position );
}
