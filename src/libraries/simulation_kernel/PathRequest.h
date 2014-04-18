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
class DEC_PathFind_Manager;

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
             PathRequest( boost::shared_ptr< DEC_PathResult >& path, const sword::PathfindRequest& request,
                          DEC_PathFind_Manager& pathfindManager, unsigned int nCtx, unsigned int clientId,
                          unsigned long id, unsigned int unitId, bool stored );
    virtual ~PathRequest();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    bool IsStored() const;
    bool Update();
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< DEC_PathResult > path_;
    const sword::PathfindRequest request_;
    unsigned int nCtx_;
    unsigned int clientId_;
    unsigned long id_;
    unsigned long unitId_;
    bool computed_;
    bool stored_;
    //@}
};

#endif // __PathRequest_h_
