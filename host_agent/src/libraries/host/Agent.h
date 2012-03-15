// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef AGENT_H
#define AGENT_H

#include "Agent_ABC.h"
#include <boost/shared_ptr.hpp>
#include <map>

namespace boost
{
    class shared_mutex;
}

namespace process
{
    class Process_ABC;
    class Runtime_ABC;
}

namespace host
{
// =============================================================================
/** @class  Agent
    @brief  Agent class definition
*/
// Created: BAX 2012-03-07
// =============================================================================
class Agent : public Agent_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent( const process::Runtime_ABC& runtime );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual Reply List( int offset, int limit ) const;
    virtual Reply Start( const std::string& app, const std::vector< std::string >& args, const std::string& run );
    virtual Reply Stop( int pid );
    //@}

private:
    //! @name Private typedef helpers
    //@{
    typedef std::map< int, boost::shared_ptr< process::Process_ABC > > T_Processes;
    //@}

    //! @name Private operations
    //@{
    boost::shared_ptr< process::Process_ABC > Extract( int pid );
    void AddProcess( boost::shared_ptr< process::Process_ABC > ptr );
    //@}

private:
    //! @name Member data
    //@{
    const process::Runtime_ABC& runtime_;
    std::auto_ptr< boost::shared_mutex > access_;
    T_Processes processes_;
    //@}
};

}

#endif // AGENT_H
