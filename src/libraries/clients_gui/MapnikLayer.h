// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef MapnikLayer_h
#define MapnikLayer_h

#include "Layer.h"
#include "tools/ElementObserver_ABC.h"
#include <tools/Path.h>
#include <boost/scoped_ptr.hpp>

namespace kernel
{
    class Controller;
    class ModelLoaded;
}

namespace graphics
{
    class MapnikLayer;
}

namespace gui
{
// =============================================================================
/** @class  MapnikLayer
    @brief  Mapnik layer
*/
// Created: MCO 2013-10-21
// =============================================================================
class MapnikLayer : public Layer2D
                  , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
public:
    explicit MapnikLayer( kernel::Controller& controller );
    virtual ~MapnikLayer();

    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void Reset();

private:
    void Initialize( double width, double height );
    void Generate();
    void Update( const geometry::Rectangle2f& viewport );

private:
    kernel::Controller& controller_;
    boost::scoped_ptr< graphics::MapnikLayer > layer_;
    tools::Path terrain_;
};

}

#endif // MapnikLayer_h
