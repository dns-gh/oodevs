// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LineTo_h_
#define __LineTo_h_

#include "PathCommand_ABC.h"

namespace svg 
{
class PathParser;

namespace path
{

// =============================================================================
/** @class  LineTo
    @brief  LineTo
*/
// Created: AGE 2006-08-10
// =============================================================================
class LineTo : public PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LineTo( PathParser& data, PathContext& context );
    virtual ~LineTo();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine( float expectedPrecision );
    virtual void DrawArea( Tesselator_ABC& tesselator, float expectedPrecision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LineTo( const LineTo& );            //!< Copy constructor
    LineTo& operator=( const LineTo& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< geometry::Point > T_Points;
    typedef T_Points::const_iterator       CIT_Points;
    //@}


private:
    //! @name Member data
    //@{
    geometry::Point from_;
    T_Points points_;
    //@}
};

}
}

#endif // __LineTo_h_
