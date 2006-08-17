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
#include "MIL_OrderConduite_Pion_AppliquerFeux.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_OrderConduite_Pion_AppliquerFeux::nDIAMunitionsIdx_ = 0 ;
int MIL_OrderConduite_Pion_AppliquerFeux::nDIANbITIdx_ = 0 ;
int MIL_OrderConduite_Pion_AppliquerFeux::nDIACibleIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_AppliquerFeux::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Pion_AppliquerFeux::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAMunitionsIdx_ = DEC_Tools::InitializeDIAField( "munitions_", diaType );
    nDIANbITIdx_ = DEC_Tools::InitializeDIAField( "nbIT_", diaType );
    nDIACibleIdx_ = DEC_Tools::InitializeDIAField( "cible_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_AppliquerFeux constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Pion_AppliquerFeux::MIL_OrderConduite_Pion_AppliquerFeux( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup )
    : MIL_OrderConduite_ABC( type, pKnowledgeGroup )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_AppliquerFeux destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Pion_AppliquerFeux::~MIL_OrderConduite_Pion_AppliquerFeux()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_AppliquerFeux::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Pion_AppliquerFeux::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_Pion_AppliquerFeux& asnMission = *asnMsg.u.order_conduite_pion_appliquer_feux;
    if( !NET_ASN_Tools::CopyDotation( asnMission.munitions, GetVariable( nDIAMunitionsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNumeric( asnMission.nb_IT, GetVariable( nDIANbITIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.cible, GetVariable( nDIACibleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_AppliquerFeux::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Pion_AppliquerFeux::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyDotation( diaParams[nCurDIAParamIdx++], GetVariable( nDIAMunitionsIdx_));
    NET_ASN_Tools::CopyNumeric( diaParams[nCurDIAParamIdx++], GetVariable( nDIANbITIdx_));
    NET_ASN_Tools::CopyPoint( diaParams[nCurDIAParamIdx++], GetVariable( nDIACibleIdx_));

}
