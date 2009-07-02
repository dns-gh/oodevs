//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_Perception_ABC_h_
#define __PHY_Perception_ABC_h_

#include "simulation_terrain/TER_Agent_ABC.h"
#include "simulation_terrain/TER_Object_ABC.h"
#include "simulation_terrain/TER_PopulationFlow_ABC.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"

class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class MIL_Agent_ABC;
class MIL_Object_ABC;
class MIL_PopulationConcentration;
class MIL_PopulationFlow;
class PHY_PerceptionLevel;
class PHY_RolePion_Perceiver;

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_Perception_ABC : private boost::noncopyable
{

public:
    explicit PHY_Perception_ABC( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_Perception_ABC();

    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const;

    virtual void                       Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    virtual const PHY_PerceptionLevel& Compute( const MIL_Object_ABC&   object ) const;

    virtual void                       Execute( const TER_PopulationFlow_ABC::T_PopulationFlowVector& perceivableFlows );

    virtual void                       Execute( const TER_PopulationConcentration_ABC::T_PopulationConcentrationVector perceivableConcentrations );
    virtual const PHY_PerceptionLevel& Compute( const MIL_PopulationConcentration& concentration ) const;
    //@}

protected:
    //! @name Tools
    //@{
    const MT_Vector2D& GetPerceiverPosition () const; 
    const MT_Vector2D& GetPerceiverDirection() const; // Direction de déplacement
    //@}

protected:
    PHY_RolePion_Perceiver& perceiver_;
};

#endif // __PHY_Perception_ABC_h_
