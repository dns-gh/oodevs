// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EllipticalArc_h_
#define __EllipticalArc_h_

#include "PathCommand_ABC.h"

namespace svg 
{
class PathParser;

namespace path
{

// =============================================================================
/** @class  EllipticalArc
    @brief  EllipticalArc
*/
// Created: AGE 2006-08-10
// =============================================================================
class EllipticalArc : public PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EllipticalArc( PathParser& data, PathContext& context );
    virtual ~EllipticalArc();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine( float expectedPrecision );
    virtual void DrawArea( Tesselator_ABC& tesselator, float expectedPrecision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EllipticalArc( const EllipticalArc& );            //!< Copy constructor
    EllipticalArc& operator=( const EllipticalArc& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    unsigned ComputeSegmentNumber( float expectedPrecision ) const;
    void EndpointToCenter( const geometry::Point& from, const geometry::Point& to, bool largeArc, bool sweep );
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point radius_;
    float phy_;
    geometry::Point center_;
    float thetaMin_;
    float deltaTheta_;
    float length_;
    //@}
};

}
}

#endif // __EllipticalArc_h_
