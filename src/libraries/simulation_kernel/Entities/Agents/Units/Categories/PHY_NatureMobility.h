// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureMobility.h $
// $Author: Jvt $
// $Modtime: 9/12/04 15:37 $
// $Revision: 2 $
// $Workfile: PHY_NatureMobility.h $
//
// *****************************************************************************

#ifndef __PHY_NatureMobility_h_
#define __PHY_NatureMobility_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

// =============================================================================
// @class  PHY_NatureMobility
// Created: JVT 2004-08-03
// =============================================================================
class PHY_NatureMobility
{
    MT_COPYNOTALLOWED( PHY_NatureMobility )

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_NatureMobility* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string&                 GetName () const;
          uint                         GetID   () const;
          ASN1T_EnumUnitNatureMobility GetAsnID() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eAeroporte            ,
        eAerienCompose        ,
        eAerienVoilureFixe    ,
        eAerienPlusLegerQueAir,
        eAerienAilesRotatives ,
        eAerienAmphibie       ,
        eAnimalMonte          ,
        eDemonte              ,
        eTerrestreCheminDeFer ,
        eTerrestreAutoPropulse,
        eTerrestreChenille    ,
        eTerrestreRemorque    ,
        eTerrestreRoues       ,
        eMaritimeSousMarin    ,
        eMaritimeSurface      ,
        eInconnu              ,
        eNone                 ,
    };

    typedef std::map< std::string, const PHY_NatureMobility*, sCaseInsensitiveLess > T_NatureMobilityMap;
    typedef T_NatureMobilityMap::const_iterator                                      CIT_NatureMobilityMap;
    //@}

private:
     PHY_NatureMobility( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureMobility nAsnID );
    ~PHY_NatureMobility();

private:
    const std::string                  strName_;
    const E_Type                       nType_;
    const ASN1T_EnumUnitNatureMobility nAsnID_;

private:
    static T_NatureMobilityMap natureMobility_;
};

#include "PHY_NatureMobility.inl"

#endif // __PHY_NatureMobility_h_
