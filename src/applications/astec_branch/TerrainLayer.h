// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainLayer_h_
#define __TerrainLayer_h_

#include "Layer_ABC.h"
#include "graphics/GraphicManager_ABC.h"
#include "graphics/GraphicSetup_ABC.h"

class GraphicShape;

// =============================================================================
/** @class  TerrainLayer
    @brief  TerrainLayer
    // $$$$ AGE 2006-03-15: lod, spatial container
*/
// Created: AGE 2006-03-15
// =============================================================================
class TerrainLayer : public Layer2d_ABC, private GraphicManager_ABC, private GraphicSetup_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainLayer( const std::string& dataDirectory );
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

    virtual void SetupLineGraphics  ( const Data_ABC* pData );
    virtual void SetupBorderGraphics( const Data_ABC* pData );
    virtual void SetupAreaGraphics  ( const Data_ABC* pData );
    //@}

    //! @name Types
    //@{
    typedef std::vector< GraphicShape* > T_Shapes;
    typedef T_Shapes::const_iterator  CIT_Shapes;
    //@}

private:
    //! @name Member data
    //@{
    const std::string dataDirectory_;
    T_Shapes shapes_;
    //@}
};

#endif // __TerrainLayer_h_
