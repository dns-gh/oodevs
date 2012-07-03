// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_SURFACES_OBJECT_ABC_H
#define SWORD_PERCEPTION_SURFACES_OBJECT_ABC_H

#include <boost/noncopyable.hpp>

namespace sword
{
namespace perception
{
    class SurfacesObjectVisitor_ABC;

// =============================================================================
/** @class  SurfacesObject_ABC
    @brief  Surfaces object definition
*/
// Created: SLI 2012-05-25
// =============================================================================
class SurfacesObject_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SurfacesObject_ABC() {}
    virtual ~SurfacesObject_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Apply( SurfacesObjectVisitor_ABC& visitor ) const = 0;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_SURFACES_OBJECT_ABC_H
