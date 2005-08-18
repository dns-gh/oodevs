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
#include "MIL_OrderConduite_PasserEnSilenceRadio.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"




//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_PasserEnSilenceRadio::InitializeDIA
// Created: AGR 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_PasserEnSilenceRadio::InitializeDIA( const MIL_OrderConduiteType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_PasserEnSilenceRadio constructor
// Created: AGR 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_OrderConduite_PasserEnSilenceRadio::MIL_OrderConduite_PasserEnSilenceRadio( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : MIL_OrderConduite_ABC( knowledgeGroup, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_PasserEnSilenceRadio destructor
// Created: AGR 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_OrderConduite_PasserEnSilenceRadio::~MIL_OrderConduite_PasserEnSilenceRadio()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_PasserEnSilenceRadio::Initialize
// Created: AGR 2005-08-17 - 16:30:44
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_PasserEnSilenceRadio::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 


    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_PasserEnSilenceRadio::Initialize
// Created: AGR 2005-08-17 - 16:30:44
// -----------------------------------------------------------------------------
void MIL_OrderConduite_PasserEnSilenceRadio::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );


}
