// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __QuadraticCurveTo_h_
#define __QuadraticCurveTo_h_

#include "PathCommand_ABC.h"

class BezierEvaluator;

namespace svg
{
class PathParser;

namespace path
{

// =============================================================================
/** @class  QuadraticCurveTo
    @brief  QuadraticCurveTo
*/
// Created: AGE 2006-08-10
// =============================================================================
class QuadraticCurveTo : public PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             QuadraticCurveTo( PathParser& data, PathContext& context, bool shortHand );
    virtual ~QuadraticCurveTo();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine( float expectedPrecision );
    virtual void DrawArea( Tesselator_ABC& tesselator, float expectedPrecision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    QuadraticCurveTo( const QuadraticCurveTo& );            //!< Copy constructor
    QuadraticCurveTo& operator=( const QuadraticCurveTo& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    unsigned ComputeSegmentNumber( float expectedPrecision ) const;
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point from_;
    float fromZ_;
    geometry::Point control_;
    float cZ_;
    geometry::Point end_;
    float endZ_;
    float length_;
    //@}
};

}
}

#endif // __QuadraticCurveTo_h_
