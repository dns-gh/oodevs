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
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/WorldParameters.h"

namespace kernel
{
    class Controller;
    class ModelLoaded;
}

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
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                  , private kernel::WorldParameters
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RasterLayer( kernel::Controller& controller );
    virtual ~RasterLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void Reset();
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
    kernel::Controller& controller_;
    std::auto_ptr< TextureSet > textures_;
    bool ignore_;
    //@}
};

}

#endif // __RasterLayer_h_
