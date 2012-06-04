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

namespace boost
{
    class shared_mutex;
}

namespace runtime
{
    struct Runtime_ABC;
}

namespace host
{
    struct Port_ABC;
    struct PortFactory_ABC;

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
             Session( const Uuid& id, const Uuid& node, const std::string& name, const std::string& exercise, std::auto_ptr< Port_ABC > port );
             Session( const Tree& tree, const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports );
    virtual ~Session();
    //@}

    //! @name Session_ABC methods
    //@{
    virtual Uuid GetId() const;
    virtual Uuid GetNode() const;
    virtual std::string GetExercise() const;
    virtual std::string GetName() const;
    virtual int GetPort() const;
    virtual std::string GetConfiguration() const;
    virtual Tree GetProperties() const;
    //@}

    //! @name Public methods
    //@{
    Tree Save() const;
    bool Start( const T_Starter& starter );
    bool Stop();
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
    const Uuid id_;
    const Uuid node_;
    const std::string name_;
    const std::string exercise_;
    const std::auto_ptr< boost::shared_mutex > access_;
    const std::auto_ptr< Port_ABC > port_;
    T_Process process_;
    Status status_;
    //@}
};

}

#endif // SESSION_H
