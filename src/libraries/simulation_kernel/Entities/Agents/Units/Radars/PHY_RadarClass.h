// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Radars/PHY_RadarClass.h $
// $Author: Nld $
// $Modtime: 2/05/05 18:20 $
// $Revision: 1 $
// $Workfile: PHY_RadarClass.h $
//
// *****************************************************************************

#ifndef __PHY_RadarClass_h_
#define __PHY_RadarClass_h_

#include "MT_Tools/MT_String.h"

// =============================================================================
// @class  PHY_RadarClass
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RadarClass : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_RadarClass* > T_RadarClassMap;
    typedef T_RadarClassMap::const_iterator              CIT_RadarClassMap;

    static PHY_RadarClass radar_;
    static PHY_RadarClass tapping_;
    static PHY_RadarClass tappingRadar_;
    //@}

public:
    //! @name Manager
    //@{
    static       void             Initialize     ();
    static       void             Terminate      ();
    static const PHY_RadarClass*  Find           ( const std::string& strName );
    static const PHY_RadarClass*  Find           ( unsigned int nID );
    static const T_RadarClassMap& GetRadarClasses();
    static       std::size_t      GetNbrClasses  ();
    //@}

    //! @name Accessors
    //@{
          unsigned int         GetID   () const;
    const std::string& GetName () const;
          bool         IsActive() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_RadarClass& rhs ) const;
    bool operator!=( const PHY_RadarClass& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eRadar,
        eTapping,
        eTappingRadar
    };
    //@}

private:
     PHY_RadarClass( const std::string& strName, E_Type nType, bool bIsActive );
    ~PHY_RadarClass();

private:
    const E_Type      nType_;
          std::string strName_;
          bool        bIsActive_;

private:
    static T_RadarClassMap radarClasses_;
};

#endif // __PHY_RadarClass_h_
