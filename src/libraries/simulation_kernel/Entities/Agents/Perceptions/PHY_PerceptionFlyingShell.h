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

namespace xml
{
    class xistream;
}

class MIL_Effect_IndirectFire;

// =============================================================================
// @class  PHY_PerceptionFlyingShell
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionFlyingShell : public PHY_Perception_ABC
{
public:
             PHY_PerceptionFlyingShell( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionFlyingShell();

    static void Initialize( xml::xistream& xis );

    int  AddLocalisation        ( const TER_Localisation& );
    void RemoveLocalisation     ( int );
    bool HasLocalisationToHandle() const;

    virtual void FinalizePerception();
    virtual void Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );

private:
    typedef std::vector< const TER_Localisation* > T_ZoneVector;
    typedef std::set< const MIL_Effect_IndirectFire* > T_FlyingShellSet;
    typedef std::map< int, const TER_Localisation* > T_PerceptionIdMap;

private:
    T_ZoneVector      zones_;
    T_FlyingShellSet  lastPerceivedFlyingShells_;
    T_PerceptionIdMap ids_;
};

#endif // __PHY_PerceptionFlyingShell_h_
