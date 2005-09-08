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
#include "MIL_OrderConduite_$OrderName$.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"

$StaticMemberInit$


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_$OrderName$::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_$OrderName$::InitializeDIA( const MIL_OrderConduiteType& type )
{
$StaticMemberScriptInit$
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_$OrderName$ constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_$OrderName$::MIL_OrderConduite_$OrderName$( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : MIL_OrderConduite_ABC( knowledgeGroup, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_$OrderName$ destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_$OrderName$::~MIL_OrderConduite_$OrderName$()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_$OrderName$::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_$OrderName$::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

$InitMembersFromAsn$
    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_$OrderName$::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_$OrderName$::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

$InitMemberFromDIA$
}
