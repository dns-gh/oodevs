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
class PHY_PerceptionLevel;
class PHY_RoleInterface_Perceiver;

namespace detection
{
    class DetectionComputerFactory_ABC;
}
//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_Perception_ABC : private boost::noncopyable
{
public:
    explicit PHY_Perception_ABC( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_Perception_ABC();

    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents, const detection::DetectionComputerFactory_ABC& detectionComputerFactory );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const;

    virtual void                       Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    virtual const PHY_PerceptionLevel& Compute( const MIL_Object_ABC&   object ) const;

    virtual void                       Execute( const TER_PopulationFlow_ABC::T_ConstPopulationFlowVector& perceivableFlows );

    virtual void                       Execute( const TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector perceivableConcentrations );
    virtual const PHY_PerceptionLevel& Compute( const MIL_PopulationConcentration& concentration ) const;

    virtual void FinalizePerception() = 0;

    static int GetPerceptionId();
    //@}

protected:
    //! @name Tools
    //@{
    const MT_Vector2D& GetPerceiverPosition () const;
    const MT_Vector2D& GetPerceiverDirection() const; // Direction de déplacement
    //@}

protected:
    PHY_RoleInterface_Perceiver& perceiver_;
};

#endif // __PHY_Perception_ABC_h_
