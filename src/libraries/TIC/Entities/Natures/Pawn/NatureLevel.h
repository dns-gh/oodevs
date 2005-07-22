// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureLevel.h $
// $Author: Sbo $
// $Modtime: 7/06/05 9:58 $
// $Revision: 1 $
// $Workfile: NatureLevel.h $
//
// *****************************************************************************

#ifndef __NatureLevel_h_
#define __NatureLevel_h_

namespace TIC
{

// =============================================================================
// @class  NatureLevel
// Created: JVT 2004-08-03
// =============================================================================
class NatureLevel
{
    MT_COPYNOTALLOWED( NatureLevel )

public:
    //! @name Manager
    //@{
    static       void           Initialize();
    static       void           Terminate ();

    static const NatureLevel*   Find      ( const std::string&    strName );
    static const std::string*   Find      ( ASN1T_EnumNatureLevel asnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string&          GetName   () const;
          uint                  GetID     () const;
          ASN1T_EnumNatureLevel GetAsnID  () const;
    const std::string&          GetKey    () const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
      eNatureLevel_none = 0,
      eNatureLevel_o = 1,
      eNatureLevel_oo = 2,
      eNatureLevel_ooo = 3,
      eNatureLevel_i = 4,
      eNatureLevel_ii = 5,
      eNatureLevel_iii = 6,
      eNatureLevel_x = 7,
      eNatureLevel_xx = 8,
      eNatureLevel_xxx = 9,
      eNatureLevel_xxxx = 10,
      eNatureLevel_xxxxx = 11
    };

    typedef std::map< std::string, const NatureLevel*, sCaseInsensitiveLess > T_NatureLevelMap;
    typedef T_NatureLevelMap::const_iterator                                  CIT_NatureLevelMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    NatureLevel( const std::string& strName, E_Type nType, ASN1T_EnumNatureLevel nAsnID, const std::string& strKey );
    ~NatureLevel();
    //@}

private:
    //! @name Member data
    //@{
    const std::string     strName_;
    const E_Type          nType_;
    const ASN1T_EnumNatureLevel nAsnID_;
    const std::string     strKey_;

    static T_NatureLevelMap natureLevel_;
    //@}
};

} // end namespace TIC

#include "NatureLevel.inl"

#endif // __NatureLevel_h_
