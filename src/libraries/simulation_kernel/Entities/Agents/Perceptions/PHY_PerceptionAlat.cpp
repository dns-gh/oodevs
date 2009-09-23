//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionAlat.h"
#include "PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "MIL_AgentServer.h"
#include "simulation_terrain/TER_World.h"

#include "simulation_kernel/DetectionComputer_ABC.h"
#include "simulation_kernel/DetectionComputerFactory_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionAlat::PHY_PerceptionAlat( PHY_RoleInterface_Perceiver& perceiver, const TER_Localisation& localisation )
    : PHY_Perception_ABC( perceiver )
    , localisation_     ( localisation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionAlat::~PHY_PerceptionAlat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat::Execute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_PerceptionAlat::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/, const detection::DetectionComputerFactory_ABC& detectionComputerFactory )
{
    const PHY_RoleInterface_Location& perceiverRoleLocation = perceiver_.GetPion().GetRole< PHY_RoleInterface_Location >();
    const MT_Float               rDetectionSemiHeight  = perceiver_.GetMaxAgentPerceptionDistance() / 2.;
    const PHY_RawVisionData&     rawVisionData         = MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData();

    // Recherche des pions dans la localisation
    TER_Agent_ABC::T_AgentPtrVector agentsDetected;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( localisation_, agentsDetected );
   
    // Enregistrement des pions vus
    for( TER_Agent_ABC::CIT_AgentPtrVector itAgent = agentsDetected.begin(); itAgent != agentsDetected.end(); ++itAgent )
    {        
        PHY_RoleInterface_Location& targetRoleLocation = static_cast< PHY_RoleInterface_Location& >( **itAgent );  
        MIL_Agent_ABC& target = targetRoleLocation.GetAgent();
        detection::DetectionComputer_ABC& detectionComputer = detectionComputerFactory.Create( target );
        perceiver_.GetPion().Execute( detectionComputer );
        target.Execute( detectionComputer );

        if( detectionComputer.CanBeSeen() && fabs( ( targetRoleLocation.GetPosition() - perceiverRoleLocation.GetPosition() ) * perceiverRoleLocation.GetDirection() ) <= rDetectionSemiHeight )
        {
            const bool bPerceptionDelayed = ( rawVisionData.GetVisionObject( targetRoleLocation.GetPosition() ) != PHY_RawVisionData::eVisionEmpty );
            perceiver_.NotifyPerception( targetRoleLocation.GetAgent(), PHY_PerceptionLevel::recognized_, bPerceptionDelayed );
        }
    }
}


