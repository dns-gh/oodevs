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

#include "Missions_pch.h"
#include "MIL_OrderConduite_ModifierTempsBordeeSante.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_OrderConduite_ModifierTempsBordeeSante::nDIAOrderConduiteModifierTempsBordeeSanteIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierTempsBordeeSante::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_ModifierTempsBordeeSante::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduiteModifierTempsBordeeSanteIdx_ = DEC_Tools::InitializeDIAField( "orderConduiteModifierTempsBordeeSante_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierTempsBordeeSante constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_ModifierTempsBordeeSante::MIL_OrderConduite_ModifierTempsBordeeSante( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : MIL_OrderConduite_ABC( knowledgeGroup, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierTempsBordeeSante destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_ModifierTempsBordeeSante::~MIL_OrderConduite_ModifierTempsBordeeSante()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierTempsBordeeSante::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_ModifierTempsBordeeSante::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_ModifierTempsBordeeSante& asnMission = asnMsg.u.order_conduite_modifier_temps_bordee_sante;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission, GetVariable( nDIAOrderConduiteModifierTempsBordeeSanteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierTempsBordeeSante::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_ModifierTempsBordeeSante::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyEnumeration( diaParams[nCurDIAParamIdx++], GetVariable( nDIAOrderConduiteModifierTempsBordeeSanteIdx_));

}
