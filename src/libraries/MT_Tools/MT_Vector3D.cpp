// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "MT_Vector3D.h"
#include <ostream>

std::ostream& operator<<( std::ostream& out, const MT_Vector3D& vect )
{
    return out << '[' << vect.rX_ << ", " << vect.rY_ << ", " << vect.rZ_ << ']';
}
