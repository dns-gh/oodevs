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

class ElevationLayer;
class ModelLoaded;

namespace kernel
{
    class DetectionMap;
    class Controller;
}

namespace gui
{

// =============================================================================
/** @class  Elevation2dLayer
    @brief  Elevation2dLayer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Elevation2dLayer : public Layer2d_ABC
                       , public kernel::Observer_ABC
                       , public kernel::ElementObserver_ABC< ModelLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             Elevation2dLayer( kernel::Controller& controller, const kernel::DetectionMap& elevation );
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
    kernel::Controller& controller_;
    const kernel::DetectionMap& elevation_;
    ElevationLayer* layer_;
    bool modelLoaded_;
    //@}
};

}

#endif // __Elevation2dLayer_h_
