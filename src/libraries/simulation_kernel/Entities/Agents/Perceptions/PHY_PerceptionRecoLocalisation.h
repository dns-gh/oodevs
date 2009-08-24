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

#include "PHY_PerceptionWithLocation.h"
#include "PHY_PerceptionLocalisation.h"
#include "simulation_terrain/TER_Localisation.h"

class PHY_PerceptionRecoLocalisationReco : public PHY_PerceptionLocalisation
{
public:
    PHY_PerceptionRecoLocalisationReco( const TER_Localisation& localisation, MT_Float rRadius );
    PHY_PerceptionRecoLocalisationReco( const TER_Localisation& localisation, bool bUseDefaultRadius );

    bool IsInside        ( const PHY_RoleInterface_Perceiver& perceiver, const MT_Vector2D& vPoint )        const;
    void GetAgentsInside ( const PHY_RoleInterface_Perceiver& perceiver, TER_Agent_ABC::T_AgentPtrVector& ) const;

private:
    PHY_PerceptionRecoLocalisationReco& operator = ( const PHY_PerceptionRecoLocalisationReco& );

private:
    const TER_Localisation localisation_;
    const MT_Float         rRadius_;
    const bool             bShouldUseRadius_;
};

// =============================================================================
// @class  PHY_PerceptionRecoLocalisation
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionRecoLocalisation : public PHY_PerceptionWithLocation< PHY_PerceptionRecoLocalisationReco >
{

public:
    explicit PHY_PerceptionRecoLocalisation( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionRecoLocalisation();

    //! @name Add/Remove Points
    //@{
    int AddLocalisationWithRadius        ( const TER_Localisation&, MT_Float rRadius );
    int AddLocalisationWithDefaultRadius ( const TER_Localisation& );
    int AddLocalisationWithoutRadius     ( const TER_Localisation& );
    void  RemoveLocalisation( int );

    bool  HasLocalisationToHandle() const;
    //@}

    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const;
    //@}
};

#endif // __PHY_PerceptionRecoLocalisation_h_
