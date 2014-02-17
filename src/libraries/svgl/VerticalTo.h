// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __VerticalTo_h_
#define __VerticalTo_h_

#include "PathCommand_ABC.h"

namespace svg 
{
class PathParser;

namespace path
{

// =============================================================================
/** @class  VerticalTo
    @brief  VerticalTo
*/
// Created: AGE 2006-08-10
// =============================================================================
class VerticalTo : public PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             VerticalTo( PathParser& data, PathContext& context );
    virtual ~VerticalTo();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine( float expectedPrecision );
    virtual void DrawArea( Tesselator_ABC& tesselator, float expectedPrecision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    VerticalTo( const VerticalTo& );            //!< Copy constructor
    VerticalTo& operator=( const VerticalTo& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point from_;
    geometry::Point to_;
    //@}
};

}
}

#endif // __VerticalTo_h_
