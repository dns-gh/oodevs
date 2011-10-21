// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __GhostFactory_ABC_h_
#define __GhostFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class Ghost_ABC;
    class GhostPrototype;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  GhostFactory_ABC
    @brief  GhostFactory_ABC
*/
// Created: ABR 2011-10-14
// =============================================================================
class GhostFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             GhostFactory_ABC() {};
    virtual ~GhostFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual kernel::Ghost_ABC* Create( kernel::Entity_ABC& parent, const kernel::GhostPrototype& prototype, const geometry::Point2f& position ) = 0;
    virtual kernel::Ghost_ABC* Create( kernel::Entity_ABC& parent, xml::xistream& xis ) = 0;
    //@}
};

#endif // __GhostFactory_ABC_h_
