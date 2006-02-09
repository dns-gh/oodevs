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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ShapeEditorMapEventFilter.cpp $
// $Author: Age $
// $Modtime: 19/05/05 15:07 $
// $Revision: 6 $
// $Workfile: ShapeEditorMapEventFilter.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ShapeEditorMapEventFilter.h"
#include "moc_ShapeEditorMapEventFilter.cpp"

#include "App.h"
#include "World.h"
#include "GLTool.h"
#include "MapMouseEvent.h"

// -----------------------------------------------------------------------------
// Name: ShapeEditorMapEventFilter constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
ShapeEditorMapEventFilter::ShapeEditorMapEventFilter( QObject* pParent )
    : QObject                ( pParent )
    , MapEventFilter_ABC ()
    , nLineType_             ( eTacticalLine )
{
}


// -----------------------------------------------------------------------------
// Name: ShapeEditorMapEventFilter destructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
ShapeEditorMapEventFilter::~ShapeEditorMapEventFilter()
{
    Pop();
}


// -----------------------------------------------------------------------------
// Name: ShapeEditorMapEventFilter::Draw
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
void ShapeEditorMapEventFilter::Draw() const
{
    GLTool::Draw( *this );
}


// -----------------------------------------------------------------------------
// Name: ShapeEditorMapEventFilter::eventFilter
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
bool ShapeEditorMapEventFilter::eventFilter( QObject* /*pSender*/, QEvent* pEvent )
{
    switch( pEvent->type() )
    {
        case QEvent::KeyPress:
            return OnKeyPress( (QKeyEvent&)*pEvent );

        // All QMouseEvent are actually MapMouseEvent. Cast and use them.
        case QEvent::MouseButtonPress:
            return OnMousePress( *(MapMouseEvent*)pEvent );

        case QEvent::MouseButtonDblClick:
            return OnMouseDblClick( *(MapMouseEvent*)pEvent );
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: ShapeEditorMapEventFilter::OnMousePress
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
bool ShapeEditorMapEventFilter::OnMousePress( const MapMouseEvent& mouseEvent )
{
    // Forbid points outside the world.
    const MT_Rect& rectWorld = App::GetApp().GetWorld().GetRect();
    if( ! rectWorld.IsInside( mouseEvent.GetMapPos() ) )
        return false;

    if( mouseEvent.button() == Qt::LeftButton && mouseEvent.state() == NoButton )
    {
        pointList_.push_back( mouseEvent.GetMapPos() );

        // If we reached the correct number of points for the current shape,
        // exit the editing process.
        if(    ( nLineType_ == eRectangle && pointList_.size() == 2 )
            || ( nLineType_ == eCircle && pointList_.size() == 2 )
            || ( nLineType_ == ePoint  && pointList_.size() == 1 ) )
        {
            emit Done();
            pointList_.clear();
            Pop();
        }
        return true;
    }

    if( mouseEvent.button() == Qt::RightButton && mouseEvent.state() == NoButton && ! pointList_.empty() )
    {
        pointList_.pop_back();
        return true;
    }

    return false;
}


// -----------------------------------------------------------------------------
// Name: ShapeEditorMapEventFilter::OnMouseDblClick
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
bool ShapeEditorMapEventFilter::OnMouseDblClick( const MapMouseEvent& mouseEvent )
{
    // Make sure that double clicking on the right button correctly deletes two points
    // (one for the click event, one for the double click event).
    if( mouseEvent.button() == Qt::RightButton && mouseEvent.state() == NoButton && ! pointList_.empty() )
    {
        pointList_.pop_back();
        return true;
    }

    emit Done();
    pointList_.clear();
    Pop();
    return true;
}


// -----------------------------------------------------------------------------
// Name: ShapeEditorMapEventFilter::OnKeyPress
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
bool ShapeEditorMapEventFilter::OnKeyPress( const QKeyEvent& keyEvent )
{
    switch( keyEvent.key() )
    {
        case Qt::Key_BackSpace:
        case Qt::Key_Delete:
        {
            if( ! pointList_.empty() )
                pointList_.pop_back();
            return true;
        }
        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            emit Done();
            pointList_.clear();
            Pop();
            return true;
        }
        case Qt::Key_Escape:
        {
            pointList_.clear();
            emit Done();
            Pop();
            return true;
        }
    }

    return false;
}
