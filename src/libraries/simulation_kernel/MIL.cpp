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


//-----------------------------------------------------------------------------
// Name: operator <<
// Created: NLD 2002-12-13
//-----------------------------------------------------------------------------
NEK::NEK_Output& operator << ( NEK::NEK_Output& msg, const MT_Vector2D& vPos )
{
    return msg << vPos.rX_ << vPos.rY_;
}

//-----------------------------------------------------------------------------
// Name: operator >>
// Created: NLD 2002-12-13
//-----------------------------------------------------------------------------
NEK::NEK_Input& operator >> ( NEK::NEK_Input& msg, MT_Vector2D& vPos )
{
    return msg >> vPos.rX_ >> vPos.rY_;
}
