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

#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/MIL_EntityManager.h"
#include "Network/NET_ASN_Tools.h"
#include "MIL_AgentServer.h"

//-----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: AGN 03-08-27
//-----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object()
    : pType_             ( 0 )
    , localisation_      ()
    , nPreliminaire_     ( 0 )
    , rDensity_          ( 0. )
    , nMinesActivityTime_( 0 )
    , pTC2_              ( 0 )    
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object( const DEC_Gen_Object& rhs )
    : pType_             ( rhs.pType_ )
    , localisation_      ( rhs.localisation_ )
    , nPreliminaire_     ( rhs.nPreliminaire_ )
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
}

//-----------------------------------------------------------------------------
// Name: DEC_Gen_Object::Initialize
// Created: AGN 03-08-27
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode DEC_Gen_Object::Initialize( const ASN1T_MissionGenObject& asn )
{
    pType_ = MIL_RealObjectType::Find( asn.type );
    if( !pType_ )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    if( !NET_ASN_Tools::ReadLocation( asn.position, localisation_ ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    nPreliminaire_      = asn.preliminaire;
    rDensity_           = asn.densite;
    nMinesActivityTime_ = asn.delai_activite_mines;

    if( asn.tc2 != 0 )
    {
        pTC2_ = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asn.tc2 );
        if( !pTC2_ )
            return EnumOrderErrorCode::error_invalid_mission_parameters;
    }
    return EnumOrderErrorCode::no_error;
}
