// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_SURFACES_OBJECT_VISITOR_ABC_H
#define SWORD_PERCEPTION_SURFACES_OBJECT_VISITOR_ABC_H

#include <boost/noncopyable.hpp>

namespace sword
{
namespace perception
{
    class PerceptionSurfaceObject;

// =============================================================================
/** @class  SurfacesObjectVisitor_ABC
    @brief  Surfaces object visitor definition
*/
// Created: SLI 2012-05-25
// =============================================================================
class SurfacesObjectVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SurfacesObjectVisitor_ABC() {}
    virtual ~SurfacesObjectVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Notify( const PerceptionSurfaceObject& surface ) = 0;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_SURFACES_OBJECT_VISITOR_ABC_H
