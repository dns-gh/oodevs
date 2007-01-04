// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RasterLayer_h_
#define __RasterLayer_h_

#include "Layer_ABC.h"

class TextureSet;

namespace gui
{

// =============================================================================
/** @class  RasterLayer
    @brief  RasterLayer
*/
// Created: AGE 2007-01-04
// =============================================================================
class RasterLayer : public Layer2d_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RasterLayer( TextureSet& raster );
    virtual ~RasterLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RasterLayer( const RasterLayer& );            //!< Copy constructor
    RasterLayer& operator=( const RasterLayer& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    TextureSet& raster_;
    //@}
};

}

#endif // __RasterLayer_h_
