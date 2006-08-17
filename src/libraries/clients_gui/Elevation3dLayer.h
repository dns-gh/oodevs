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
#include "astec_kernel/Observer_ABC.h"
#include "astec_kernel/ElementObserver_ABC.h"

class DetectionMap;
class ElevationTextureTree;
class Controller;
class ModelLoaded;

// =============================================================================
/** @class  Elevation3dLayer
    @brief  Elevation3dLayer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Elevation3dLayer : public Layer3d_ABC
                       , private ElevationColor_ABC
                       , public Observer_ABC
                       , public ElementObserver_ABC< ModelLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             Elevation3dLayer( Controller& controller, const DetectionMap& elevation );
    virtual ~Elevation3dLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& frustum );
    virtual void NotifyUpdated( const ModelLoaded& modelLoaded );
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
    Controller& controller_;
    const DetectionMap& elevation_;
    ViewFrustum lastFrustum_;
    ElevationTextureTree* tree_;
    float zRatio_;
    bool modelLoaded_;
    //@}
};

#endif // __Elevation3dLayer_h_
