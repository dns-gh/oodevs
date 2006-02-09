// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-14 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ShapeEditorMapEventFilter.h $
// $Author: Ape $
// $Modtime: 5/10/04 15:58 $
// $Revision: 2 $
// $Workfile: ShapeEditorMapEventFilter.h $
//
// *****************************************************************************

#ifndef __ShapeEditorMapEventFilter_h_
#define __ShapeEditorMapEventFilter_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MapEventFilter_ABC.h"
#include "Types.h"
#include "ASN_Types.h"
class MapMouseEvent;


// =============================================================================
/** @class  ShapeEditorMapEventFilter
    @brief  ShapeEditorMapEventFilter
    @par    Using example
    @code
    ShapeEditorMapEventFilter;
    @endcode
*/
// Created: APE 2004-04-14
// =============================================================================
class ShapeEditorMapEventFilter : public QObject, public MapEventFilter_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ShapeEditorMapEventFilter );
    friend class GLTool;

public:
    enum E_LineType
    {
        eCircle  = EnumTypeLocalisation::circle,
        eEllipse = EnumTypeLocalisation::ellipse,
        eLine    = EnumTypeLocalisation::line,
        ePolygon = EnumTypeLocalisation::polygon,
        ePoint   = EnumTypeLocalisation::point,
        eSector  = EnumTypeLocalisation::sector,
        ePath,
        eTacticalLine,
        eRectangle
    };
    
public:
    //! @name Constructors/Destructor
    //@{
    ShapeEditorMapEventFilter( QObject* pParent );
    ~ShapeEditorMapEventFilter();
    //@}

    //! @name Operations
    //@{
    void PrepareNewLine( E_LineType nType );
    void Draw() const;
    //@}

    //! @name Accessors
    //@{
    QObject* GetObject();
    T_PointVector& GetPointList();
    //@}

private:
    //! @name Event filter & related helpers
    //@{
    bool eventFilter( QObject* pSender, QEvent* pEvent );

    bool OnMousePress     ( const MapMouseEvent& mouseEvent );
    bool OnMouseDblClick  ( const MapMouseEvent& mouseEvent );
    bool OnKeyPress       ( const QKeyEvent& keyEvent );
    //@}

signals:
    void Done();

private:
    T_PointVector pointList_;
    E_LineType nLineType_;
};

#   include "ShapeEditorMapEventFilter.inl"

#endif // __ShapeEditorMapEventFilter_h_
