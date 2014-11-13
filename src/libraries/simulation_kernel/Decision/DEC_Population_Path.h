// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Population_Path_h_
#define __DEC_Population_Path_h_

#include "Decision/DEC_PathResult.h"
#include <boost/shared_ptr.hpp>

class MIL_Population;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Population_Path : public DEC_PathResult
{
public:
             DEC_Population_Path( const MIL_Population& population, const T_PointVector& points );
    virtual ~DEC_Population_Path();

private:
    DEC_Population_Path( const DEC_Population_Path& rhs ); // Copy only query parameters, not the result !
};

#endif // __DEC_Population_Path_h_
