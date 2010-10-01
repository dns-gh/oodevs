// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Alat/MIL_AgentTypePionALAT.cpp $
// $Author: Jvt $
// $Modtime: 24/05/05 10:17 $
// $Revision: 19 $
// $Workfile: MIL_AgentTypePionALAT.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypePionALAT.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_PerceptionFunctions.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <directia/brain/Brain.h>

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionALAT constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionALAT::MIL_AgentTypePionALAT( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePion( strName, strType, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionALAT destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionALAT::~MIL_AgentTypePionALAT()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionALAT::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePionALAT( strName, strType, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionALAT::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePionALAT::RegisterFunctions( directia::brain::Brain& brain, MIL_Agent_ABC& agent ) const
{
    brain[ "DEC_ALAT_ActiverReconnaissance" ] =
        boost::function< void( const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecoAlat, boost::ref( agent ), _1 ) );
    brain[ "DEC_ALAT_DesactiverReconnaissance" ] = boost::bind( &DEC_PerceptionFunctions::DisableRecoAlat, boost::ref( agent ) );
    brain[ "DEC_ALAT_ReconnaissanceNonVuTerminee" ] = boost::bind( &DEC_PerceptionFunctions::HasNoDelayedPeceptions, boost::cref( agent ) );
    brain[ "DEC_Perception_ActiverSurveillance" ] =
        boost::function< int( const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableSurveillanceLocalisation, boost::ref( agent ), _1 ) );
    brain[ "DEC_Perception_DesactiverSurveillance" ] =
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableSurveillanceLocalisation, boost::ref( agent ), _1 ) );
}
