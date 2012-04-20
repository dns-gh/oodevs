// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SESSION_FACTORY_H
#define SESSION_FACTORY_H

#include "SessionController_ABC.h"

#include <boost/filesystem/path.hpp>

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
    class Runtime_ABC;
    class Process_ABC;
}

namespace host
{
    class FileSystem_ABC;
    class Pool_ABC;
    class PortFactory_ABC;
    class SecurePool;
    class Session;
    class UuidFactory_ABC;
    template< typename T > class Container;

// =============================================================================
/** @class  SessionFactory
    @brief  SessionFactory interface
*/
// Created: BAX 2012-03-19
// =============================================================================
class SessionController : public SessionController_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SessionController( cpplog::BaseLogger& log,
                                const runtime::Runtime_ABC& runtime,
                                const FileSystem_ABC& system,
                                const UuidFactory_ABC& uuids,
                                const boost::filesystem::path& data,
                                const boost::filesystem::path& apps,
                                Pool_ABC& pool,
                                PortFactory_ABC& ports
                           );
    virtual ~SessionController();
    //@}

    //! @name Methods
    //@{
    virtual void        Reload( T_Predicate predicate );
    virtual T_Sessions  List( T_Predicate predicate = T_Predicate(), int offset = 0, int limit = INT_MAX ) const;
    virtual size_t      Count( T_Predicate predicate = T_Predicate() ) const;
    virtual bool        Has( const boost::uuids::uuid& id ) const;
    virtual T_Session   Get( const boost::uuids::uuid& id ) const;
    virtual T_Session   Create( const boost::uuids::uuid& node, const std::string& name, const std::string& exercise );
    virtual T_Session   Delete( const boost::uuids::uuid& id );
    virtual T_Session   Start( const boost::uuids::uuid& id ) const;
    virtual T_Session   Stop( const boost::uuids::uuid& id ) const;
    virtual T_Exercises GetExercises() const;
    //@}

private:
    //! @name Private methods
    //@{
    void Save( const Session& session ) const;
    boost::shared_ptr< runtime::Process_ABC > StartWith( const Session& session ) const;
    void Start( Session& session, bool mustSave ) const;
    void Stop( Session& session, bool skipSave ) const;
    boost::filesystem::path GetPath( const Session& session ) const;
    //@}

private:
    //! @name Private members
    //@{
    mutable cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const FileSystem_ABC& system_;
    const UuidFactory_ABC& uuids_;
    const boost::filesystem::path data_;
    const boost::filesystem::path apps_;
    const std::vector< std::string > exercises_;
    const std::auto_ptr< SecurePool > pool_;
    const std::auto_ptr< Container< Session > > sessions_;
    PortFactory_ABC& ports_;
    //@}
};

}

#endif // SESSION_FACTORY_H
