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
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <directia/Brain.h>

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCirculation constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionCirculation::MIL_AgentTypePionCirculation( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
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
const MIL_AgentTypePion* MIL_AgentTypePionCirculation::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionCirculation( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCirculation::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePionCirculation::RegisterFunctions( directia::Brain& brain, MIL_AgentPion& agent ) const
{    
    brain.RegisterFunction( "DEC_Circulation_EquiperItineraireLogistique", 
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::EquipLogisticRoute, boost::cref( agent ), _1 ) ) );
}
