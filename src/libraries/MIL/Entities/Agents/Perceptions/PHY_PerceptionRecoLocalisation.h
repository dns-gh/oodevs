// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoLocalisation.h $
// $Author: Nld $
// $Modtime: 2/05/05 15:43 $
// $Revision: 8 $
// $Workfile: PHY_PerceptionRecoLocalisation.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionRecoLocalisation_h_
#define __PHY_PerceptionRecoLocalisation_h_

#include "PHY_Perception_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
// @class  PHY_PerceptionRecoLocalisation
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionRecoLocalisation : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionRecoLocalisation )

public:
             PHY_PerceptionRecoLocalisation( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionRecoLocalisation();

    //! @name Add/Remove Points
    //@{
    void* AddLocalisationWithRadius        ( const TER_Localisation&, MT_Float rRadius );
    void* AddLocalisationWithDefaultRadius ( const TER_Localisation& );
    void* AddLocalisationWithoutRadius     ( const TER_Localisation& );
    void  RemoveLocalisation( void* );

    bool  HasLocalisationToHandle() const;
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
    //! @name Types
    //@{
    struct sReco
    {
        sReco( const TER_Localisation& localisation, MT_Float rRadius );
        sReco( const TER_Localisation& localisation, bool bUseDefaultRadius );

        bool IsInside        ( const PHY_RolePion_Perceiver& perceiver, const MT_Vector2D& vPoint )        const;
        void GetAgentsInside ( const PHY_RolePion_Perceiver& perceiver, TER_Agent_ABC::T_AgentPtrVector& ) const;

    private:
        sReco& operator = ( const sReco& );

    private:
        const TER_Localisation localisation_;
        const MT_Float         rRadius_;
        const bool             bShouldUseRadius_;
    };

    typedef std::vector< sReco* >         T_RecoVector;
    typedef T_RecoVector::iterator       IT_RecoVector;
    typedef T_RecoVector::const_iterator CIT_RecoVector;
    //@}

private:
    T_RecoVector recos_;
};

#include "PHY_PerceptionRecoLocalisation.inl"

#endif // __PHY_PerceptionRecoLocalisation_h_
