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
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

class DetectionMap;
class ElevationLayer;
class Controller;
class ModelLoaded;

// =============================================================================
/** @class  Elevation2dLayer
    @brief  Elevation2dLayer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Elevation2dLayer : public Layer2d_ABC
                       , public Observer_ABC
                       , public ElementObserver_ABC< ModelLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             Elevation2dLayer( Controller& controller, const DetectionMap& elevation );
    virtual ~Elevation2dLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const ModelLoaded& modelLoaded );
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
    Controller& controller_;
    const DetectionMap& elevation_;
    ElevationLayer* layer_;
    bool modelLoaded_;
    //@}
};

#endif // __Elevation2dLayer_h_
