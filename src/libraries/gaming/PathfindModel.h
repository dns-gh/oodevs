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
    class Controller;
    class Pathfind_ABC;
}

namespace sword
{
    class PathfindCreation;
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
             PathfindModel( kernel::Controller& controller );
    virtual ~PathfindModel();
    //@}

    //! @name tools::Resolver< Pathfind >
    //@{
    virtual void Purge();
    //@}

    //! @name Operations
    //@{
    void Create( const sword::PathfindCreation&    msg );
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
    //@}
};

#endif // __PathfindModel_h_
