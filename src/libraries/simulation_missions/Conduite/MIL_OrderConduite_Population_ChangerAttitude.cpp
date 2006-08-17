// *****************************************************************************
//
// $Created: AGE 2004-09-15 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Population_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:52 $
// $Revision: 5 $
// $Workfile: AGR_OrderConduite_Population_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"
#include "MIL_OrderConduite_Population_ChangerAttitude.h"

#include "simulation_kernel/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_OrderConduite_Population_ChangerAttitude::nDIAOrderConduitePopulationChangerAttitudeIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Population_ChangerAttitude::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Population_ChangerAttitude::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduitePopulationChangerAttitudeIdx_ = DEC_Tools::InitializeDIAField( "orderConduitePopulationChangerAttitude_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Population_ChangerAttitude constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Population_ChangerAttitude::MIL_OrderConduite_Population_ChangerAttitude( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* /*pKnowledgeGroup*/ )
    : MIL_OrderConduite_ABC( type, 0 )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Population_ChangerAttitude destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Population_ChangerAttitude::~MIL_OrderConduite_Population_ChangerAttitude()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Population_ChangerAttitude::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Population_ChangerAttitude::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_Population_ChangerAttitude& asnMission = asnMsg.u.order_conduite_population_changer_attitude;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission, GetVariable( nDIAOrderConduitePopulationChangerAttitudeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Population_ChangerAttitude::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Population_ChangerAttitude::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyEnumeration( diaParams[nCurDIAParamIdx++], GetVariable( nDIAOrderConduitePopulationChangerAttitudeIdx_));

}
