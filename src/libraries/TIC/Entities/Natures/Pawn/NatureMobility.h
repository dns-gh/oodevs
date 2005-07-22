// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureMobility.h $
// $Author: Sbo $
// $Modtime: 14/06/05 16:54 $
// $Revision: 1 $
// $Workfile: NatureMobility.h $
//
// *****************************************************************************

#ifndef __NatureMobility_h_
#define __NatureMobility_h_

namespace TIC
{

// =============================================================================
// @class  NatureMobility
// Created: JVT 2004-08-03
// =============================================================================
class NatureMobility
{
    MT_COPYNOTALLOWED( NatureMobility )

public:
    //! @name Manager
    //@{
    static       void            Initialize();
    static       void            Terminate ();

    static const NatureMobility* Find      ( const std::string&           strName );
    static const std::string*    Find      ( ASN1T_EnumUnitNatureMobility asnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string&                 GetName   () const;
          uint                         GetID     () const;
          ASN1T_EnumUnitNatureMobility GetAsnID  () const;
    const std::string&                 GetKey    () const;
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

    typedef std::map< std::string, const NatureMobility*, sCaseInsensitiveLess > T_NatureMobilityMap;
    typedef T_NatureMobilityMap::const_iterator                                  CIT_NatureMobilityMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    NatureMobility( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureMobility nAsnID, const std::string& strKey );
    ~NatureMobility();
    //@}

private:
    //! @name Member data
    //@{
    const std::string     strName_;
    const E_Type          nType_;
    const ASN1T_EnumUnitNatureMobility nAsnID_;
    const std::string     strKey_;

    static T_NatureMobilityMap natureMobility_;
    //@}
};

} // end namespace TIC

#include "NatureMobility.inl"

#endif // __NatureMobility_h_
