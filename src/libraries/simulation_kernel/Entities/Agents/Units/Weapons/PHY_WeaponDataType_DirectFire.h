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
#include "MT_Tools/MT_Vector3D.h"

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
class PHY_AmmoDotationClass;
class PHY_Composante_ABC;
class PHY_ComposanteType_ABC;
class PHY_FireResults_ABC;
class PHY_MaterialCompositionType;
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
    double GetDangerosity                ( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, bool bUsePH ) const;
    double GetDangerosity                ( const PHY_ComposanteType_ABC& targetComposanteType, double rDistBtwFirerAndTarget, const PHY_MaterialCompositionType* material ) const;
    double GetMaxRangeToFireOn           ( const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const;
    double GetMinRangeToFireOn           ( const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const;
    double GetMaxRangeToFireOnWithPosture( const PHY_ComposanteType_ABC& targetComposanteType, const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, double rWantedPH ) const;
    double GetMinRangeToFireOnWithPosture( const PHY_ComposanteType_ABC& targetComposanteType, const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, double rWantedPH ) const;
    double GetMaxRangeToFire             ( double rWantedPH ) const;
    double GetMaxRange                   () const;
    void     Fire                          ( MIL_Agent_ABC& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH ) const;
    void     Fire                          ( MIL_Agent_ABC& firer, MIL_PopulationElement_ABC& target, unsigned int nNbrAmmoReserved, PHY_FireResults_ABC& fireResult, const PHY_AmmoDotationClass* dotationClass ) const;
    //@}

private:
    //! @name Helpers
    //@{
    double GetPH( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_Volume& targetVolume, const MT_Vector3D& firerPosition, const MT_Vector3D& targetPosition ) const;
    double GetPH( const PHY_Volume& targetVolume, double rDistance ) const;

    double GetMaxDistanceForPH( double rPH, const PHY_Volume& targetVolume ) const;
    double GetMinDistanceForPH( double rPH, const PHY_Volume& targetVolume ) const;
    double GetMaxDistanceForPH( double rPH, const PHY_RoleInterface_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture, const PHY_Volume& targetVolume ) const;
    double GetMinDistanceForPH( double rPH, const PHY_RoleInterface_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture, const PHY_Volume& targetVolume ) const;

    void InitializePH( xml::xistream& xis );

    void ReadHitProbability( xml::xistream& xis, MT_InterpolatedFunction& phFunction );
    void NotifyFirerPerception( MIL_Agent_ABC& firer, MIL_Agent_ABC& target ) const;
    bool IsFirerInsideRecognitionDistance( MIL_Agent_ABC& firer, MIL_Agent_ABC& target ) const;
    //@}

private:
    //! @name Member data
    //@{
    const PHY_WeaponType& weaponType_;
    std::vector< MT_InterpolatedFunction > phs_;
    //@}
};

#endif // __PHY_WeaponDataType_DirectFire_h_
