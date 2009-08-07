// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Circulation/MIL_AgentTypePionCIRCULATION.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 11:16 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionCIRCULATION.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionCIRCULATION.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <directia/Brain.h>

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCIRCULATION constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionCIRCULATION::MIL_AgentTypePionCIRCULATION( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCIRCULATION destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionCIRCULATION::~MIL_AgentTypePionCIRCULATION()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCIRCULATION::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionCIRCULATION::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionCIRCULATION( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCIRCULATION::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePionCIRCULATION::RegisterFunctions( directia::Brain& brain, MIL_AgentPion& agent ) const
{    
    brain.RegisterFunction( "DEC_Circulation_EquiperItineraireLogistique", 
        boost::function< int( int ) >( boost::bind( &DEC_KnowledgeObjectFunctions::EquipLogisticRoute, boost::cref( agent ), _1 ) ) );
}
