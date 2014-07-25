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

#include "simulation_terrain/TER_PathFindRequest_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace sword
{
    class Pathfind;
}

class DEC_PathComputer_ABC;
class DEC_PathFind_Manager_ABC;
class MIL_Agent_ABC;
class TER_World_ABC;

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
             DEC_PathFindRequest( DEC_PathFind_Manager_ABC& manager, const boost::shared_ptr< DEC_PathComputer_ABC >& computer,
                                  const sword::Pathfind& pathfind, const TER_World_ABC& world );
    virtual ~DEC_PathFindRequest();
    //@}

    //! @name Operations
    //@{
    virtual bool IgnoreDynamicObjects() const;
    virtual void FindPath( TER_Pathfinder_ABC& pathfind );
    //@}

private:
    //! @name Helpers
    //@{
    bool IsItinerary() const;
    //@}

private:
    //! @name Member data
    //@{
    DEC_PathFind_Manager_ABC& manager_;
    boost::weak_ptr< DEC_PathComputer_ABC > computer_;
    const sword::Pathfind pathfind_;
    const TER_World_ABC& world_;
    //@}
};

#endif // DEC_PATHFINDREQUEST_H

