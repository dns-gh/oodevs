// *****************************************************************************
//
// $Created: AGE 2004-09-15 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Pion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:52 $
// $Revision: 5 $
// $Workfile: AGR_OrderConduite_Pion_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"
#include "MIL_OrderConduite_ChangerParametresRecompletementALAT.h"

#include "simulation_kernel/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_OrderConduite_ChangerParametresRecompletementALAT::nDIAPorteeActionIdx_ = 0 ;
int MIL_OrderConduite_ChangerParametresRecompletementALAT::nDIAAmbianceMissionIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerParametresRecompletementALAT::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_ChangerParametresRecompletementALAT::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );
    nDIAAmbianceMissionIdx_ = DEC_Tools::InitializeDIAField( "ambianceMission_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerParametresRecompletementALAT constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_ChangerParametresRecompletementALAT::MIL_OrderConduite_ChangerParametresRecompletementALAT( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup )
    : MIL_OrderConduite_ABC( type, pKnowledgeGroup )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerParametresRecompletementALAT destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_ChangerParametresRecompletementALAT::~MIL_OrderConduite_ChangerParametresRecompletementALAT()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerParametresRecompletementALAT::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_ChangerParametresRecompletementALAT::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_ChangerParametresRecompletementALAT& asnMission = *asnMsg.u.order_conduite_changer_parametres_recompletement_alat;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_Action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.ambiance_Mission, GetVariable( nDIAAmbianceMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerParametresRecompletementALAT::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_ChangerParametresRecompletementALAT::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyEnumeration( diaParams[nCurDIAParamIdx++], GetVariable( nDIAPorteeActionIdx_));
    NET_ASN_Tools::CopyEnumeration( diaParams[nCurDIAParamIdx++], GetVariable( nDIAAmbianceMissionIdx_));

}
