// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SESSION_CONTROLLER_ABC_H
#define SESSION_CONTROLLER_ABC_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>

namespace host
{
    class Session_ABC;

// =============================================================================
/** @class  SessionController_ABC
    @brief  SessionController_ABC interface
*/
// Created: BAX 2012-03-19
// =============================================================================
class SessionController_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SessionController_ABC() {}
    virtual ~SessionController_ABC() {}
    //@}

    //! @name Type helpers
    //@{
    typedef Session_ABC T_Base;
    typedef boost::shared_ptr< T_Base > T_Session;
    typedef std::vector< T_Session > T_Sessions;
    typedef std::vector< std::string > T_Exercises;
    typedef boost::function< bool( const T_Base& ) > T_Predicate;
    //@}

    //! @name Methods
    //@{
    virtual void        Reload( T_Predicate predicate ) = 0;
    virtual T_Sessions  List( T_Predicate predicate = T_Predicate(), int offset = 0, int limit = INT_MAX ) const = 0;
    virtual size_t      Count( T_Predicate predicate = T_Predicate() ) const = 0;
    virtual bool        Has( const boost::uuids::uuid& id ) const = 0;
    virtual T_Session   Get( const boost::uuids::uuid& id ) const = 0;
    virtual T_Session   Create( const boost::uuids::uuid& node, const std::string& name, const std::string& exercise ) = 0;
    virtual T_Session   Delete( const boost::uuids::uuid& id ) = 0;
    virtual T_Session   Start( const boost::uuids::uuid& id ) const = 0;
    virtual T_Session   Stop( const boost::uuids::uuid& id ) const = 0;
    virtual T_Exercises GetExercises() const = 0;
    //@}
};

}

#endif // SESSION_CONTROLLER_ABC_H
