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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ShapeEditorMapEventFilter.h $
// $Author: Ape $
// $Modtime: 5/10/04 15:58 $
// $Revision: 2 $
// $Workfile: MOS_ShapeEditorMapEventFilter.h $
//
// *****************************************************************************

#ifndef __MOS_ShapeEditorMapEventFilter_h_
#define __MOS_ShapeEditorMapEventFilter_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_MapEventFilter_ABC.h"
#include "MOS_Types.h"
#include "MOS_ASN_Types.h"
class MOS_MapMouseEvent;


// =============================================================================
/** @class  MOS_ShapeEditorMapEventFilter
    @brief  MOS_ShapeEditorMapEventFilter
    @par    Using example
    @code
    MOS_ShapeEditorMapEventFilter;
    @endcode
*/
// Created: APE 2004-04-14
// =============================================================================
class MOS_ShapeEditorMapEventFilter : public QObject, public MOS_MapEventFilter_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ShapeEditorMapEventFilter );
    friend class MOS_GLTool;

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
    MOS_ShapeEditorMapEventFilter( QObject* pParent );
    ~MOS_ShapeEditorMapEventFilter();
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

    bool OnMousePress     ( const MOS_MapMouseEvent& mouseEvent );
    bool OnMouseDblClick  ( const MOS_MapMouseEvent& mouseEvent );
    bool OnKeyPress       ( const QKeyEvent& keyEvent );
    //@}

signals:
    void Done();

private:
    T_PointVector pointList_;
    E_LineType nLineType_;
};

#   include "MOS_ShapeEditorMapEventFilter.inl"

#endif // __MOS_ShapeEditorMapEventFilter_h_
