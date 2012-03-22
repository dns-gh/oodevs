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

namespace host
{
    class Session_ABC;
    class SessionFactory_ABC;

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
             Agent( const SessionFactory_ABC& sessionFactory );
    virtual ~Agent();
    //@}

    //! @name Session Methods
    //@{
    virtual Reply ListSessions ( int offset, int limit ) const;
    virtual Reply CountSessions() const;
    virtual Reply GetSession   ( const boost::uuids::uuid& tag ) const;
    virtual Reply CreateSession( const std::string& exercise, const std::string& name );
    virtual Reply DeleteSession( const boost::uuids::uuid& tag );
    //@}

    //! @name Type helpers
    //@{
    typedef std::map< boost::uuids::uuid, boost::shared_ptr< Session_ABC > > T_Sessions;
    //@}

private:
    //! @name Member data
    //@{
    const SessionFactory_ABC& sessionFactory_;
    std::auto_ptr< boost::shared_mutex > access_;
    T_Sessions sessions_;
    //@}
};

}

#endif // AGENT_H
