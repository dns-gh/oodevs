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

#include "Layer_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "tools/WorldParameters.h"
#include <spatialcontainer/TerrainData.h>
#include <graphics/GraphicSetup_ABC.h>

class RawShapeLayer;
class NoVBOShapeLayer;

namespace kernel
{
    class Controllers;
    class ModelLoaded;
}

namespace
{
    class ContourGraphicSetup : public GraphicSetup_ABC
    {
    public:
    //! @name Constructors/Destructor
    //@{
    explicit ContourGraphicSetup(){}
    virtual ~ContourGraphicSetup(){}
    //@}

    virtual void SetupLineGraphics  ( const Data_ABC* )
    {
        glLineWidth( 1. );
        const float color[4] = { r_, g_, b_, alpha_ };
        glColor4fv( color );
    }
    virtual void SetupLineGraphics  ( unsigned int )
    {}
    virtual void SetupBorderGraphics( const Data_ABC* )
    {}
    virtual void SetupAreaGraphics  ( const Data_ABC* )
    {}

    void SetColor( float r, float g, float b )
    {
        r_ = r;
        g_ = g;
        b_ = b;
    }
    //@}

    private:
        //! @name Copy/Assignment
        //@{
        ContourGraphicSetup( const ContourGraphicSetup& );            //!< Copy constructor
        ContourGraphicSetup& operator=( const ContourGraphicSetup& ); //!< Assignment operator
        //@}

    public:
        //! @name Member data
        //@{
        float r_, g_, b_, alpha_;
        //@}
    };
}

namespace gui
{

// =============================================================================
/** @class  ContourLinesLayer
    @brief  ContourLinesLayer
*/
// Created: SBO 2010-03-23
// =============================================================================
class ContourLinesLayer : public Layer2d_ABC
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                     , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ContourLinesLayer( kernel::Controllers& controllers );
    virtual ~ContourLinesLayer();
    //@}

    //! @name Operations
    //@{
    virtual void SetAlpha( float alpha );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void Reset();
    //@}

    //! @name Accessors
    //@{
    //@}

private:
    //! @name Helpers
    //@{
    void LoadGraphics();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    tools::WorldParameters parameters_;
    bool reset_;
    bool modelLoaded_;
    bool enabled_;
    QColor color_;

    std::auto_ptr< RawShapeLayer >        layer_;
    std::auto_ptr< NoVBOShapeLayer > noVBOlayer_;

    geometry::Rectangle2f world_;
    ContourGraphicSetup& setup_;

    //@}
};

}

#endif // __ContourLinesLayer_h_
