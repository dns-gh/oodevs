// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SESSION_FACTORY_ABC_H
#define SESSION_FACTORY_ABC_H

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <map>

namespace host
{
    class Session_ABC;

// =============================================================================
/** @class  SessionFactory_ABC
    @brief  SessionFactory_ABC interface
*/
// Created: BAX 2012-03-19
// =============================================================================
class SessionFactory_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SessionFactory_ABC() {}
    virtual ~SessionFactory_ABC() {}
    //@}

    //! @name Type helpers
    //@{
    typedef std::map< boost::uuids::uuid, boost::shared_ptr< Session_ABC > > T_Sessions;
    //@}

    //! @name Methods
    //@{
    virtual boost::shared_ptr< Session_ABC > Create( const boost::uuids::uuid& node, const std::string& exercise, const std::string& name ) const = 0;
    virtual T_Sessions Reload() const = 0;
    virtual std::vector< std::string > GetExercises() const = 0;
    //@}
};

}

#endif // SESSION_FACTORY_ABC_H
