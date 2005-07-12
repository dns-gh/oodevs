// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-07-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShapeGroup.h $
// $Author: Ape $
// $Modtime: 8/07/04 16:09 $
// $Revision: 1 $
// $Workfile: MT_GLDShapeGroup.h $
//
// *****************************************************************************

#ifndef __MT_GLDShapeGroup_h_
#define __MT_GLDShapeGroup_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_GLDShape_ABC.h"
#include "MT_GLDShapeProperties.h"


// =============================================================================
/** @class  MT_GLDShapeGroup
    @brief  MT_GLDShapeGroup
    @par    Using example
    @code
    MT_GLDShapeGroup;
    @endcode
*/
// Created: APE 2004-07-07
// =============================================================================
class MT_GLDShapeGroup : public MT_GLDShape_ABC
{
    MT_COPYNOTALLOWED( MT_GLDShapeGroup );

public:
    //! @name Constructors/Destructor
    //@{
     MT_GLDShapeGroup( MT_GLDrawer& glDrawer, const MT_GLDShape_ABC::T_ShapeVector& shapes = MT_GLDShape_ABC::T_ShapeVector() );
    ~MT_GLDShapeGroup();
    //@}

    //! @name Operations
    //@{
    void Draw();
    bool IsAtPos( const MT_Vector2D& vPos );

    void Translate( const MT_Vector2D& vPos );
    MT_Vector2D* GetPointAt( const MT_Vector2D& vPos );
    //@}

    //! @name Accessors
    //@{
    const MT_GLDShapeProperties& GetProperties() const;
    const MT_GLDShape_ABC::T_ShapeVector GetShapes() const;
    //@}

    //! @name Modifiers
    //@{
    void SetProperties( const MT_GLDShapeProperties& properties );
    void ClearShapes();
    void SetState( E_State nState );
    //}

    void Read( MT_XXmlInputArchive& archive );
    void Write( MT_XXmlOutputArchive& archive ) const;


private:
    //! @name Member data
    //@{
    MT_GLDShape_ABC::T_ShapeVector shapes_;
    mutable MT_GLDShapeProperties properties_;
    //@}
};


#ifdef MT_USE_INLINE
#   include "MT_GLDShapeGroup.inl"
#endif

#endif // __MT_GLDShapeGroup_h_
