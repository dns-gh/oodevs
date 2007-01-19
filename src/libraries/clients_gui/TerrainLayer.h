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
#include "graphics/GraphicManager_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/WorldParameters.h"
#include "pathfind/TerrainData.h"
#include "pathfind/SpatialContainer.h"
#include "terrain/TesselatedShape.h"

class GraphicShape;
class GraphicSetup_ABC;

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class ModelLoaded;
}

namespace gui
{

// =============================================================================
/** @class  TerrainLayer
    @brief  TerrainLayer
*/
// Created: AGE 2006-03-15
// =============================================================================
class TerrainLayer : public Layer2d_ABC
                   , private GraphicManager_ABC
                   , public kernel::Observer_ABC
                   , public kernel::OptionsObserver_ABC
                   , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             TerrainLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, GraphicSetup_ABC& setup );
    virtual ~TerrainLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void Reset();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TerrainLayer( const TerrainLayer& );            //!< Copy constructor
    TerrainLayer& operator=( const TerrainLayer& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    struct ShapeTraits {
        int CompareOnX( float rValue, TesselatedShape*const& shape ) const;
        int CompareOnY( float rValue, TesselatedShape*const& shape ) const;
    };
    typedef pathfind::SpatialContainer< TesselatedShape*, ShapeTraits >   T_ShapeContainer;
    typedef std::map< TerrainData, T_ShapeContainer* >                    T_Shapes;
    typedef T_Shapes::const_iterator                                    CIT_Shapes;
    //@}

    //! @name Helpers
    //@{
    virtual void AddShape( GraphicShape& shape );
    virtual void AddShape( TesselatedShape& shape );
    virtual bool ShouldLoad( const std::string& filename );
    void Purge();

    void DrawInnerShapes  ( const geometry::Rectangle2f& viewport ) const;
    void DrawShapesBorders( const geometry::Rectangle2f& viewport ) const;
    void DrawLinearShapes ( const geometry::Rectangle2f& viewport ) const;
    void DrawNames        ( const geometry::Rectangle2f& viewport ) const;

    virtual void SetupLineGraphics  ( const TerrainData& d ) const;
    virtual void SetupBorderGraphics( const TerrainData& d ) const;
    virtual void SetupAreaGraphics  ( const TerrainData& d ) const;

    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );

    template< typename Functor >
    void Apply( const T_ShapeContainer& container, const geometry::Rectangle2f& viewport, const Functor& functor ) const;

    bool ShouldDisplayArea( const TerrainData& data, const geometry::Rectangle2f& viewport ) const;
    bool ShouldDisplayBorder( const TerrainData& data, const geometry::Rectangle2f& viewport ) const;
    bool ShouldDisplayLinear( const TerrainData& data, const geometry::Rectangle2f& viewport ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    GraphicSetup_ABC& setup_;

    const std::string dataDirectory_;
    geometry::Rectangle2f world_;
    T_Shapes shapes_;
    kernel::TristateOption smallNames_;
    kernel::TristateOption bigNames_;

    kernel::WorldParameters parameters_;
    //@}
};

}

#endif // __TerrainLayer_h_
