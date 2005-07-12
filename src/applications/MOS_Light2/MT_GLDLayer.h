// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDLayer.h $
// $Author: Ape $
// $Modtime: 8/07/04 15:59 $
// $Revision: 1 $
// $Workfile: MT_GLDLayer.h $
//
// *****************************************************************************

#ifndef __MT_GLDLayer_h_
#define __MT_GLDLayer_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_GLDShape_ABC.h"

class MT_GLDrawer;


// =============================================================================
/** @class  MT_GLDLayer
    @brief  MT_GLDLayer
    @par    Using example
    @code
    MT_GLDLayer;
    @endcode
*/
// Created: APE 2004-06-23
// =============================================================================
class MT_GLDLayer
{
    MT_COPYNOTALLOWED( MT_GLDLayer );

public:
    //! @name Constructors/Destructor
    //@{
     MT_GLDLayer( const QString& strName, MT_GLDrawer& glDrawer );
    ~MT_GLDLayer();
    //@}

    //! @name Operations
    //@{
    void Draw();

    void AddShape( MT_GLDShape_ABC& shape );
    void AddShapes( const MT_GLDShape_ABC::T_ShapeVector& shapes );
    void RemoveShapes( const MT_GLDShape_ABC::T_ShapeVector& shapes );
    void DeleteShapes( const MT_GLDShape_ABC::T_ShapeVector& shapes );

    MT_GLDShape_ABC* GetShapeAtPos( const MT_Vector2D& vPos );

    MT_GLDShape_ABC& Group( MT_GLDShape_ABC::T_ShapeVector& shapes );
    MT_GLDShape_ABC::T_ShapeVector Disgroup( MT_GLDShape_ABC& shape );
    //@}

    //! @name Accessors
    //@{
    const QString& GetName() const;
    float GetOpacity() const;
    bool IsVisible() const;
    //@}

    //! @name Modifiers
    //@{
    void SetName( const QString& strName );
    void SetOpacity( float rOpacity );
    void SetVisible( bool bVisible );
    //@}

    //! @name In/Out
    //@{
    void Read( MT_XXmlInputArchive& archive );
    void Write( MT_XXmlOutputArchive& archive ) const;
    //@}

private:
    //! @name Member data
    //@{
    MT_GLDrawer& glDrawer_;
    QString strName_;
    float rOpacity_;
    bool bVisible_;

    MT_GLDShape_ABC::T_ShapeVector shapes_;
    //@}
};


#ifdef MT_USE_INLINE
#   include "MT_GLDLayer.inl"
#endif

#endif // __MT_GLDLayer_h_
