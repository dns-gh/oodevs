// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureQualifier.h $
// $Author: Jvt $
// $Modtime: 9/12/04 15:33 $
// $Revision: 2 $
// $Workfile: PHY_NatureQualifier.h $
//
// *****************************************************************************

#ifndef __PHY_NatureQualifier_h_
#define __PHY_NatureQualifier_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

// =============================================================================
// @class  PHY_NatureQualifier
// Created: JVT 2004-08-03
// =============================================================================
class PHY_NatureQualifier
{
    MT_COPYNOTALLOWED( PHY_NatureQualifier )

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_NatureQualifier* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string&                  GetName () const;
          uint                          GetID   () const;
          ASN1T_EnumUnitNatureQualifier GetAsnID() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eLourd,
        eLeger,
        eMoyen,
        eNone 
    };

    typedef std::map< std::string, const PHY_NatureQualifier*, sCaseInsensitiveLess > T_NatureQualifierMap;
    typedef T_NatureQualifierMap::const_iterator                                      CIT_NatureQualifierMap;
    //@}

private:
     PHY_NatureQualifier( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureQualifier nAsnID );
    ~PHY_NatureQualifier();

private:
    const std::string                   strName_;
    const E_Type                        nType_;
    const ASN1T_EnumUnitNatureQualifier nAsnID_;

private:
    static T_NatureQualifierMap natureQualifier_;
};

#include "PHY_NatureQualifier.inl"

#endif // __PHY_NatureQualifier_h_
