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

#include "graphics/MapLayer_ABC.h"
#include "graphics/GraphicManager_ABC.h"
#include "graphics/GraphicShapeProxy.h"

// =============================================================================
/** @class  TerrainLayer
    @brief  TerrainLayer
    // $$$$ AGE 2006-03-15: lod, spatial container
*/
// Created: AGE 2006-03-15
// =============================================================================
class TerrainLayer : public MapLayer_ABC, private GraphicManager_ABC
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
    virtual void AddShape( const GraphicShapeProxy& shape );
    virtual bool ShouldUseList( const std::string& filename );
    virtual bool ShouldLoad( const std::string& filename );

    void DrawInnerShapes  ( const geometry::Rectangle2f& viewport ) const;
    void DrawShapesBorders( const geometry::Rectangle2f& viewport ) const;
    void DrawLinearShapes ( const geometry::Rectangle2f& viewport ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< GraphicShapeProxy > T_Shapes;
    typedef T_Shapes::const_iterator       CIT_Shapes;
    //@}

private:
    //! @name Member data
    //@{
    const std::string dataDirectory_;
    T_Shapes shapes_;
    //@}
};

#endif // __TerrainLayer_h_
