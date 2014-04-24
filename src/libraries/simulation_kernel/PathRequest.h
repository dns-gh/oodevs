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

class DEC_PathResult;

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
             PathRequest( const boost::shared_ptr< DEC_PathResult >& path, const sword::PathfindRequest& request,
                          unsigned int nCtx, unsigned int clientId, uint32_t id, bool stored );
            ~PathRequest();
    //@}

    //! @name Operations
    //@{
    bool Update();
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< DEC_PathResult > path_;
    const sword::PathfindRequest request_;
    std::unique_ptr< client::ComputePathfindAck > ack_;
    const unsigned int nCtx_;
    const unsigned int clientId_;
    const bool stored_;
    //@}
};

#endif // __PathRequest_h_
