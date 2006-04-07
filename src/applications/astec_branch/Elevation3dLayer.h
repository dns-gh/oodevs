// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Elevation3dLayer_h_
#define __Elevation3dLayer_h_

#include "Layer_ABC.h"
#include "graphics/ElevationColor_ABC.h"
#include "graphics/ViewFrustum.h"

class ElevationMap;
class ElevationTextureTree;

// =============================================================================
/** @class  Elevation3dLayer
    @brief  Elevation3dLayer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Elevation3dLayer : public Layer3d_ABC, private ElevationColor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Elevation3dLayer( const ElevationMap& elevation );
    virtual ~Elevation3dLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void Paint( const ViewFrustum& frustum );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Elevation3dLayer( const Elevation3dLayer& );            //!< Copy constructor
    Elevation3dLayer& operator=( const Elevation3dLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void SelectColor( short elevation, float slope, short maxElevation, unsigned char* color );
    virtual bool HandleKeyPress( QKeyEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    ViewFrustum lastFrustum_;
    const ElevationMap& elevation_;
    ElevationTextureTree* tree_;
    float zRatio_;
    //@}
};

#endif // __Elevation3dLayer_h_
