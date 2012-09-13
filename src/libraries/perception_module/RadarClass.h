// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_RADAR_CLASS_H
#define SWORD_PERCEPTION_RADAR_CLASS_H

#include <boost/noncopyable.hpp>
#include <map>
#include <string>

namespace sword
{
namespace perception
{
// =============================================================================
/** @class  RadarClass
    @brief  Radar class
*/
// Created: SLI 2012-04-12
// =============================================================================
class RadarClass : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const RadarClass* > T_RadarClassMap;
    typedef T_RadarClassMap::const_iterator          CIT_RadarClassMap;

    static RadarClass radar_;
    static RadarClass tapping_;
    static RadarClass tappingRadar_;
    //@}

public:
    //! @name Manager
    //@{
    static       void             Initialize     ();
    static       void             Terminate      ();
    static const RadarClass*  Find           ( const std::string& strName );
    static const RadarClass*  Find           ( std::size_t nID );
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
    bool operator==( const RadarClass& rhs ) const;
    bool operator!=( const RadarClass& rhs ) const;
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
     RadarClass( const std::string& strName, E_Type nType, bool bIsActive );
    ~RadarClass();

private:
    const E_Type      nType_;
          std::string strName_;
          bool        bIsActive_;

private:
    static T_RadarClassMap radarClasses_;
};

}
}

#endif // SWORD_PERCEPTION_RADAR_CLASS_H
