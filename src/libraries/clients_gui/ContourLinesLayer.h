// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ContourLinesLayer_h_
#define __ContourLinesLayer_h_

#include "Layer.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/Types.h"

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace gui
{
    class ContourLinesComputer;
    class GLView_ABC;

// =============================================================================
/** @class  ContourLinesLayer
    @brief  ContourLinesLayer
*/
// Created: SBO 2010-03-23
// =============================================================================
class ContourLinesLayer : public Layer2D
                        , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                        , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
             ContourLinesLayer( kernel::Controllers& controllers,
                                GLView_ABC& view );
    virtual ~ContourLinesLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    bool modelLoaded_;
    //@}
};

} //! namespace gui

#endif // __ContourLinesLayer_h_
