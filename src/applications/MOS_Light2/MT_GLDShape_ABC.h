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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShape_ABC.h $
// $Author: Ape $
// $Modtime: 8/07/04 16:09 $
// $Revision: 1 $
// $Workfile: MT_GLDShape_ABC.h $
//
// *****************************************************************************

#ifndef __MT_GLDShape_ABC_h_
#define __MT_GLDShape_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"

class MT_GLDShapeProperties;
class MT_GLDrawer;


// =============================================================================
/** @class  MT_GLDShape_ABC
    @brief  MT_GLDShape_ABC
    @par    Using example
    @code
    MT_GLDShape_ABC;
    @endcode
*/
// Created: APE 2004-06-24
// =============================================================================
class MT_GLDShape_ABC : public QObject
{
    MT_COPYNOTALLOWED( MT_GLDShape_ABC );

public:
    enum E_State
    {
        eNormal,
        eCreation,
        eSelected
    };

    typedef std::vector< MT_GLDShape_ABC* >  T_ShapeVector;
    typedef T_ShapeVector::iterator          IT_ShapeVector;
    typedef T_ShapeVector::const_iterator    CIT_ShapeVector;

public:
    //! @name Constructors/Destructor
    //@{
     MT_GLDShape_ABC( MT_GLDrawer& glDrawer );
    ~MT_GLDShape_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Draw() = 0;
    virtual bool IsAtPos( const MT_Vector2D& vPos ) = 0;

    virtual void Translate( const MT_Vector2D& vPos ) = 0;
    virtual MT_Vector2D* GetPointAt( const MT_Vector2D& vPos ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const MT_GLDShapeProperties& GetProperties() const = 0;
    //@}

    //! @name Modifiers
    //@{
    virtual void SetProperties( const MT_GLDShapeProperties& properties ) = 0;
    virtual void SetState( E_State nState );
    //@}

    //! @name For groups //$$$$ Ugly.
    //@{
    virtual const MT_GLDShape_ABC::T_ShapeVector GetShapes() const;
    virtual void ClearShapes();
    //@}


    virtual void Read( MT_XXmlInputArchive& archive ) = 0;
    virtual void Write( MT_XXmlOutputArchive& archive ) const = 0;

public:
    static MT_GLDShape_ABC* ReadNewShape( MT_XXmlInputArchive& archive, MT_GLDrawer& glDrawer );

protected:
    MT_GLDrawer& glDrawer_;
    E_State nState_;
};


#ifdef MT_USE_INLINE
#   include "MT_GLDShape_ABC.inl"
#endif

#endif // __MT_GLDShape_ABC_h_
