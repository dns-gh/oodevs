// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TooltipsLayer_ABC_h_
#define __TooltipsLayer_ABC_h_

#include "Layer_ABC.h"

class QImage;

namespace gui
{

// =============================================================================
/** @class  TooltipsLayer_ABC
    @brief  TooltipsLayer_ABC
*/
// Created: SBO 2008-04-11
// =============================================================================
class TooltipsLayer_ABC : public Layer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TooltipsLayer_ABC() {}
    virtual ~TooltipsLayer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& position, const QImage& image ) = 0;
    //@}
};

}

#endif // __TooltipsLayer_ABC_h_
