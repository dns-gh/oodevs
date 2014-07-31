// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PathfindModel_h_
#define __PathfindModel_h_

#include <tools/Resolver.h>
#include <tools/Observer_ABC.h>

class Pathfind;

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class CoordinateConverter_ABC;
    class Pathfind_ABC;
    class Population_ABC;
    class Profile_ABC;
}

namespace sword
{
    class Pathfind;
    class PathfindDestruction;
}

namespace actions
{
    class ActionsModel;
}

// =============================================================================
/** @class  PathfindModel
*/
// =============================================================================
class PathfindModel : public tools::Resolver< kernel::Pathfind_ABC >
                    , public tools::Observer_ABC
                    , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PathfindModel( kernel::Controller& controller,
                            actions::ActionsModel& actionsModel,
                            const tools::Resolver_ABC< kernel::Agent_ABC >& agents,
                            const tools::Resolver_ABC< kernel::Population_ABC >& populations,
                            const kernel::CoordinateConverter_ABC& converter,
                            const kernel::Profile_ABC& profile );
    virtual ~PathfindModel();
    //@}

    //! @name tools::Resolver< Pathfind >
    //@{
    virtual void Purge();
    //@}

    //! @name Operations
    //@{
    void Create( const sword::Pathfind&            msg );
    void Delete( const sword::PathfindDestruction& msg );
    //@}

private:
    //! @name helper
    //@{
    typedef tools::Resolver< kernel::Pathfind_ABC > T_Resolver;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    actions::ActionsModel& actionsModel_;
    const kernel::CoordinateConverter_ABC& converter_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& agents_;
    const tools::Resolver_ABC< kernel::Population_ABC >& populations_;
    const kernel::Profile_ABC& profile_;
    //@}
};

#endif // __PathfindModel_h_
