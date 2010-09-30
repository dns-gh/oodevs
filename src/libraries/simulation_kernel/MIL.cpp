//*****************************************************************************
// Created: JVT 02-07-19
//*****************************************************************************


#include "simulation_kernel_pch.h"

std::ostream& operator << ( std::ostream& out, const MT_Vector3D& vect )
{
    return out << '[' << vect.rX_ << ", " << vect.rY_ << ", " << vect.rZ_ << ']';
}

std::ostream& operator << ( std::ostream& out, const MT_Vector2D& vect )
{
    return out << '[' << vect.rX_ << ", " << vect.rY_ << ']';
}
