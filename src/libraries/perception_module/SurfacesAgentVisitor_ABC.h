// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_SURFACES_AGENT_VISITOR_ABC_H
#define SWORD_PERCEPTION_SURFACES_AGENT_VISITOR_ABC_H

#include <boost/noncopyable.hpp>

namespace sword
{
namespace perception
{
    class PerceptionSurfaceAgent;

// =============================================================================
/** @class  SurfacesAgentVisitor_ABC
    @brief  Surfaces agent visitor definition
*/
// Created: SLI 2012-05-25
// =============================================================================
class SurfacesAgentVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SurfacesAgentVisitor_ABC() {}
    virtual ~SurfacesAgentVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Notify( const PerceptionSurfaceAgent& surface ) = 0;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_SURFACES_AGENT_VISITOR_ABC_H
