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

#include "PHY_Perception_ABC.h"

// =============================================================================
// @class  PHY_PerceptionRecoPoint
// Created: JVT 2004-10-21
// =============================================================================
class PHY_PerceptionRecoPoint : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionRecoPoint )

public:
             PHY_PerceptionRecoPoint( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionRecoPoint();

    void Update();

    //! @name Add/Remove Points
    //@{
    void* AddPoint   ( const MT_Vector2D& center, MT_Float rSize, MT_Float rSpeed, DIA_Variable_ABC& result );
    void  RemovePoint( void* );

    bool  HasPointToHandle() const;
    //@}

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
    //! @name Types
    //@{
    struct sReco
    {
        const MT_Vector2D vCenter_;
        MT_Float          rCurrentSize_;
        const MT_Float    rFinalSize_;
        const MT_Float    rGrowthSpeed_;
        DIA_Variable_ABC* pReturn_;

        sReco( const MT_Vector2D& vCenter, MT_Float rSize, MT_Float rGrowthSpeed, DIA_Variable_ABC& returnRes )
            : vCenter_      ( vCenter )
            , rCurrentSize_ ( 0.f )
            , rFinalSize_   ( rSize )
            , rGrowthSpeed_ ( rGrowthSpeed )
            , pReturn_      ( &returnRes )
        {
            assert( rGrowthSpeed_ > 0.f );
            assert( rFinalSize_ > 0.f );
        }

    private:
        sReco& operator = ( const sReco& );
    };

    typedef std::vector< sReco* >        T_RecoVector;
    typedef T_RecoVector::iterator       IT_RecoVector;
    typedef T_RecoVector::const_iterator CIT_RecoVector;
    //@}

private:
    T_RecoVector recos_;
};

#include "PHY_PerceptionRecoPoint.inl"

#endif // __PHY_PerceptionRecoPoint_h_
