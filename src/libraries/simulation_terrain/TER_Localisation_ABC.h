// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef TER_LOCALISATION_ABC_H
#define TER_LOCALISATION_ABC_H

#include "MT_Tools/MT_Rect.h"
#include "MT_Tools/MT_Vector2D.h"

class TER_Localisation_ABC
{
public:
             TER_Localisation_ABC() {}
    virtual ~TER_Localisation_ABC() {}

    // Return the geometry bounding box.
    virtual MT_Rect GetBoundingBox() const = 0;

    // Return true if pos is contained in the geometry.
    virtual bool IsInside( const MT_Vector2D& pos ) const = 0;
};

#endif  // TER_LOCALISATION_ABC_H
