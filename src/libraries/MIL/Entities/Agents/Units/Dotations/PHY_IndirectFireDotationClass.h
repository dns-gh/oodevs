// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:58 $
// $Revision: 1 $
// $Workfile: PHY_IndirectFireDotationClass.h $
//
// *****************************************************************************

#ifndef __PHY_IndirectFireDotationClass_h_
#define __PHY_IndirectFireDotationClass_h_

#include "MIL.h"

class PHY_DotationCategory;
class PHY_DotationCategory_IndirectFire_ABC;

// =============================================================================
// @class  PHY_IndirectFireDotationClass
// Created: JVT 2004-08-03
// =============================================================================
class PHY_IndirectFireDotationClass
{
    MT_COPYNOTALLOWED( PHY_IndirectFireDotationClass )

public:
    //! @name Statics
    //@{
    static const PHY_IndirectFireDotationClass explosif_;
    static const PHY_IndirectFireDotationClass grenade_;
    static const PHY_IndirectFireDotationClass aced_;
    static const PHY_IndirectFireDotationClass fumigene_;
    static const PHY_IndirectFireDotationClass eclairant_;
    static const PHY_IndirectFireDotationClass mine_;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_IndirectFireDotationClass* Find( int nID );
    static const PHY_IndirectFireDotationClass* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
          int          GetID  () const;
    const std::string& GetName() const;
    //@}

    //! @name Operations
    //@{
    PHY_DotationCategory_IndirectFire_ABC& InstanciateDotationCategory( const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive ) const;
    bool                                    operator==                ( const PHY_IndirectFireDotationClass& rhs ) const;
    bool                                    operator!=                ( const PHY_IndirectFireDotationClass& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< int, const PHY_IndirectFireDotationClass* > T_TypeMap;
    typedef T_TypeMap::const_iterator                                 CIT_TypeMap;

    typedef PHY_DotationCategory_IndirectFire_ABC& (*T_TypeInstancier)( const PHY_IndirectFireDotationClass&, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& );

    enum E_Type
    {
        eExplosif,
        eGrenade,
        eACED,
        eFumigene,
        eEclairant,
        eMine
    };
    //@}

private:
     PHY_IndirectFireDotationClass( const std::string& strName, E_Type nType, T_TypeInstancier typeInstancier );
    ~PHY_IndirectFireDotationClass();

private:
    const std::string       strName_;
    const E_Type            nType_;
    const T_TypeInstancier  typeInstancier_;

private:
    static T_TypeMap types_;
};

#include "PHY_IndirectFireDotationClass.inl"

#endif // __PHY_IndirectFireDotationClass_h_
