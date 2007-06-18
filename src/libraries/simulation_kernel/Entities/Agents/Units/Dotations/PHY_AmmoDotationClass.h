// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h $
// $Author: Nld $
// $Modtime: 4/05/05 16:39 $
// $Revision: 2 $
// $Workfile: PHY_AmmoDotationClass.h $
//
// *****************************************************************************

#ifndef __PHY_AmmoDotationClass_h_
#define __PHY_AmmoDotationClass_h_

#include "MIL.h"

class PHY_DotationCategory;
class PHY_DotationCategory_IndirectFire_ABC;

// =============================================================================
// @class  PHY_AmmoDotationClass
// Created: JVT 2004-08-03
// =============================================================================
class PHY_AmmoDotationClass
{
    MT_COPYNOTALLOWED( PHY_AmmoDotationClass )

public:
    //! @name Statics
    //@{
    static const PHY_AmmoDotationClass obus_;
    static const PHY_AmmoDotationClass missileAir_;
    static const PHY_AmmoDotationClass missileSol_;
    static const PHY_AmmoDotationClass mitraille_;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_AmmoDotationClass* Find( ASN1T_EnumAmmunitionFamily nAsnID );
    static const PHY_AmmoDotationClass* Find( int nID );
    static const PHY_AmmoDotationClass* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
          int          GetID  () const;
          ASN1T_EnumAmmunitionFamily GetAsnID() const;
    const std::string& GetName() const;
    //@}

    //! @name Operations
    //@{
    bool operator==( const PHY_AmmoDotationClass& rhs ) const;
    bool operator!=( const PHY_AmmoDotationClass& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< int, const PHY_AmmoDotationClass* > T_TypeMap;
    typedef T_TypeMap::const_iterator                     CIT_TypeMap;

    enum E_Type
    {
        eObus = 0, 
        eMissileSol,
        eMissileAir,
        eMitraille,
    };
    //@}

private:
     PHY_AmmoDotationClass( const std::string& strName, E_Type nType, ASN1T_EnumAmmunitionFamily nAsnID );
    ~PHY_AmmoDotationClass();

private:
    const std::string               strName_;
    const E_Type                    nType_;
    const ASN1T_EnumAmmunitionFamily nAsnID_;

private:
    static T_TypeMap types_;
};

#include "PHY_AmmoDotationClass.inl"

#endif // __PHY_AmmoDotationClass_h_
