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

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace boost
{
    class shared_mutex;
}

namespace runtime
{
    class Runtime_ABC;
    class Process_ABC;
}

namespace host
{
    class Port_ABC;
    class PortFactory_ABC;

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
             Session( const boost::uuids::uuid& id, const boost::uuids::uuid& node, const std::string& name, const std::string& exercise, std::auto_ptr< Port_ABC > port );
             Session( const boost::property_tree::ptree& tree, const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports );
    virtual ~Session();
    //@}

    //! @name Session_ABC methods
    //@{
    virtual boost::uuids::uuid GetId() const;
    virtual boost::uuids::uuid GetNode() const;
    virtual boost::property_tree::ptree GetProperties() const;
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    typedef boost::function< T_Process( const Session& ) > T_Starter;
    //@}

    //! @name Public methods
    //@{
    boost::property_tree::ptree Save() const;
    std::string GetConfiguration() const;
    bool Start( const T_Starter& starter );
    bool Stop();
    //@}

    //! @name Public members
    //@{
    const boost::uuids::uuid id_;
    const boost::uuids::uuid node_;
    const std::string name_;
    const std::string exercise_;
    const std::auto_ptr< Port_ABC > port_;
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
    //! @name Private members
    //@{
    const std::auto_ptr< boost::shared_mutex > access_;
    T_Process process_;
    Status status_;
    //@}
};

}

#endif // SESSION_H
