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

#include "PHY_PerceptionWithLocation.h"
#include "PHY_PerceptionLocalisation.h"
#include "simulation_terrain/TER_Localisation.h"

namespace xml
{
    class xistream;
}

class PHY_PerceptionRecoSurveillanceReco : public PHY_PerceptionLocalisation
{
public:
    PHY_PerceptionRecoSurveillanceReco( const TER_Localisation& localisation );

    bool IsInside       ( const MT_Vector2D& vPoint )        const;
    void GetAgentsInside( TER_Agent_ABC::T_AgentPtrVector& ) const;

private:
    PHY_PerceptionRecoSurveillanceReco& operator = ( const PHY_PerceptionRecoSurveillanceReco& );

private:
    const TER_Localisation localisation_;
    const unsigned int             nForestDetectionTimeStep_;
    const unsigned int             nUrbanDetectionTimeStep_;
    const unsigned int             nEmptyDetectionTimeStep_;
};

// =============================================================================
// @class  PHY_PerceptionRecoSurveillance
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionRecoSurveillance : public PHY_PerceptionWithLocation< PHY_PerceptionRecoSurveillanceReco >
{
public:
             PHY_PerceptionRecoSurveillance( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionRecoSurveillance();

    //! @name Initialization
    //@{
    static void Initialize( xml::xistream& xis );
    //@}

    //! @name Add/Remove Points
    //@{
    int  AddLocalisation    ( const TER_Localisation& );
    void RemoveLocalisation ( int );

    bool HasLocalisationToHandle() const;
    //@}

    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    //@}

private:
    //! @name Helpers
    //@{
    static void ReadAlatTime( xml::xistream& xis );
    //@}
};

#endif // __PHY_PerceptionRecoSurveillance_h_
