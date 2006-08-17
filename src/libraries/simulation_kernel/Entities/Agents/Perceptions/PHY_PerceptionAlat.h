//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_PerceptionAlat_h_
#define __PHY_PerceptionAlat_h_

#include "MIL.h"

#include "PHY_Perception_ABC.h"

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_PerceptionAlat : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionAlat )

public:
             PHY_PerceptionAlat( PHY_RolePion_Perceiver& perceiver, const TER_Localisation& localisation );
    virtual ~PHY_PerceptionAlat();

    //! @name Execution
    //@{
    virtual void Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    //@}

private:
    TER_Localisation localisation_;
};

#include "PHY_PerceptionAlat.inl"

#endif // __PHY_PerceptionAlat_h_
