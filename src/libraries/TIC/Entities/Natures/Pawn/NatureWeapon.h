// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureWeapon.h $
// $Author: Sbo $
// $Modtime: 30/06/05 15:55 $
// $Revision: 2 $
// $Workfile: NatureWeapon.h $
//
// *****************************************************************************

#ifndef __NatureWeapon_h_
#define __NatureWeapon_h_

namespace TIC
{

// =============================================================================
// @class  NatureWeapon
// Created: JVT 2004-08-03
// =============================================================================
class NatureWeapon
{
    MT_COPYNOTALLOWED( NatureWeapon )

public:
    //! @name Manager
    //@{
    static       void                Initialize();
    static       void                Terminate ();

    static const NatureWeapon*       Find      ( const std::string&         strName );
    static const std::string*        Find      ( ASN1T_EnumUnitNatureWeapon asnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string&               GetName   () const;
          uint                       GetID     () const;
          ASN1T_EnumUnitNatureWeapon GetAsnID  () const;
    const std::string&               GetKey    () const;
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

    typedef std::map< std::string, const NatureWeapon*, sCaseInsensitiveLess > T_NatureWeaponMap;
    typedef T_NatureWeaponMap::const_iterator                                  CIT_NatureWeaponMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    NatureWeapon( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureWeapon nAsnID, const std::string& strKey );
    ~NatureWeapon();
    //@}

private:
    //! @name Member data
    //@{
    const std::string                strName_;
    const E_Type                     nType_;
    const ASN1T_EnumUnitNatureWeapon nAsnID_;
    const std::string                strKey_;

    static T_NatureWeaponMap         natureWeapons_;
    //@}
};

} // end namespace TIC

#include "NatureWeapon.inl"

#endif // __NatureWeapon_h_
