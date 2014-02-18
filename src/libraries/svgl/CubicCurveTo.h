// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CubicCurveTo_h_
#define __CubicCurveTo_h_

#include "PathCommand_ABC.h"

class BezierEvaluator;

namespace svg
{
class PathParser;

namespace path
{

// =============================================================================
/** @class  CubicCurveTo
    @brief  CubicCurveTo
*/
// Created: AGE 2006-08-10
// =============================================================================
class CubicCurveTo : public PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CubicCurveTo( PathParser& data, PathContext& context, bool shortHand );
    virtual ~CubicCurveTo();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine( float expectedPrecision );
    virtual void DrawArea( Tesselator_ABC& tesselator, float expectedPrecision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CubicCurveTo( const CubicCurveTo& );            //!< Copy constructor
    CubicCurveTo& operator=( const CubicCurveTo& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    // $$$$ AGE 2006-08-22: factoriser ces bidules
    unsigned ComputeSegmentNumber( float expectedPrecision ) const;
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point from_;
    float fromZ_;
    geometry::Point control1_;
    float c1Z_;
    geometry::Point control2_;
    float c2Z_;
    geometry::Point end_;
    float endZ_;
    float length_;
    //@}
};

}
}

#endif // __CubicCurveTo_h_
