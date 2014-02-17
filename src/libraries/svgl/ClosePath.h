// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClosePath_h_
#define __ClosePath_h_

#include "PathCommand_ABC.h"

namespace svg 
{
class PathParser;

namespace path
{

// =============================================================================
/** @class  ClosePath
    @brief  ClosePath
*/
// Created: AGE 2006-08-10
// =============================================================================
class ClosePath : public PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ClosePath( PathParser& data, PathContext& context );
    virtual ~ClosePath();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine( float expectedPrecision );
    virtual void DrawArea( Tesselator_ABC& tesselator, float expectedPrecision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ClosePath( const ClosePath& );            //!< Copy constructor
    ClosePath& operator=( const ClosePath& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point currentPoint_;
    geometry::Point startPoint_;
    //@}
};

}
}

#endif // __ClosePath_h_
