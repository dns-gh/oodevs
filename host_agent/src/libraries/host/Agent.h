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

namespace runtime
{
    class Runtime_ABC;
}

namespace host
{
    class Session_ABC;

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

    //! @name Session Methods
    //@{
    virtual Reply ListSessions ( int offset, int limit ) const;
    virtual Reply CountSessions() const;
    virtual Reply GetSession   ( const boost::uuids::uuid& tag ) const;
    virtual Reply CreateSession( int port );
    virtual Reply DeleteSession( const boost::uuids::uuid& tag );
    //@}

private:
    //! @name Private helpers
    //@{
    typedef std::map< boost::uuids::uuid, boost::shared_ptr< Session_ABC > > T_Sessions;
    //@}

    //! @name Private operations
    //@{
    boost::shared_ptr< Session_ABC > ExtractSession( const boost::uuids::uuid& tag );
    void AddSession( boost::shared_ptr< Session_ABC > ptr );
    //@}

    //! @name Member data
    //@{
    const runtime::Runtime_ABC& runtime_;
    std::auto_ptr< boost::shared_mutex > access_;
    T_Sessions sessions_;
    //@}
};

}

#endif // AGENT_H
