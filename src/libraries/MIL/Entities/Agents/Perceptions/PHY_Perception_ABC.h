//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_Perception_ABC_h_
#define __PHY_Perception_ABC_h_

#include "MIL.h"

#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "TER/TER_Agent_ABC.h"
#include "TER/TER_DynaObject_ABC.h"

class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class MIL_Agent_ABC;
class MIL_RealObject_ABC;

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_Perception_ABC )

public:
    PHY_Perception_ABC( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_Perception_ABC();

    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const = 0;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents ) = 0;
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const = 0;
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const = 0;

    virtual void                       Execute( const TER_DynaObject_ABC::T_DynaObjectVector& perceivableObjects ) = 0;
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const = 0;
    virtual const PHY_PerceptionLevel& Compute( const MIL_RealObject_ABC&   object    ) const = 0;
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

#include "PHY_Perception_ABC.inl"

#endif // __PHY_Perception_ABC_h_
