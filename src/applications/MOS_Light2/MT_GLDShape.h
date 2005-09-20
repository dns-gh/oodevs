// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShape.h $
// $Author: Ape $
// $Modtime: 8/07/04 10:19 $
// $Revision: 1 $
// $Workfile: MT_GLDShape.h $
//
// *****************************************************************************

#ifndef __MT_GLDShape_h_
#define __MT_GLDShape_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_GLDShape_ABC.h"
#include "MT_GLDShapeProperties.h"

class MT_GLDrawer;


// =============================================================================
/** @class  MT_GLDShape
    @brief  MT_GLDShape
    @par    Using example
    @code
    MT_GLDShape;
    @endcode
*/
// Created: APE 2004-06-24
// =============================================================================
class MT_GLDShape : public MT_GLDShape_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MT_GLDShape );

public:
    enum E_ShapeType
    {
        ePoint = 0,
        eLine,
        eRectangle,
        ePolygon,
        eCircle,
        eText,
        eImage,
        eNbrShapes
    };

public:
    //! @name Constructors/Destructor
    //@{
     MT_GLDShape( MT_GLDrawer& glDrawer, E_ShapeType nType );
    ~MT_GLDShape();
    //@}

    //! @name Operations
    //@{
    void Draw();
    bool IsAtPos( const MT_Vector2D& vPos );

    void Translate( const MT_Vector2D& vPos );
    MT_Vector2D* GetPointAt( const MT_Vector2D& vPos );

    void StartCreation( const MT_Vector2D* pFirstPoint = 0 );
    void StopCreation( bool bOk );
    //@}

    //! @name Accessors
    //@{
    const MT_GLDShapeProperties& GetProperties() const;
    //@}

    //! @name Modifiers
    //@{
    void SetProperties( const MT_GLDShapeProperties& properties );
    //@}

    void Read( MT_XXmlInputArchive& archive );
    void Write( MT_XXmlOutputArchive& archive ) const;

signals:
    //! @name Signals
    //@{
    void CreationDone();
    void CreationCanceled();
    //@}

private:
    //! @name Helpers
    //@{
    bool eventFilter( QObject* pObject, QEvent* pEvent );

    bool GetImageFileName();
    bool InitImage();
    MT_Vector2D DrawArrowhead( const MT_Vector2D& src, const MT_Vector2D& dest, MT_GLDShapeProperties::E_ArrowHead nArrowType );
    void SetColor( QRgb color ) const;
    //@}

private:
    //! @name Member data
    //@{
    E_ShapeType nType_;

    MT_GLDShapeProperties properties_;

    T_PointVector points_;
    MT_Vector2D* pMousePoint_;

    // Used for eImage
    GLuint nGLDisplayList_;
    std::string strFileName_;
    //@}
};

#   include "MT_GLDShape.inl"

#endif // __MT_GLDShape_h_
