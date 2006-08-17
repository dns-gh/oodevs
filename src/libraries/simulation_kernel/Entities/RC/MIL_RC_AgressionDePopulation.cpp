// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_AgressionDePopulation.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:21 $
// $Revision: 5 $
// $Workfile: MIL_RC_AgressionDePopulation.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_AgressionDePopulation.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Populations/MIL_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_AgressionDePopulation constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_AgressionDePopulation::MIL_RC_AgressionDePopulation( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_AgressionDePopulation::~MIL_RC_AgressionDePopulation()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_AgressionDePopulation destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_AgressionDePopulation::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissancePopulation( diaParams[ 1 ] ) );
    
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyPopulationKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_agression_de_population, sender.GetKnowledgeGroup() );
    asnMsg.Send(); 
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_AgressionDePopulation destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_AgressionDePopulation::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissancePopulation( diaParams[ 1 ] ) );
    
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyPopulationKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_agression_de_population, sender.GetKnowledgeGroup() );
    asnMsg.Send(); 
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_AgressionDePopulation::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_AgressionDePopulation::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissancePopulation( diaParams[ 1 ] ) );
    assert( false );    
}

