// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureLevel.h $
// $Author: Nld $
// $Modtime: 6/04/05 10:45 $
// $Revision: 3 $
// $Workfile: PHY_NatureLevel.h $
//
// *****************************************************************************

#ifndef __PHY_NatureLevel_h_
#define __PHY_NatureLevel_h_

#include "MT_Tools/MT_String.h"

namespace sword
{
    enum EnumNatureLevel;
}

// =============================================================================
// @class  PHY_NatureLevel
// Created: JVT 2004-08-03
// =============================================================================
class PHY_NatureLevel : private boost::noncopyable
{
public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_NatureLevel* Find( const std::string& strName );
    static const PHY_NatureLevel* Find( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&            GetName () const;
          unsigned int            GetID   () const;
          sword::EnumNatureLevel GetAsnID() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
      eNatureLevel_none = 0,
      eNatureLevel_b = 1,
      eNatureLevel_o = 2,
      eNatureLevel_c = 3,
      eNatureLevel_oo = 4,
      eNatureLevel_ooo = 5,
      eNatureLevel_i = 6,
      eNatureLevel_ii = 7,
      eNatureLevel_iii = 8,
      eNatureLevel_x = 9,
      eNatureLevel_xx = 10,
      eNatureLevel_xxx = 11,
      eNatureLevel_xxxx = 12,
      eNatureLevel_xxxxx = 13
    };

    typedef std::map< std::string, const PHY_NatureLevel* > T_NatureLevelMap;
    typedef T_NatureLevelMap::const_iterator              CIT_NatureLevelMap;
    //@}

private:
     PHY_NatureLevel( const std::string& strName, E_Type nType, sword::EnumNatureLevel nAsnID );
    virtual ~PHY_NatureLevel();

private:
    const std::string     strName_;
    const E_Type          nType_;
    const sword::EnumNatureLevel nAsnID_;

private:
    static T_NatureLevelMap natureLevels_;
};

#endif // __PHY_NatureLevel_h_
