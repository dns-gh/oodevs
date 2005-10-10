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
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const;

    virtual void                       Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    virtual const PHY_PerceptionLevel& Compute( const MIL_RealObject_ABC&   object    ) const;
    //@}

private:
    TER_Localisation localisation_;
};

#include "PHY_PerceptionAlat.inl"

#endif // __PHY_PerceptionAlat_h_
