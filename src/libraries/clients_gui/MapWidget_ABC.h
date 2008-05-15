// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MapWidget_ABC_h_
#define __MapWidget_ABC_h_

namespace gui
{

// =============================================================================
/** @class  MapWidget_ABC
    @brief  Map Widget definition
*/
// Created: AGE 2008-05-14
// =============================================================================
class MapWidget_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MapWidget_ABC() {};
    virtual ~MapWidget_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void PaintLayers() = 0;
    virtual unsigned int Width() const = 0;
    virtual unsigned int Height() const = 0;
    virtual geometry::Rectangle2f Viewport() const = 0;
    //@}
};

}

#endif // __MapWidget_ABC_h_
