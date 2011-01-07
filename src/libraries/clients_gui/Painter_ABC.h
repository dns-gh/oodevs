// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef gui_Painter_ABC_h
#define gui_Painter_ABC_h

#include <boost/noncopyable.hpp>

class QPainter;

namespace gui
{
// =============================================================================
/** @class  Painter_ABC
    @brief  Painter declaration
*/
// Created: LGY 2010-01-06
// =============================================================================
class Painter_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Painter_ABC() {}
    virtual ~Painter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Draw( QPainter& painter, unsigned int pourcentage, int x, int y ) const = 0;
    //@}
};

}

#endif // gui_Painter_ABC_h
