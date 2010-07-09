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

namespace Common
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
          Common::EnumNatureLevel GetAsnID() const;
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

    typedef std::map< std::string, const PHY_NatureLevel*, sCaseInsensitiveLess > T_NatureLevelMap;
    typedef T_NatureLevelMap::const_iterator                                      CIT_NatureLevelMap;
    //@}

private:
     PHY_NatureLevel( const std::string& strName, E_Type nType, Common::EnumNatureLevel nAsnID );
    virtual ~PHY_NatureLevel();

private:
    const std::string     strName_;
    const E_Type          nType_;
    const Common::EnumNatureLevel nAsnID_;

private:
    static T_NatureLevelMap natureLevels_;
};

#endif // __PHY_NatureLevel_h_
