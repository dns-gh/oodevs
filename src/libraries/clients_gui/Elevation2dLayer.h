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
#include "clients_kernel/ElementObserver_ABC.h"

class ElevationTextureSet;
class ElevationShader;

namespace kernel
{
    class DetectionMap;
    class Controller;
    class ModelLoaded;
}

namespace gui
{
    class ElevationExtrema;

// =============================================================================
/** @class  Elevation2dLayer
    @brief  Elevation2dLayer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Elevation2dLayer : public Layer2d_ABC
                       , public kernel::Observer_ABC
                       , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
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
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );

    void SetColors( const QColor& min, const QColor& max );
    void SetElevations( unsigned short min, unsigned short max );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Elevation2dLayer( const Elevation2dLayer& );            //!< Copy constructor
    Elevation2dLayer& operator=( const Elevation2dLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void SetGradient();
    void SetShader();
    void Cleanup();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::DetectionMap& elevation_;
    std::auto_ptr< ElevationExtrema >    extrema_;
    std::auto_ptr< ElevationShader >     shader_;
    std::auto_ptr< ElevationTextureSet > layer_;
    bool modelLoaded_;

    QColor min_;
    QColor max_;
    unsigned gradient_;
    //@}
};

}

#endif // __Elevation2dLayer_h_
