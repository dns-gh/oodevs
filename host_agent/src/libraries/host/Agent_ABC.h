// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef AGENT_ABC_H
#define AGENT_ABC_H

#include <boost/noncopyable.hpp>
#include <boost/uuid/uuid.hpp>
#include <string>
#include <vector>

namespace host
{
// =============================================================================
/** @class  Reply
    @brief  Reply struct definition
*/
// Created: BAX 2012-03-07
// =============================================================================
struct Reply
{
    std::string data;
    bool valid;
    Reply( const std::string& data, bool valid = true )
        : data ( data )
        , valid( valid )
    {
        // NOTHING
    }
};

// =============================================================================
/** @class  Agent_ABC
    @brief  Agent_ABC interface
*/
// Created: BAX 2012-03-07
// =============================================================================
class Agent_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent_ABC() {}
    virtual ~Agent_ABC() {}
    //@}

    //! @name Session Methods
    //@{
    virtual Reply ListSessions ( int offset, int limit ) const = 0;
    virtual Reply CountSessions() const = 0;
    virtual Reply GetSession   ( const boost::uuids::uuid& id ) const = 0;
    virtual Reply CreateSession( const std::string& exercise, const std::string& name ) = 0;
    virtual Reply DeleteSession( const boost::uuids::uuid& id ) = 0;
    //@}

    //! @name Other Methods
    //@{
    virtual Reply ListExercises ( int offset, int limit ) const = 0;
    virtual Reply CountExercises() const = 0;
    //@}
};

}

#endif // AGENT_ABC_H
