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

namespace boost
{
    class shared_mutex;
}

namespace runtime
{
    class Runtime_ABC;
    class Process_ABC;
}

namespace xml
{
    class xistream;
}

namespace host
{
    class FileSystem_ABC;
    class PortFactory_ABC;
    class Port_ABC;
    class UuidFactory_ABC;

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
                      const boost::filesystem::wpath& applications,
                      const std::string& exercise, const std::string& name,
                      PortFactory_ABC& ports );
             Session( const runtime::Runtime_ABC& runtime, const FileSystem_ABC& system,
                      const boost::filesystem::wpath& data, const boost::filesystem::wpath& applications,
                      xml::xistream& xis, PortFactory_ABC& ports );
    virtual ~Session();
    //@}

    //! @name Overrided methods
    //@{
    virtual boost::uuids::uuid GetTag() const;
    virtual std::string ToJson() const;
    virtual void Save() const;
    virtual void Start();
    virtual void Stop();
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
    void CheckPaths() const;
    std::string ToXml() const;
    boost::filesystem::wpath GetPath() const;
    //@}

private:
    //! @name Member data
    //@{
    const runtime::Runtime_ABC& runtime_;
    const FileSystem_ABC& system_;
    const boost::uuids::uuid id_;
    const boost::filesystem::wpath data_;
    const boost::filesystem::wpath applications_;
    const std::string exercise_;
    const std::string name_;
    std::auto_ptr< boost::shared_mutex > access_;
    boost::shared_ptr< runtime::Process_ABC > process_;
    std::auto_ptr< Port_ABC > port_;
    Status status_;
    //@}
};

}

#endif // SESSION_H
