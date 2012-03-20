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
#include "SessionFactory_ABC.h"

namespace runtime
{
    class Runtime_ABC;
    class Process_ABC;
}

namespace host
{
    class UuidFactory_ABC;
    class FileSystem_ABC;

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
             Session( const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids,
                      const FileSystem_ABC& system, const boost::filesystem::wpath& data,
                      const boost::filesystem::wpath& applications, const std::string& exercise,
                      const std::string& name, int port );
    virtual ~Session();
    //@}

    //! @name Overrided methods
    //@{
    virtual boost::uuids::uuid GetTag() const;
    virtual std::string ToJson() const;
    virtual void Start();
    virtual void Stop();
    //@}

private:
    //! @name Member data
    //@{
    const runtime::Runtime_ABC& runtime_;
    const FileSystem_ABC& system_;
    const boost::uuids::uuid tag_;
    const boost::filesystem::wpath& data_;
    const boost::filesystem::wpath& applications_;
    const std::string exercise_;
    const std::string name_;
    int port_;
    boost::shared_ptr< runtime::Process_ABC > process_;
    //@}
};

}

#endif // SESSION_H
