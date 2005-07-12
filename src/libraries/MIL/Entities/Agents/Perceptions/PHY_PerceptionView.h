//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_PerceptionView_h_
#define __PHY_PerceptionView_h_

#include "MIL.h"

#include "PHY_Perception_ABC.h"

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_PerceptionView : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionView )

public:
             PHY_PerceptionView( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionView();

    //! @name Tools
    //@{
    void Enable();
    void Disable();
    //@}
    
    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const;

    virtual void                       Execute( const TER_DynaObject_ABC::T_DynaObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    virtual const PHY_PerceptionLevel& Compute( const MIL_RealObject_ABC&   object    ) const;
    //@}

private:
    bool bIsEnabled_;
};

#include "PHY_PerceptionView.inl"

#endif // __PHY_PerceptionView_h_
