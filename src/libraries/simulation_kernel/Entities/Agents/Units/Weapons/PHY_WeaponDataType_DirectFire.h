// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_WeaponDataType_DirectFire.h $
// $Author: Jvt $
// $Modtime: 2/05/05 10:56 $
// $Revision: 6 $
// $Workfile: PHY_WeaponDataType_DirectFire.h $
//
// *****************************************************************************

#ifndef __PHY_WeaponDataType_DirectFire_h_
#define __PHY_WeaponDataType_DirectFire_h_

#include "MT_Tools/MT_InterpolatedFunction.h"
#include "MT_Tools/MT_Random.h"

namespace xml
{
    class xistream;
}

class PHY_Volume;
class PHY_Posture;
class PHY_RoleInterface_Posture;
class PHY_WeaponType;
class MIL_AgentPion;
class MIL_Agent_ABC;
class PHY_Composante_ABC;
class PHY_ComposanteType_ABC;
class PHY_FireResults_ABC;
class MIL_PopulationElement_ABC;

// =============================================================================
// @class  PHY_WeaponDataType_DirectFire
// Created: JVT 2004-08-03
// =============================================================================
class PHY_WeaponDataType_DirectFire : private boost::noncopyable
{

public:
             PHY_WeaponDataType_DirectFire( const PHY_WeaponType& weaponType, xml::xistream& xis );
    virtual ~PHY_WeaponDataType_DirectFire();

    //! @name Operations
    //@{
    MT_Float GetDangerosity                ( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, bool bUsePH ) const;
    MT_Float GetDangerosity                ( const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rDistBtwFirerAndTarget ) const;
    MT_Float GetMaxRangeToFireOn           ( const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const;
    MT_Float GetMinRangeToFireOn           ( const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const;
    MT_Float GetMaxRangeToFireOnWithPosture( const PHY_ComposanteType_ABC& targetComposanteType, const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, MT_Float rWantedPH ) const;
    MT_Float GetMinRangeToFireOnWithPosture( const PHY_ComposanteType_ABC& targetComposanteType, const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, MT_Float rWantedPH ) const;
    void     Fire                          ( MIL_AgentPion& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH ) const;
    void     Fire                          ( MIL_AgentPion& firer, MIL_PopulationElement_ABC& target, uint nNbrAmmoReserved, PHY_FireResults_ABC& fireResult ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_InterpolatedFunction< MT_Float > >  T_PhVector;
    typedef T_PhVector::const_iterator                          CIT_PhVector;   
    //@}

private:
    //! @name Init / Tools
    //@{
    MT_Float GetPH              ( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_Volume& targetVolume, MT_Vector3D firerPosition, MT_Vector3D targetPosition ) const;
    MT_Float GetPH              ( const PHY_Posture& firerPosture, const PHY_Posture& targetPosture, const PHY_Volume& targetVolume, MT_Float rDistance ) const;
    
    MT_Float GetMaxDistanceForPH( MT_Float rPH, const PHY_Posture&          firerPosture, const PHY_Posture& targetPosture              , const PHY_Volume& targetVolume ) const;
    MT_Float GetMinDistanceForPH( MT_Float rPH, const PHY_Posture&          firerPosture, const PHY_Posture& targetPosture              , const PHY_Volume& targetVolume ) const;
    MT_Float GetMaxDistanceForPH( MT_Float rPH, const PHY_RoleInterface_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture, const PHY_Volume& targetVolume ) const;
    MT_Float GetMinDistanceForPH( MT_Float rPH, const PHY_RoleInterface_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture, const PHY_Volume& targetVolume ) const;

    void     InitializePH       ( xml::xistream& xis );
    //@}
    //! @name Helpers
    //@{
    void ReadHitProbability( xml::xistream& xis, MT_InterpolatedFunction< MT_Float >& phFunction );
    //@}
private:
    //! @name Member data
    //@{
    const PHY_WeaponType& weaponType_;
          T_PhVector      phs_;
    //@}

private:
    static MT_Random randomGenerator_;
};

#endif // __PHY_WeaponDataType_DirectFire_h_
