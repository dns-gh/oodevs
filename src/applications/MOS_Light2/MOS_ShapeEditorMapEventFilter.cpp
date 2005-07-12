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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ShapeEditorMapEventFilter.cpp $
// $Author: Age $
// $Modtime: 19/05/05 15:07 $
// $Revision: 6 $
// $Workfile: MOS_ShapeEditorMapEventFilter.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_ShapeEditorMapEventFilter.h"
#include "moc_MOS_ShapeEditorMapEventFilter.cpp"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_GLTool.h"
#include "MOS_MapMouseEvent.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ShapeEditorMapEventFilter.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_ShapeEditorMapEventFilter constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
MOS_ShapeEditorMapEventFilter::MOS_ShapeEditorMapEventFilter( QObject* pParent )
    : QObject                ( pParent )
    , MOS_MapEventFilter_ABC ()
    , nLineType_             ( eTacticalLine )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ShapeEditorMapEventFilter destructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
MOS_ShapeEditorMapEventFilter::~MOS_ShapeEditorMapEventFilter()
{
    Pop();
}


// -----------------------------------------------------------------------------
// Name: MOS_ShapeEditorMapEventFilter::Draw
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
void MOS_ShapeEditorMapEventFilter::Draw() const
{
    MOS_GLTool::Draw( *this );
}


// -----------------------------------------------------------------------------
// Name: MOS_ShapeEditorMapEventFilter::eventFilter
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
bool MOS_ShapeEditorMapEventFilter::eventFilter( QObject* /*pSender*/, QEvent* pEvent )
{
    switch( pEvent->type() )
    {
        case QEvent::KeyPress:
            return OnKeyPress( (QKeyEvent&)*pEvent );

        // All QMouseEvent are actually MOS_MapMouseEvent. Cast and use them.
        case QEvent::MouseButtonPress:
            return OnMousePress( *(MOS_MapMouseEvent*)pEvent );

        case QEvent::MouseButtonDblClick:
            return OnMouseDblClick( *(MOS_MapMouseEvent*)pEvent );
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_ShapeEditorMapEventFilter::OnMousePress
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
bool MOS_ShapeEditorMapEventFilter::OnMousePress( const MOS_MapMouseEvent& mouseEvent )
{
    // Forbid points outside the world.
    const MT_Rect& rectWorld = MOS_App::GetApp().GetWorld().GetRect();
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
// Name: MOS_ShapeEditorMapEventFilter::OnMouseDblClick
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
bool MOS_ShapeEditorMapEventFilter::OnMouseDblClick( const MOS_MapMouseEvent& mouseEvent )
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
// Name: MOS_ShapeEditorMapEventFilter::OnKeyPress
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
bool MOS_ShapeEditorMapEventFilter::OnKeyPress( const QKeyEvent& keyEvent )
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
