// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureCategory.h $
// $Author: Sbo $
// $Modtime: 7/06/05 9:58 $
// $Revision: 1 $
// $Workfile: NatureCategory.h $
//
// *****************************************************************************

#ifndef __NatureCategory_h_
#define __NatureCategory_h_

namespace TIC
{

// =============================================================================
// @class  NatureCategory
// Created: JVT 2004-08-03
// =============================================================================
class NatureCategory
{
    MT_COPYNOTALLOWED( NatureCategory )

public:
    //! @name Manager
    //@{
    static       void                  Initialize();
    static       void                  Terminate ();

    static const NatureCategory*       Find      ( const std::string&           strName );
    static const std::string*          Find      ( ASN1T_EnumUnitNatureCategory asnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string&                 GetName   () const;
          uint                         GetID     () const;
          ASN1T_EnumUnitNatureCategory GetAsnID  () const;
    const std::string&                 GetKey    () const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eCombat           ,
        eSoutienLogistique,
        eCombatSupport    ,
        eInconnu          ,
        eNone             ,
    };

    typedef std::map< std::string, const NatureCategory*, sCaseInsensitiveLess > T_NatureCategoryMap;
    typedef T_NatureCategoryMap::const_iterator                                  CIT_NatureCategoryMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    NatureCategory( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureCategory nAsnID, const std::string& strKey );
    ~NatureCategory();
    //@}

private:
    //! @name Member data
    //@{
    const std::string                  strName_;
    const E_Type                       nType_;
    const ASN1T_EnumUnitNatureCategory nAsnID_;
    const std::string                  strKey_;

    static T_NatureCategoryMap         natureCategory_;
    //@}
};

} // end namespace TIC

#include "NatureCategory.inl"

#endif // __NatureCategory_h_
