// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef gui_ElevationPainter_h
#define gui_ElevationPainter_h

#include "Painter_ABC.h"

namespace kernel
{
    class DetectionMap;
}

namespace gui
{
// =============================================================================
/** @class  ElevationPainter
    @brief  Elevation painter
*/
// Created: LGY 2010-01-06
// =============================================================================
class ElevationPainter : public Painter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ElevationPainter( const kernel::DetectionMap& elevation );
    virtual ~ElevationPainter();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( QPainter& painter, unsigned int pourcentage, int x, int y ) const;
    //@}

private:
    //! @name Helpers
    //@{
    float Compute( unsigned int pourcentage ) const;
    //@}

private:
    //! @name Member data
    //@{
     const kernel::DetectionMap& elevation_;
    //@}
};
}

#endif // gui_ElevationPainter_h
