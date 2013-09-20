//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_PerceptionCoupDeSonde_h_
#define __PHY_PerceptionCoupDeSonde_h_

#include "PHY_Perception_ABC.h"

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_PerceptionCoupDeSonde : public PHY_Perception_ABC
{
public:
    explicit PHY_PerceptionCoupDeSonde( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionCoupDeSonde();

    //! @name Execution
    //@{
    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    virtual void FinalizePerception();
    //@}

private:
    const double rLength_;
    const double rWidth_;
};

#endif // __PHY_PerceptionCoupDeSonde_h_
