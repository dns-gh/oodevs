// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __GhostModel_h_
#define __GhostModel_h_

#include "tools/Resolver.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Ghost_ABC;
    class GhostPrototype;
}

namespace xml
{
    class xostream;
    class xistream;
}

class GhostFactory_ABC;

// =============================================================================
/** @class  GhostModel
    @brief  GhostModel
*/
// Created: ABR 2011-10-14
// =============================================================================
class GhostModel : public tools::Resolver< kernel::Ghost_ABC >
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< kernel::Ghost_ABC >
                 , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             GhostModel( kernel::Controllers& controllers, GhostFactory_ABC& ghostFactory );
    virtual ~GhostModel();
    //@}

    //! @name Operations
    //@{
    kernel::Ghost_ABC* Create( kernel::Entity_ABC& parent, const kernel::GhostPrototype& prototype, const geometry::Point2f& position );
    void Create( xml::xistream& xis, kernel::Entity_ABC& parent );
    void Purge();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Ghost_ABC& ghost );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    GhostFactory_ABC&    ghostFactory_;
    //@}
};

#endif // __GhostModel_h_
