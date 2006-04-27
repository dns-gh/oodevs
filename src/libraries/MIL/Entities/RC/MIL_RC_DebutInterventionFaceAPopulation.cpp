// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_DebutInterventionFaceAPopulation.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 18:41 $
// $Revision: 1 $
// $Workfile: MIL_RC_DebutInterventionFaceAPopulation.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_DebutInterventionFaceAPopulation.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_DebutInterventionFaceAPopulation constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_DebutInterventionFaceAPopulation::MIL_RC_DebutInterventionFaceAPopulation( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_DebutInterventionFaceAPopulation::~MIL_RC_DebutInterventionFaceAPopulation()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_DebutInterventionFaceAPopulation destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_DebutInterventionFaceAPopulation::Send( const MIL_AgentPion& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );  
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_DebutInterventionFaceAPopulation destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_DebutInterventionFaceAPopulation::Send( const MIL_Automate& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );  
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_DebutInterventionFaceAPopulation::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_DebutInterventionFaceAPopulation::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );  
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_DebutInterventionFaceAPopulation::Send
// Created: NLD 2006-04-26
// -----------------------------------------------------------------------------
void MIL_RC_DebutInterventionFaceAPopulation::Send( const MIL_AgentPion&  sender, E_RcType nType, const MIL_PopulationAttitude& populationAttitude ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.GetAsnMsg().cr.u.cr_debut_intervention_face_a_population = populationAttitude.GetAsnID();
    asnMsg.Send();           
}