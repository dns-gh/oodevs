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
#include "MIL_OrderConduite_ModifierPrioritesTactiquesBlesses.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_OrderConduite_ModifierPrioritesTactiquesBlesses::nDIAOrderConduiteModifierPrioritesTactiquesBlessesIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierPrioritesTactiquesBlesses::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_ModifierPrioritesTactiquesBlesses::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduiteModifierPrioritesTactiquesBlessesIdx_ = DEC_Tools::InitializeDIAField( "orderConduiteModifierPrioritesTactiquesBlesses_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierPrioritesTactiquesBlesses constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_ModifierPrioritesTactiquesBlesses::MIL_OrderConduite_ModifierPrioritesTactiquesBlesses( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup )
    : MIL_OrderConduite_ABC( type, pKnowledgeGroup )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierPrioritesTactiquesBlesses destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_ModifierPrioritesTactiquesBlesses::~MIL_OrderConduite_ModifierPrioritesTactiquesBlesses()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierPrioritesTactiquesBlesses::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_ModifierPrioritesTactiquesBlesses::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_ModifierPrioritesTactiquesBlesses& asnMission = *asnMsg.u.order_conduite_modifier_priorites_tactiques_blesses;
    if( !NET_ASN_Tools::CopyAutomateList( asnMission, GetVariable( nDIAOrderConduiteModifierPrioritesTactiquesBlessesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierPrioritesTactiquesBlesses::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_ModifierPrioritesTactiquesBlesses::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyAutomateList( diaParams[nCurDIAParamIdx++], GetVariable( nDIAOrderConduiteModifierPrioritesTactiquesBlessesIdx_));

}
