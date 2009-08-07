// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Nbc/MIL_AgentTypePionNBC.cpp $
// $Author: Nld $
// $Modtime: 25/03/05 16:40 $
// $Revision: 5 $
// $Workfile: MIL_AgentTypePionNBC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionNBC.h"
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"
#include "Decision/Functions/DEC_AgentFunctions.h"
#include "Decision/DEC_Tools.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <directia/Brain.h>

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionNBC constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionNBC::MIL_AgentTypePionNBC( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{
    
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionNBC destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionNBC::~MIL_AgentTypePionNBC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionNBC::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionNBC::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionNBC( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionNBC::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePionNBC::RegisterFunctions( directia::Brain& brain, MIL_AgentPion& agent ) const
{
    brain.RegisterFunction( "DEC_DecontaminerZone",
        boost::function< void( const TER_Localisation* ) >( boost::bind( &DEC_KnowledgeObjectFunctions::DecontaminateZone, boost::cref( agent ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_SeDecontaminer", boost::bind( &DEC_AgentFunctions::SelfDecontaminate, boost::ref( agent ) ) );
}
