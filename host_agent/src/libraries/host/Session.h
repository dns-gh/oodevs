// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SESSION_H
#define SESSION_H

#include "Session_ABC.h"

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/uuid/uuid.hpp>

namespace boost
{
    class shared_mutex;
}

namespace runtime
{
    struct Runtime_ABC;
    struct Process_ABC;
}

namespace web
{
    struct Client_ABC;
}

namespace host
{
    struct Node_ABC;
    struct Port_ABC;
    struct PortFactory_ABC;
    typedef boost::shared_ptr< const Port_ABC > Port;

// =============================================================================
/** @class  Session
    @brief  Session class definition
*/
// Created: BAX 2012-03-16
// =============================================================================
class Session : public Session_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Session( const Path& root, const Uuid& id, const Node_ABC& node, const std::string& name, const std::string& exercise, const Port& port, web::Client_ABC& client );
             Session( const Path& root, const Tree& tree, const Node_ABC& node, const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports, web::Client_ABC& client );
    virtual ~Session();
    //@}

    //! @name Session_ABC methods
    //@{
    virtual Uuid GetId() const;
    virtual Path GetRoot() const;
    virtual Uuid GetNode() const;
    virtual Path GetExercise() const;
    virtual std::string GetName() const;
    virtual int GetPort() const;
    virtual Tree GetProperties() const;
    //@}

    //! @name Public methods
    //@{
    virtual Path GetPath( const std::string& type ) const;
    virtual Path GetOutput() const;
    virtual Tree Save() const;
    virtual bool Start( const runtime::Runtime_ABC& runtime, const runtime::FileSystem_ABC& system, const Path& apps );
    virtual bool Stop();
    virtual void Update();
    virtual void Poll();
    virtual bool Pause();
    virtual void Remove( const runtime::FileSystem_ABC& system, runtime::Async& async );
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    //@}

    //! @name Status enumeration
    //@{
    enum Status
    {
        STATUS_STOPPED,
        STATUS_PLAYING,
        STATUS_REPLAYING,
        STATUS_PAUSED,
        STATUS_COUNT
    };
    //@}

private:
    //! @name Private methods
    //@{
    Tree GetProperties( bool save ) const;
    template< typename T > std::pair< T_Process, bool > StopProcess( T& lock );
    template< typename T > std::pair< T_Process, bool > ModifyStatus( T& lock, Status next );
    //@}

private:
    //! @name Private members
    //@{
    const Uuid id_;
    const Path root_;
    const Node_ABC& node_;
    const std::string name_;
    const Tree links_;
    const Port port_;
    web::Client_ABC& client_;
    mutable boost::mutex access_;
    T_Process process_;
    Status status_;
    bool polling_;
    size_t counter_;
    //@}
};

}

#endif // SESSION_H
