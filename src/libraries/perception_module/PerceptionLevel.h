// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_LEVEL_H
#define SWORD_PERCEPTION_PERCEPTION_LEVEL_H

#include <map>
#include <boost/noncopyable.hpp>

namespace sword
{
namespace perception
{
// =============================================================================
// @class  PerceptionLevel
// Created: JVT 2004-08-03
// =============================================================================
class PerceptionLevel : private boost::noncopyable
{
public:
    //! @name Member data
    //@{
    static const PerceptionLevel identified_;
    static const PerceptionLevel recognized_;
    static const PerceptionLevel detected_;
    static const PerceptionLevel notSeen_;
    //@}

public:
    //! @name Operations
    //@{
    static void Initialize();
    static void Terminate ();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    unsigned int GetID() const;
    bool IsBestLevel() const;

    static const PerceptionLevel& FindPerceptionLevel( unsigned int nID );
    //@}

    //! @name Operators
    //@{
    bool operator ==( const PerceptionLevel& rhs ) const;
    bool operator !=( const PerceptionLevel& rhs ) const;
    bool operator < ( const PerceptionLevel& rhs ) const;
    bool operator <=( const PerceptionLevel& rhs ) const;
    bool operator > ( const PerceptionLevel& rhs ) const;
    bool operator >=( const PerceptionLevel& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Level
    {
        eNotSeen    = 0,
        eDetected   = 1,
        eRecognized = 2,
        eIdentified = 3,
    };
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             PerceptionLevel( const std::string& strName, E_Level nLevel );
    virtual ~PerceptionLevel();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, const PerceptionLevel* > T_LevelMap;
    typedef T_LevelMap::const_iterator                     CIT_LevelMap;
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    const E_Level nLevel_;
    static T_LevelMap levels_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_LEVEL_H
