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

#include "Layer_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/TristateOption.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/WorldParameters.h"

class RawShapeLayer;
class NoVBOShapeLayer;
class TerrainData;

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class ModelLoaded;
}

namespace gui
{
    class GraphicPreferences;
    class TerrainPicker;

// =============================================================================
/** @class  TerrainLayer
    @brief  TerrainLayer
*/
// Created: AGE 2006-03-15
// =============================================================================
class TerrainLayer : public Layer2d_ABC
                   , public kernel::OptionsObserver_ABC
                   , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
             TerrainLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, GraphicPreferences& setup, TerrainPicker& picker );
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
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
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
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    GraphicPreferences& setup_;
    TerrainPicker& picker_;
    bool pickingEnabled_;
    int minVisuScale_[ 13 ];
    int maxVisuScale_[ 13 ];

    geometry::Rectangle2f world_;
    kernel::TristateOption smallNames_;
    kernel::TristateOption bigNames_;
    tools::WorldParameters parameters_;

    std::auto_ptr< RawShapeLayer >        layer_;
    std::auto_ptr< NoVBOShapeLayer > noVBOlayer_;
    //@}
};

}

#endif // __TerrainLayer_h_
