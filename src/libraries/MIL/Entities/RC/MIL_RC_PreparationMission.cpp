// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_PreparationMission.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:21 $
// $Revision: 4 $
// $Workfile: MIL_RC_PreparationMission.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_PreparationMission.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Populations/MIL_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_PreparationMission constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_PreparationMission::MIL_RC_PreparationMission( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_PreparationMission::~MIL_RC_PreparationMission()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_PreparationMission destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PreparationMission::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );    
    
    ASN1T_CR_PreparationMission asnCR;    
    if( diaParams.GetParameters().size() > 1 )
    {
        asnCR.m.delaiPresent = 1;
        NET_ASN_Tools::CopyNumeric( diaParams.GetParameter( 1 ), asnCR.delai );
    }
    asnMsg.GetAsnMsg().cr.u.cr_preparation_mission = &asnCR;     
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_PreparationMission destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PreparationMission::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );    
    
    ASN1T_CR_PreparationMission asnCR;    
    if( diaParams.GetParameters().size() > 1 )
    {
        asnCR.m.delaiPresent = 1;
        NET_ASN_Tools::CopyNumeric( diaParams.GetParameter( 1 ), asnCR.delai );
    }
    asnMsg.GetAsnMsg().cr.u.cr_preparation_mission = &asnCR;     
    asnMsg.Send();       
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_PreparationMission::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PreparationMission::Send( const MIL_Population& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );    
    
    ASN1T_CR_PreparationMission asnCR;    
    if( diaParams.GetParameters().size() > 1 )
    {
        asnCR.m.delaiPresent = 1;
        NET_ASN_Tools::CopyNumeric( diaParams.GetParameter( 1 ), asnCR.delai );
    }
    asnMsg.GetAsnMsg().cr.u.cr_preparation_mission = &asnCR;     
    asnMsg.Send();  
}