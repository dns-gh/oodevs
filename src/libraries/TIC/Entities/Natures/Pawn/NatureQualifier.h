// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureQualifier.h $
// $Author: Sbo $
// $Modtime: 14/06/05 17:14 $
// $Revision: 1 $
// $Workfile: NatureQualifier.h $
//
// *****************************************************************************

#ifndef __NatureQualifier_h_
#define __NatureQualifier_h_

namespace TIC
{

// =============================================================================
// @class  NatureQualifier
// Created: JVT 2004-08-03
// =============================================================================
class NatureQualifier
{
    MT_COPYNOTALLOWED( NatureQualifier )

public:
    //! @name Manager
    //@{
    static       void            Initialize();
    static       void            Terminate ();

    static const NatureQualifier* Find      ( const std::string&            strName );
    static const std::string*     Find      ( ASN1T_EnumUnitNatureQualifier asnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string&                  GetName   () const;
          uint                          GetID     () const;
          ASN1T_EnumUnitNatureQualifier GetAsnID  () const;
    const std::string&                  GetKey    () const;
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

    typedef std::map< std::string, const NatureQualifier*, sCaseInsensitiveLess > T_NatureQualifierMap;
    typedef T_NatureQualifierMap::const_iterator                                  CIT_NatureQualifierMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    NatureQualifier( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureQualifier nAsnID, const std::string& strKey );
    ~NatureQualifier();
    //@}

private:
    //! @name Member data
    //@{
    const std::string                   strName_;
    const E_Type                        nType_;
    const ASN1T_EnumUnitNatureQualifier nAsnID_;
    const std::string                   strKey_;

    static T_NatureQualifierMap         natureQualifier_;
    //@}
};

} // end namespace TIC

#include "NatureQualifier.inl"

#endif // __NatureQualifier_h_
