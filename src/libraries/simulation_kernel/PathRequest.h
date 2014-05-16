// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __PathRequest_h_
#define __PathRequest_h_

#include <boost/optional.hpp>

class DEC_PathComputer;

namespace client
{
    class ComputePathfindAck;
}

// =============================================================================
/** @class  PathRequest
    @brief  Path request
*/
// Created: LGY 2014-04-16
// =============================================================================
class PathRequest : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
     PathRequest( const boost::shared_ptr< DEC_PathComputer >& computer, const sword::PathfindRequest& request,
                  unsigned int ctx, unsigned int clientId, uint32_t id, const boost::optional< uint32_t >& magic );
    ~PathRequest();
    //@}

    //! @name Operations
    //@{
    bool Update();
    //@}

private:
    void SendComputePathfindAck( bool ok );
    void SendPathfindCreation  ( bool ok );

private:
    //! @name Member data
    //@{
    boost::shared_ptr< DEC_PathComputer > computer_;
    const unsigned int ctx_;
    const unsigned int clientId_;
    const uint32_t id_;
    const boost::optional< uint32_t > magic_;
    const uint32_t unit_;
    bool published_;
    //@}
};

#endif // __PathRequest_h_
