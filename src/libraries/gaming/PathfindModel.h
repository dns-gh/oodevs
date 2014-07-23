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
    class ActionController;
    class Agent_ABC;
    class Controller;
    class CoordinateConverter_ABC;
    class Pathfind_ABC;
    class Population_ABC;
}

namespace sword
{
    class Pathfind;
    class PathfindDestruction;
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
                            const kernel::ActionController& actions,
                            const tools::Resolver_ABC< kernel::Agent_ABC >& agents,
                            const tools::Resolver_ABC< kernel::Population_ABC >& populations,
                            const kernel::CoordinateConverter_ABC& converter );
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
    const kernel::ActionController& actions_;
    const kernel::CoordinateConverter_ABC& converter_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& agents_;
    const tools::Resolver_ABC< kernel::Population_ABC >& populations_;
    //@}
};

#endif // __PathfindModel_h_
