// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PolyEllipticalArc_h_
#define __PolyEllipticalArc_h_

#include "PathCommand_ABC.h"

namespace svg
{
class PathParser;

namespace path
{
    class EllipticalArc;

// =============================================================================
/** @class  PolyEllipticalArc
    @brief  PolyEllipticalArc
*/
// Created: AGE 2006-08-10
// =============================================================================
class PolyEllipticalArc : public PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PolyEllipticalArc( PathParser& data, PathContext& context );
    virtual ~PolyEllipticalArc();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine( float expectedPrecision );
    virtual void DrawArea( Tesselator_ABC& tesselator, float expectedPrecision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PolyEllipticalArc( const PolyEllipticalArc& );            //!< Copy constructor
    PolyEllipticalArc& operator=( const PolyEllipticalArc& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< EllipticalArc* >  T_Elements;
    typedef T_Elements::const_iterator   CIT_Elements;
    //@}

private:
    //! @name Member data
    //@{
    T_Elements elements_;
    //@}
};

}
}

#endif // __PolyEllipticalArc_h_
