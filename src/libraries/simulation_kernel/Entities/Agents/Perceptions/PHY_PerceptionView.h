//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_PerceptionView_h_
#define __PHY_PerceptionView_h_

#include "PHY_Perception_ABC.h"
class MIL_PopulationFlow;

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_PerceptionView : public PHY_Perception_ABC
{

public:
    explicit PHY_PerceptionView( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionView();

    //! @name Tools
    //@{
    void Enable();
    void Disable();
    //@}
    
    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents, const detection::DetectionComputerFactory_ABC& detectionComputerFactory );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const;

    virtual void                       Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    virtual const PHY_PerceptionLevel& Compute( const MIL_Object_ABC&   object    ) const;

    virtual void                       Execute( const std::vector< const urban::TerrainObject_ABC* >& perceivables );
    virtual const PHY_PerceptionLevel& Compute( const urban::Block& block ) const;

    virtual void                       Execute( const TER_PopulationFlow_ABC::T_PopulationFlowVector& perceivableFlows );
    virtual const PHY_PerceptionLevel& Compute( const MIL_PopulationFlow& flow, T_PointVector& shape ) const;

    virtual void                       Execute( const TER_PopulationConcentration_ABC::T_PopulationConcentrationVector perceivableConcentrations );
    virtual const PHY_PerceptionLevel& Compute( const MIL_PopulationConcentration& concentration ) const;
    //@}

private:
    bool bIsEnabled_;
};

#endif // __PHY_PerceptionView_h_
