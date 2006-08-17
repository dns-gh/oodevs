// *****************************************************************************
//
// $Created: AGE 2004-09-15 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Automate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 16:14 $
// $Revision: 5 $
// $Workfile: AGR_OrderConduite_Automate_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"
#include "MIL_OrderConduite_Automate_ReconnaitreZoneImplantation.h"

#include "simulation_kernel/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_OrderConduite_Automate_ReconnaitreZoneImplantation::nDIAOrderConduiteAutomateReconnaitreZoneImplantationIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_ReconnaitreZoneImplantation::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Automate_ReconnaitreZoneImplantation::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduiteAutomateReconnaitreZoneImplantationIdx_ = DEC_Tools::InitializeDIAField( "orderConduiteAutomateReconnaitreZoneImplantation_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_ReconnaitreZoneImplantation constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Automate_ReconnaitreZoneImplantation::MIL_OrderConduite_Automate_ReconnaitreZoneImplantation( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup )
    : MIL_OrderConduite_ABC( type, pKnowledgeGroup )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_ReconnaitreZoneImplantation destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Automate_ReconnaitreZoneImplantation::~MIL_OrderConduite_Automate_ReconnaitreZoneImplantation()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_ReconnaitreZoneImplantation::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Automate_ReconnaitreZoneImplantation::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_Automate_ReconnaitreZoneImplantation& asnMission = *asnMsg.u.order_conduite_automate_reconnaitre_zone_implantation;
    if( !NET_ASN_Tools::CopyPoint( asnMission, GetVariable( nDIAOrderConduiteAutomateReconnaitreZoneImplantationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_ReconnaitreZoneImplantation::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Automate_ReconnaitreZoneImplantation::Initialize( DIA_Parameters& /*diaParams*/, uint /*nCurDIAParamIdx*/ )
{
    assert( false );
}
