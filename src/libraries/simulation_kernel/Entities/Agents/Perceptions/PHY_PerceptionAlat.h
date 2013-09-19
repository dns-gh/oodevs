//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_PerceptionAlat_h_
#define __PHY_PerceptionAlat_h_

#include "PHY_Perception_ABC.h"
#include "simulation_terrain/TER_Localisation.h"

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_PerceptionAlat : public PHY_Perception_ABC
{
public:
             PHY_PerceptionAlat( PHY_RoleInterface_Perceiver& perceiver, const TER_Localisation& localisation );
    virtual ~PHY_PerceptionAlat();

    //! @name Execution
    //@{
    virtual void Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual void FinalizePerception();
    //@}

private:
    TER_Localisation localisation_;
};

#endif // __PHY_PerceptionAlat_h_
