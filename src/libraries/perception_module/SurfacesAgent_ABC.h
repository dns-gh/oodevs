// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_SURFACES_AGENT_ABC_H
#define SWORD_PERCEPTION_SURFACES_AGENT_ABC_H

#include <boost/noncopyable.hpp>

namespace sword
{
namespace perception
{
    class SurfacesAgentVisitor_ABC;

// =============================================================================
/** @class  SurfacesAgent_ABC
    @brief  Surfaces agent definition
*/
// Created: SLI 2012-05-25
// =============================================================================
class SurfacesAgent_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SurfacesAgent_ABC() {}
    virtual ~SurfacesAgent_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Apply( SurfacesAgentVisitor_ABC& visitor ) const = 0;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_SURFACES_AGENT_ABC_H
