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
#include <boost/filesystem.hpp>

namespace runtime
{
    class Process_ABC;
}

namespace host
{

// =============================================================================
/** @class  Session
    @brief  Session class definition
*/
// Created: BAX 2012-03-16
// =============================================================================
struct SessionConfig
{
    int port;
    boost::filesystem::wpath applications;
    boost::filesystem::wpath data;
    boost::filesystem::wpath output;
};

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
             Session( const SessionConfig& config );
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
    const boost::uuids::uuid tag_;
    const SessionConfig config_;
    boost::shared_ptr< runtime::Process_ABC > process_;
    //@}
};

}

#endif // SESSION_H
