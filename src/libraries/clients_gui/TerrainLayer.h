// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainLayer_h_
#define __TerrainLayer_h_

#include "Layer.h"
#include "clients_kernel/TristateOption.h"
#include <tools/ElementObserver_ABC.h>

class RawShapeLayer;
class NoVBOShapeLayer;
class TerrainData;

namespace kernel
{
    class Controllers;
    class ModelLoaded;
}

namespace gui
{
    class TerrainPicker;
    class TerrainSettings;

// =============================================================================
/** @class  TerrainLayer
    @brief  TerrainLayer
*/
// Created: AGE 2006-03-15
// =============================================================================
class TerrainLayer : public Layer2D
                   , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
             TerrainLayer( kernel::Controllers& controllers,
                           const std::shared_ptr< TerrainSettings >& settings,
                           GLView_ABC& tools,
                           TerrainPicker& picker );
    virtual ~TerrainLayer();
    //@}

    //! @name Operations
    //@{
    virtual void SetAlpha( float alpha );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void Reset();
    virtual bool HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point );
    TerrainData Pick( int x, int y );
    //@}

private:
    //! @name Helpers
    //@{
    void LoadGraphics();
    //@}

    //! @name Types
    //@{
    template< typename Base >
    class MyLayer;
    //@}

private:
    //! @name Member data
    //@{
    std::shared_ptr< TerrainSettings > settings_;
    TerrainPicker& picker_;
    bool pickingEnabled_;
    int minVisuScale_[ 13 ];
    int maxVisuScale_[ 13 ];

    geometry::Rectangle2f world_;
    kernel::TristateOption smallNames_;
    kernel::TristateOption bigNames_;
    tools::Path graphicsDirectory_;
    float width_;
    float height_;

    std::unique_ptr< RawShapeLayer > layer_;
    std::unique_ptr< NoVBOShapeLayer > noVBOlayer_;
    //@}
};

}

#endif // __TerrainLayer_h_
