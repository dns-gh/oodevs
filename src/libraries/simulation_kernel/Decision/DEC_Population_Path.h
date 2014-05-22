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
#include "MT_Tools/MT_Profiler.h"
#include <boost/shared_ptr.hpp>

class MIL_Population;
class DEC_PopulationContext_ABC;
class DEC_PathComputer_ABC;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Population_Path : public DEC_PathResult
{
public:
             DEC_Population_Path( const MIL_Population& population, const T_PointVector& points,
                const boost::shared_ptr< DEC_PathComputer_ABC >& computer );
    virtual ~DEC_Population_Path();

private:
    DEC_Population_Path( const DEC_Population_Path& rhs ); // Copy only query parameters, not the result !

    virtual void Destroy();
    virtual void Cancel();
    virtual void Finalize();
    virtual E_State GetState() const;
    virtual double GetLength() const;
    virtual const MT_Vector2D& GetLastWaypoint() const;

private:
    //! @name Member data
    //@{
    MT_Profiler profiler_;
    boost::shared_ptr< DEC_PopulationContext_ABC > context_;
    boost::shared_ptr< DEC_PathComputer_ABC > computer_;
    //@}
};

#endif // __DEC_Population_Path_h_
