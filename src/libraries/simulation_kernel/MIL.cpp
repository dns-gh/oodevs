//*****************************************************************************
// Created: JVT 02-07-19
//*****************************************************************************


#include "simulation_kernel_pch.h"
#include "DIN/MessageService/DIN_BufferedMessage.h"
#include "DIN/DIN_Input.h"

std::ostream& operator << ( std::ostream& out, const MT_Vector3D& vect )
{
    return out << '[' << vect.rX_ << ", " << vect.rY_ << ", " << vect.rZ_ << ']';
}

std::ostream& operator << ( std::ostream& out, const MT_Vector2D& vect )
{
    return out << '[' << vect.rX_ << ", " << vect.rY_ << ']';
}

// -----------------------------------------------------------------------------
// Name: MIL::operator <<
// Created: AGE 2007-06-25
// -----------------------------------------------------------------------------
DIN::DIN_BufferedMessage& operator <<( DIN::DIN_BufferedMessage& msg, const MT_Vector2D& vPos )
{
    return msg << vPos.rX_ << vPos.rY_;
}

// -----------------------------------------------------------------------------
// Name: MIL::operator >>
// Created: AGE 2007-06-25
// -----------------------------------------------------------------------------
DIN::DIN_Input& operator >>( DIN::DIN_Input& msg, MT_Vector2D& vPos )
{
    return msg >> vPos.rX_ >> vPos.rY_;
}

