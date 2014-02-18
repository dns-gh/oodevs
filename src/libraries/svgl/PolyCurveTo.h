// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PolyCurveTo_h_
#define __PolyCurveTo_h_

#include "PathCommand_ABC.h"

namespace svg
{
class PathParser;

namespace path
{
    class CurveTo;

// =============================================================================
/** @class  PolyCurveTo
    @brief  PolyCurveTo
*/
// Created: AGE 2006-08-10
// =============================================================================
class PolyCurveTo : public PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PolyCurveTo( PathParser& data, PathContext& context, char command );
    virtual ~PolyCurveTo();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine( float expectedPrecision );
    virtual void DrawArea( Tesselator_ABC& tesselator, float expectedPrecision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PolyCurveTo( const PolyCurveTo& );            //!< Copy constructor
    PolyCurveTo& operator=( const PolyCurveTo& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< PathCommand_ABC* >   T_Elements;
    typedef T_Elements::const_iterator      CIT_Elements;
    //@}

private:
    //! @name Member data
    //@{
    T_Elements elements_;
    //@}
};

}
}

#endif // __PolyCurveTo_h_
