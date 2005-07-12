// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureWeapon.h $
// $Author: Jvt $
// $Modtime: 9/12/04 16:35 $
// $Revision: 2 $
// $Workfile: PHY_NatureWeapon.h $
//
// *****************************************************************************

#ifndef __PHY_NatureWeapon_h_
#define __PHY_NatureWeapon_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

// =============================================================================
// @class  PHY_NatureWeapon
// Created: JVT 2004-08-03
// =============================================================================
class PHY_NatureWeapon
{
    MT_COPYNOTALLOWED( PHY_NatureWeapon )

public:
    static const PHY_NatureWeapon notDefined_;

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_NatureWeapon* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string&               GetName () const;
          uint                       GetID   () const;
          ASN1T_EnumUnitNatureWeapon GetAsnID() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eNone,
        eAntichar,
        eAdministration,
        eDefenseAerienne,
        eCentreSoutienOperationsAeriennes,
        eBlinde,
        eAviation,
        eCavalerie,
        eGenie,
        eDeminage,
        eSoutienArtillerie,
        eArtillerie,
        eInfanterie,
        eGuerreInformation,
        eForcesSecuriteInterne,
        eExecutionLoi,
        eSoutienAtterrissage,
        eLogistique,
        eReparation,
        eManoeuvre,
        eMedical,
        eRenseignementMilitaire,
        eMissile,
        eNbc,
        eInconnue,
        eNonSpecifiee,
        eReconnaissance,
        eTransmissions,
        eRavitaillement,
        eTacticalAirControlParty,
        eTransport
    };

    typedef std::map< std::string, const PHY_NatureWeapon*, sCaseInsensitiveLess > T_NatureWeaponMap;
    typedef T_NatureWeaponMap::const_iterator                                      CIT_NatureWeaponMap;
    //@}

private:
     PHY_NatureWeapon( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureWeapon nAsnID );
    ~PHY_NatureWeapon();

private:
    const std::string                strName_;
    const E_Type                     nType_;
    const ASN1T_EnumUnitNatureWeapon nAsnID_;

private:
    static T_NatureWeaponMap natureWeapons_;
};

#include "PHY_NatureWeapon.inl"

#endif // __PHY_NatureWeapon_h_
