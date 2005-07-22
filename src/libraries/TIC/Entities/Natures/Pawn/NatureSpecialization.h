// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureSpecialization.h $
// $Author: Sbo $
// $Modtime: 7/06/05 9:58 $
// $Revision: 1 $
// $Workfile: NatureSpecialization.h $
//
// *****************************************************************************

#ifndef __NatureSpecialization_h_
#define __NatureSpecialization_h_

namespace TIC
{

// =============================================================================
// @class  NatureSpecialization
// Created: JVT 2004-08-03
// =============================================================================
class NatureSpecialization
{
    MT_COPYNOTALLOWED( NatureSpecialization )

public:
    //! @name Manager
    //@{
    static       void                        Initialize();
    static       void                        Terminate ();

    static const NatureSpecialization*       Find      ( const std::string&                 strName );
    static const std::string*                Find      ( ASN1T_EnumUnitNatureSpecialization asnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string&                       GetName   () const;
          uint                               GetID     () const;
          ASN1T_EnumUnitNatureSpecialization GetAsnID  () const;
    const std::string&                       GetKey    () const;
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

    typedef std::map< std::string, const NatureSpecialization*, sCaseInsensitiveLess > T_NatureSpecializationMap;
    typedef T_NatureSpecializationMap::const_iterator                                  CIT_NatureSpecializationMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    NatureSpecialization( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureSpecialization nAsnID, const std::string& strKey );
    ~NatureSpecialization();
    //@}

private:
    //! @name Member data
    //@{
    const std::string                        strName_;
    const E_Type                             nType_;
    const ASN1T_EnumUnitNatureSpecialization nAsnID_;
    const std::string                        strKey_;

    static T_NatureSpecializationMap         natureSpecialization_;
    //@}
};

} // end namespace TIC

#include "NatureSpecialization.inl"

#endif // __NatureSpecialization_h_
