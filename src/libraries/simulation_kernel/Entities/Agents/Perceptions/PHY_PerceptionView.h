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
             PHY_PerceptionView( PHY_RoleInterface_Perceiver& perceiver, bool inCity );
    virtual ~PHY_PerceptionView();

    //! @name Tools
    //@{
    void Enable( bool enable );
    //@}

    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent );

    virtual void                       Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual void                       ExecuteCollisions( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;

    virtual void                       Execute( const TER_PopulationFlow_ABC::T_ConstPopulationFlowVector& perceivableFlows );
    virtual const PHY_PerceptionLevel& Compute( const MIL_PopulationFlow& flow, T_PointVector& shape ) const;

    virtual void                       Execute( const TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector perceivableConcentrations );
    virtual const PHY_PerceptionLevel& Compute( const MIL_PopulationConcentration& concentration ) const;

    virtual void FinalizePerception();
    //@}

public:
    static unsigned int nUrbanCoefficient_;

private:
    //! @name Types
    //@{
    typedef std::pair< unsigned int, double > T_PerceptionParameterPair;
    typedef std::map< const void*, T_PerceptionParameterPair > T_PerceptionTickMap;
    //@}

private:
    //! @name Helpers
    //@{
    void FinalizeSurfaceAgents();
    void TransfertPerception();
    bool FailDirectView( const MIL_Agent_ABC& target ) const;

    T_PerceptionParameterPair GetParameter( const MIL_Agent_ABC& target );
    //@}

private:
    T_PerceptionTickMap perceptionsBuffer_;
    T_PerceptionTickMap perceptionsUnderway_;
    bool bIsEnabled_;
    bool wasInCity_;
};

#endif // __PHY_PerceptionView_h_
