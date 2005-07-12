// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureSpecialization.h $
// $Author: Jvt $
// $Modtime: 9/12/04 15:18 $
// $Revision: 2 $
// $Workfile: PHY_NatureSpecialization.h $
//
// *****************************************************************************

#ifndef __PHY_NatureSpecialization_h_
#define __PHY_NatureSpecialization_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

// =============================================================================
// @class  PHY_NatureSpecialization
// Created: JVT 2004-08-03
// =============================================================================
class PHY_NatureSpecialization
{
    MT_COPYNOTALLOWED( PHY_NatureSpecialization )

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_NatureSpecialization* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string&                       GetName () const;
          uint                               GetID   () const;
          ASN1T_EnumUnitNatureSpecialization GetAsnID() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eNatureSpec_Jag                              ,
        eNatureSpec_Riverine                         ,
        eNatureSpec_SecurityPolice                   ,
        eNatureSpec_Aerien                           ,
        eNatureSpec_Aeroporte                        ,
        eNatureSpec_AttaqueAerienne                  ,
        eNatureSpec_Blinde                           ,
        eNatureSpec_CheminDeFer                      ,
        eNatureSpec_Chirurgie                        ,
        eNatureSpec_Compose                          ,
        eNatureSpec_Construction                     ,
        eNatureSpec_Courrier                         ,
        eNatureSpec_DecollageAtterissageVerticalCourt,
        eNatureSpec_Dentiste                         ,
        eNatureSpec_Finance                          ,
        eNatureSpec_GuerreElectronique               ,
        eNatureSpec_Helicoptere                      ,
        eNatureSpec_Mecanise                         ,
        eNatureSpec_Motorise                         ,
        eNatureSpec_Naval                            ,
        eNatureSpec_PatrouilleCotiere                ,
        eNatureSpec_PoliceMilitaire                  ,
        eNatureSpec_Psychologique                    ,
        eNatureSpec_Religieux                        ,
        eNatureSpec_ServiceDuPersonnel               ,
        eNatureSpec_Terrestre                        ,
        eNatureSpec_Veterinaire                      ,
        eNatureSpec_VoilureFixe                      ,
        eNatureSpec_None
    };

    typedef std::map< std::string, const PHY_NatureSpecialization*, sCaseInsensitiveLess > T_NatureSpecializationMap;
    typedef T_NatureSpecializationMap::const_iterator                                      CIT_NatureSpecializationMap;
    //@}

private:
     PHY_NatureSpecialization( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureSpecialization nAsnID );
    ~PHY_NatureSpecialization();

private:
    const std::string                        strName_;
    const E_Type                             nType_;
    const ASN1T_EnumUnitNatureSpecialization nAsnID_;

private:
    static T_NatureSpecializationMap natureSpecialization_;
};

#include "PHY_NatureSpecialization.inl"

#endif // __PHY_NatureSpecialization_h_
