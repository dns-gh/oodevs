// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Elevation2dLayer_h_
#define __Elevation2dLayer_h_

#include "Layer_ABC.h"

class ElevationMap;
class ElevationLayer;

// =============================================================================
/** @class  Elevation2dLayer
    @brief  Elevation2dLayer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Elevation2dLayer : public Layer2d_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Elevation2dLayer( const ElevationMap& elevation );
    virtual ~Elevation2dLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Elevation2dLayer( const Elevation2dLayer& );            //!< Copy constructor
    Elevation2dLayer& operator=( const Elevation2dLayer& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const ElevationMap& elevation_;
    ElevationLayer* layer_;
    //@}
};

#endif // __Elevation2dLayer_h_
