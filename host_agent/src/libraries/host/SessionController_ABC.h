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

#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <string>
#include <vector>

namespace boost
{
    template< typename T > class function;
    template< typename T > class shared_ptr;
    namespace uuids
    {
        struct uuid;
    }
}

namespace web
{
    struct Chunker_ABC;
namespace session
{
    struct Config;
}
}

namespace host
{
    struct Session_ABC;
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;
    typedef boost::filesystem::path Path;
}

namespace host
{
// =============================================================================
/** @class  SessionController_ABC
    @brief  SessionController_ABC interface
*/
// Created: BAX 2012-03-19
// =============================================================================
struct SessionController_ABC : public boost::noncopyable
{
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
    virtual void        Reload     ( T_Predicate predicate ) = 0;
    virtual T_Sessions  List       ( T_Predicate predicate, int offset, int limit ) const = 0;
    virtual size_t      Count      ( T_Predicate predicate ) const = 0;
    virtual T_Session   Get        ( const Uuid& node, const Uuid& id ) const = 0;
    virtual T_Session   Create     ( const Uuid& node, const web::session::Config& cfg, const std::string& exercise ) = 0;
    virtual T_Session   Delete     ( const Uuid& node, const Uuid& id ) = 0;
    virtual T_Session   Start      ( const Uuid& node, const Uuid& id, const std::string& checkpoint ) const = 0;
    virtual T_Session   Stop       ( const Uuid& node, const Uuid& id ) const = 0;
    virtual T_Session   Pause      ( const Uuid& node, const Uuid& id ) const = 0;
    virtual T_Session   Update     ( const Uuid& node, const Uuid& id, const Tree& cfg ) const = 0;
    virtual T_Session   Archive    ( const Uuid& node, const Uuid& id ) const = 0;
    virtual T_Session   Restore    ( const Uuid& node, const Uuid& id ) const = 0;
    virtual void        Download   ( const Uuid& node, const Uuid& id, web::Chunker_ABC& dst ) const = 0;
    virtual T_Session   Replay     ( const Uuid& node, const Uuid& id ) = 0;
    virtual void        NotifyNode ( const Uuid& node ) = 0;
    virtual void        DownloadLog( const Uuid& node, const Uuid& id, web::Chunker_ABC& dst, const std::string& logFile, int limitSize ) const = 0;

    //@}
};
}

#endif // SESSION_CONTROLLER_ABC_H
