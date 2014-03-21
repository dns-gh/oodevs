// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoPoint.h $
// $Author: Nld $
// $Modtime: 2/05/05 15:43 $
// $Revision: 5 $
// $Workfile: PHY_PerceptionRecoPoint.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionRecoPoint_h_
#define __PHY_PerceptionRecoPoint_h_

#include "PHY_PerceptionWithLocation.h"
#include "PHY_PerceptionLocalisation.h"

class DEC_Decision_ABC;


class PHY_PerceptionRecoPointReco : public PHY_PerceptionLocalisation
{
public:
    const MT_Vector2D vCenter_;
    double          rCurrentSize_;
    const double    rFinalSize_;
    const double    rGrowthSpeed_;
    DEC_Decision_ABC& callerAgent_;
    bool              bProcessed_;

    PHY_PerceptionRecoPointReco( const MT_Vector2D& vCenter, double rSize, double rGrowthSpeed, DEC_Decision_ABC& callerAgent )
        : vCenter_     ( vCenter )
        , rCurrentSize_( 0.f )
        , rFinalSize_  ( rSize )
        , rGrowthSpeed_( rGrowthSpeed )
        , callerAgent_ ( callerAgent )
        , bProcessed_  ( false )
    {
        assert( rGrowthSpeed_ > 0.f );
        assert( rFinalSize_ > 0.f );
    }

private:
    PHY_PerceptionRecoPointReco& operator = ( const PHY_PerceptionRecoPointReco& );
};

// =============================================================================
// @class  PHY_PerceptionRecoPoint
// Created: JVT 2004-10-21
// =============================================================================
class PHY_PerceptionRecoPoint : public PHY_PerceptionWithLocation< PHY_PerceptionRecoPointReco >
{
public:
    explicit PHY_PerceptionRecoPoint( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionRecoPoint();

    void Update();

    //! @name Add/Remove Points
    //@{
    int  AddPoint   ( const MT_Vector2D& center, double rSize, double rSpeed, DEC_Decision_ABC& callerAgent );
    void RemovePoint( int id );

    bool HasPointToHandle() const;
    //@}

    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;

    virtual void                       Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    //@}
};

#endif // __PHY_PerceptionRecoPoint_h_
