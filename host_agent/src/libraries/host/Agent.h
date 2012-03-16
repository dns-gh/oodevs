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

namespace runtime
{
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
             Agent( const runtime::Runtime_ABC& runtime );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual Reply List( int offset, int limit ) const;
    virtual Reply Start( const std::string& app, const std::vector< std::string >& args, const std::string& run );
    virtual Reply Stop( int pid );
    //@}

private:
    //! @name Member data
    //@{
    const runtime::Runtime_ABC& runtime_;
    //@}
};

}

#endif // AGENT_H
