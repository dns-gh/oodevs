// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HorizontalTo_h_
#define __HorizontalTo_h_

#include "PathCommand_ABC.h"

namespace svg 
{
class PathParser;

namespace path
{

// =============================================================================
/** @class  HorizontalTo
    @brief  HorizontalTo
*/
// Created: AGE 2006-08-10
// =============================================================================
class HorizontalTo : public PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HorizontalTo( PathParser& data, PathContext& context );
    virtual ~HorizontalTo();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine( float expectedPrecision );
    virtual void DrawArea( Tesselator_ABC& tesselator, float expectedPrecision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HorizontalTo( const HorizontalTo& );            //!< Copy constructor
    HorizontalTo& operator=( const HorizontalTo& ); //!< Assignement operator
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

#endif // __HorizontalTo_h_
