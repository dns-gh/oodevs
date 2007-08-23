// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoSurveillance.h $
// $Author: Nld $
// $Modtime: 2/05/05 15:43 $
// $Revision: 4 $
// $Workfile: PHY_PerceptionRecoSurveillance.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionRecoSurveillance_h_
#define __PHY_PerceptionRecoSurveillance_h_

#include "PHY_Perception_ABC.h"
#include "simulation_terrain/TER_Localisation.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  PHY_PerceptionRecoSurveillance
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionRecoSurveillance : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionRecoSurveillance )

public:
             PHY_PerceptionRecoSurveillance( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionRecoSurveillance();

    //! @name Initialization
    //@{
    static void Initialize( xml::xistream& xis );
    //@}

    //! @name Add/Remove Points
    //@{
    void* AddLocalisation    ( const TER_Localisation& );
    void  RemoveLocalisation ( void* );

    bool  HasLocalisationToHandle() const;
    //@}

    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const;
    //@}

private:
    //! @name Types
    //@{
    struct sReco
    {
        sReco( const TER_Localisation& localisation );

        bool IsInside       ( const MT_Vector2D& vPoint )        const;
        void GetAgentsInside( TER_Agent_ABC::T_AgentPtrVector& ) const;

    private:
        sReco& operator = ( const sReco& );

    private:
        const TER_Localisation localisation_;
        const uint             nForestDetectionTimeStep_;
        const uint             nUrbanDetectionTimeStep_;
        const uint             nEmptyDetectionTimeStep_;
    };

    typedef std::vector< sReco* >        T_RecoVector;
    typedef T_RecoVector::iterator       IT_RecoVector;
    typedef T_RecoVector::const_iterator CIT_RecoVector;
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadAlatTime( xml::xistream& xis );
    //@}

private:
    static MT_Float rForestSurveillanceTime_;
    static MT_Float rUrbanSurveillanceTime_;
    static MT_Float rEmptySurveillanceTime_;

private:
    T_RecoVector recos_;
};

#include "PHY_PerceptionRecoSurveillance.inl"

#endif // __PHY_PerceptionRecoSurveillance_h_
