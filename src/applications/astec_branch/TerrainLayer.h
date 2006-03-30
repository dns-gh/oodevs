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
#include "graphics/GraphicSetup_ABC.h"
#include "OptionsObserver_ABC.h"
#include "TristateOption.h"

class GraphicShape;
class Controllers;

// =============================================================================
/** @class  TerrainLayer
    @brief  TerrainLayer
    // $$$$ AGE 2006-03-15: lod, spatial container
*/
// Created: AGE 2006-03-15
// =============================================================================
class TerrainLayer : public Layer2d_ABC
                   , private GraphicManager_ABC
                   , private GraphicSetup_ABC
                   , private Observer_ABC
                   , public OptionsObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TerrainLayer( Controllers& controllers, QGLWidget& widget, const std::string& dataDirectory );
    virtual ~TerrainLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TerrainLayer( const TerrainLayer& );            //!< Copy constructor
    TerrainLayer& operator=( const TerrainLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void AddShape( GraphicShape& shape );
    virtual bool ShouldLoad( const std::string& filename );

    void DrawInnerShapes  ( const geometry::Rectangle2f& viewport ) const;
    void DrawShapesBorders( const geometry::Rectangle2f& viewport ) const;
    void DrawLinearShapes ( const geometry::Rectangle2f& viewport ) const;
    void DrawNames        ( const geometry::Rectangle2f& viewport ) const;
    bool IsLinear         ( const GraphicShape& shape ) const;

    virtual void SetupLineGraphics  ( const Data_ABC* pData );
    virtual void SetupBorderGraphics( const Data_ABC* pData );
    virtual void SetupAreaGraphics  ( const Data_ABC* pData );

    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
    //@}

    //! @name Types
    //@{
    typedef std::vector< GraphicShape* > T_Shapes;
    typedef T_Shapes::const_iterator  CIT_Shapes;
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    QGLWidget& widget_;
    const std::string dataDirectory_;
    T_Shapes shapes_;
    TristateOption smallNames_;
    TristateOption bigNames_;
    //@}
};

#endif // __TerrainLayer_h_
