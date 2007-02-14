// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionFlyingShell.h $
// $Author: Nld $
// $Modtime: 2/05/05 15:43 $
// $Revision: 4 $
// $Workfile: PHY_PerceptionFlyingShell.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionFlyingShell_h_
#define __PHY_PerceptionFlyingShell_h_

#include "PHY_Perception_ABC.h"
#include "simulation_terrain/TER_Localisation.h"

class MIL_Effect_IndirectFire;

// =============================================================================
// @class  PHY_PerceptionFlyingShell
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionFlyingShell : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionFlyingShell )

public:
             PHY_PerceptionFlyingShell( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionFlyingShell();

    //! @name Initialization
    //@{
    static void Initialize( MIL_InputArchive& archive );
    //@}

    //! @name Add/Remove Points
    //@{
    void* AddLocalisation        ( const TER_Localisation& );
    void  RemoveLocalisation     ( void* );
    bool  HasLocalisationToHandle() const;
    //@}

    //! @name Execution
    //@{
    virtual void Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const TER_Localisation* > T_ZoneVector;
    typedef T_ZoneVector::iterator                 IT_ZoneVector;
    typedef T_ZoneVector::const_iterator           CIT_ZoneVector;

    typedef std::set< const MIL_Effect_IndirectFire* > T_FlyingShellSet;
    //@}

private:
    T_ZoneVector     zones_;
    T_FlyingShellSet lastPerceivedFlyingShells_;

private:
    static MT_Float rRadius_;
};

#include "PHY_PerceptionFlyingShell.inl"

#endif // __PHY_PerceptionFlyingShell_h_
