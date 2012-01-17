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
             PHY_PerceptionView( PHY_RoleInterface_Perceiver& perceiver, MIL_Agent_ABC& pion );
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
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const;

    virtual void                       Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    virtual const PHY_PerceptionLevel& Compute( const MIL_Object_ABC&   object    ) const;

    virtual void                       Execute( const TER_PopulationFlow_ABC::T_ConstPopulationFlowVector& perceivableFlows );
    virtual const PHY_PerceptionLevel& Compute( const MIL_PopulationFlow& flow, T_PointVector& shape ) const;

    virtual void                       Execute( const TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector perceivableConcentrations );
    virtual const PHY_PerceptionLevel& Compute( const MIL_PopulationConcentration& concentration ) const;

    virtual void FinalizePerception();
    //@}

private:
    //! @name Helpers
    //@{
    void FinalizeSurfaceAgents();
    void TransfertPerception();
    //@}

    //! @name Types
    //@{
    typedef std::pair< unsigned int, double > T_PerceptionParameterPair;

    typedef std::map< const void*, T_PerceptionParameterPair > T_PerceptionTickMap;
    typedef T_PerceptionTickMap::const_iterator               CIT_PerceptionTickMap;
    typedef T_PerceptionTickMap::iterator                      IT_PerceptionTickMap;
    //@}

private:
    T_PerceptionTickMap perceptionsBuffer_;
    T_PerceptionTickMap perceptionsUnderway_;
    bool bIsEnabled_;
    bool wasInCity_;
};

#endif // __PHY_PerceptionView_h_
