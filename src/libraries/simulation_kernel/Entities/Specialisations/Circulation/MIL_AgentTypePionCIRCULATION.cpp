// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Circulation/MIL_AgentTypePionCirculation.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 11:16 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionCirculation.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionCirculation.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Brain.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCirculation constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionCirculation::MIL_AgentTypePionCirculation( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePion( strName, strType, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCirculation destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionCirculation::~MIL_AgentTypePionCirculation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCirculation::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionCirculation::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePionCirculation( strName, strType, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCirculation::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePionCirculation::RegisterFunctions( sword::Brain& brain, MIL_Agent_ABC& /*agent*/ ) const
{
    brain.RegisterFunction( "DEC_Circulation_EquiperItineraireLogistique", &DEC_KnowledgeObjectFunctions::EquipLogisticRoute );
}
