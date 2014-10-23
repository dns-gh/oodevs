// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "InformationToolTip.h"
#include "moc_InformationToolTip.cpp"
#include "Tooltip.h"

using namespace gui;

InformationToolTip::InformationToolTip()
    : QDialog( QApplication::activeWindow(), Qt::ToolTip )
{
    // NOTHING
}

InformationToolTip::~InformationToolTip()
{
    // NOTHING
}

void InformationToolTip::Draw()
{
    if( image_.isNull() )
        image_ = GetTooltip().GenerateImage();
    if( !image_.isNull() && !QApplication::activePopupWidget() )
    {
        move( ComputePosition() );
        setFixedSize( image_.width(), image_.height() );
        show();
        update();
    }
}

void InformationToolTip::Hide()
{
    hide();
    image_ = QImage();
}

void InformationToolTip::DirtyImage()
{
    image_ = QImage();
}

void InformationToolTip::paintEvent( QPaintEvent* /*event*/ )
{
    if( image_.isNull() || !QApplication::activeWindow() )
        return;
    QPainter p( this );
    p.drawImage( 0, 0, image_ );
}

QPoint InformationToolTip::ComputePosition() const
{
    QPoint pos = QCursor::pos() + QPoint( 0, 20 ); // mouse shape default size
    const QRect screen = QApplication::desktop()->screenGeometry( QApplication::desktop()->screenNumber( pos ) );
    if( screen.left() + screen.width() < pos.x() + image_.width() )
        pos.setX( pos.x() - image_.width() );
    if( screen.top() + screen.height() < pos.y() + image_.height() )
        pos.setY( pos.y() - image_.height() );
    return pos;
}
