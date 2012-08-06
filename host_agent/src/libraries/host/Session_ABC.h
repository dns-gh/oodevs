// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SESSION_ABC_H
#define SESSION_ABC_H

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

namespace boost
{
    template< typename T > class shared_ptr;
namespace filesystem
{
    class path;
}
namespace uuids
{
    struct uuid;
}
}

namespace runtime
{
    struct Runtime_ABC;
}

namespace web
{
namespace session
{
    struct Config;
}
}

namespace host
{
    typedef boost::filesystem::path Path;
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;
}

namespace host
{
// =============================================================================
/** @class  Session_ABC
    @brief  Session_ABC interface
*/
// Created: BAX 2012-03-16
// =============================================================================
struct Session_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Session_ABC() {}
    virtual ~Session_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual Uuid GetId() const = 0;
    virtual Path GetRoot() const = 0;
    virtual Uuid GetNode() const = 0;
    virtual Path GetExercise() const = 0;
    virtual std::string GetName() const = 0;
    virtual int GetPort() const = 0;
    virtual Tree GetProperties() const = 0;
    //@}

    //! @name Public methods
    //@{
    virtual Tree Save() const = 0;
    virtual bool Start( const runtime::Runtime_ABC& runtime, const Path& apps ) = 0;
    virtual bool Stop() = 0;
    virtual bool Refresh() = 0;
    virtual bool RefreshSize() = 0;
    virtual bool Poll() = 0;
    virtual bool Pause() = 0;
    virtual void Remove() = 0;
    virtual bool Update( const Tree& cfg ) = 0;
    virtual bool Archive() = 0;
    virtual bool Restore() = 0;
    //@}
};

// =============================================================================
/** @class  SessionFactory_ABC
    @brief  SessionFactory_ABC interface
*/
// Created: BAX 2012-05-23
// =============================================================================
struct SessionFactory_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             SessionFactory_ABC() {}
    virtual ~SessionFactory_ABC() {}
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< Session_ABC > Ptr;
    //@}

    //! @name Methods
    //@{
    virtual Ptr Make( const Path& root, const Uuid& node, const web::session::Config& cfg, const std::string& exercise ) const = 0;
    virtual Ptr Make( const Path& tag ) const = 0;
    //@}
};
}

#endif // SESSION_ABC_H
