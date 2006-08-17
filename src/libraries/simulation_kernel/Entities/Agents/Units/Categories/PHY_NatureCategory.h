// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureCategory.h $
// $Author: Jvt $
// $Modtime: 9/12/04 15:48 $
// $Revision: 2 $
// $Workfile: PHY_NatureCategory.h $
//
// *****************************************************************************

#ifndef __PHY_NatureCategory_h_
#define __PHY_NatureCategory_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

// =============================================================================
// @class  PHY_NatureCategory
// Created: JVT 2004-08-03
// =============================================================================
class PHY_NatureCategory
{
    MT_COPYNOTALLOWED( PHY_NatureCategory )

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_NatureCategory* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string&                 GetName () const;
          uint                         GetID   () const;
          ASN1T_EnumUnitNatureCategory GetAsnID() const;
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

    typedef std::map< std::string, const PHY_NatureCategory*, sCaseInsensitiveLess > T_NatureCategoryMap;
    typedef T_NatureCategoryMap::const_iterator                                      CIT_NatureCategoryMap;
    //@}

private:
     PHY_NatureCategory( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureCategory nAsnID );
    ~PHY_NatureCategory();

private:
    const std::string                  strName_;
    const E_Type                       nType_;
    const ASN1T_EnumUnitNatureCategory nAsnID_;

private:
    static T_NatureCategoryMap natureCategory_;
};

#include "PHY_NatureCategory.inl"

#endif // __PHY_NatureCategory_h_
