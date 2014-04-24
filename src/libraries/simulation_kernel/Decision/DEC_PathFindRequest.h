// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef DEC_PATHFINDREQUEST_H
#define DEC_PATHFINDREQUEST_H

#include "MT_Tools/MT_Profiler.h"
#include "simulation_terrain/TER_PathFindRequest_ABC.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

class DEC_Path_ABC;
class DEC_PathFind_Manager;
class MIL_Agent_ABC;

// DEC_PathFindRequest insulates DEC_Path_ABC cleanup logic from
// TER_PathFinderThread. While the cleanup code itself belongs to DEC_Path_ABC,
// the deferred cleanup logic is only required by the threaded implementation
// of the path finder and has nothing to do in DEC_Path_ABC. We need both a
// request and an executor class.
class DEC_PathFindRequest: public TER_PathFindRequest_ABC,
                           private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_PathFindRequest( DEC_PathFind_Manager* m, const boost::shared_ptr< DEC_Path_ABC >& p );
    virtual ~DEC_PathFindRequest();
    //@}

    //! @name Operations
    //@{
    virtual void FindPath( TER_Pathfinder_ABC& pathfind );
    const boost::shared_ptr< DEC_Path_ABC >& GetPath() const;
    bool IsPathForUnit( MIL_Agent_ABC* pion ) const;
    //@}

private:
    //! @name Member data
    //@{
    DEC_PathFind_Manager* const manager_;
    const boost::shared_ptr< DEC_Path_ABC > path_;
    MT_Profiler profiler_;
    //@}
};

#endif // DEC_PATHFINDREQUEST_H

