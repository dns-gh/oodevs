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
                  unsigned int nCtx, unsigned int clientId, uint32_t id, bool store );
    ~PathRequest();
    //@}

    //! @name Operations
    //@{
    bool Update();
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< DEC_PathComputer > computer_;
    const unsigned int nCtx_;
    const unsigned int clientId_;
    const uint32_t id_;
    const uint32_t unit_;
    const bool store_;
    bool published_;
    //@}
};

#endif // __PathRequest_h_
