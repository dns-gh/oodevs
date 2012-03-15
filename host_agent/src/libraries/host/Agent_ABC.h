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
#include <boost/shared_ptr.hpp>
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

    //! @name Operations
    //@{
    virtual Reply List( int offset, int limit ) const = 0;
    virtual Reply Start( const std::string& app, const std::vector< std::string >& args, const std::string& run ) = 0;
    virtual Reply Stop( int pid ) = 0;
    //@}
};

}

#endif // AGENT_ABC_H
