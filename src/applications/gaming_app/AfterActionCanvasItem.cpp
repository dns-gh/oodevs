// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionCanvasItem.h"
#include "AfterActionCanvasConnection.h"
#include "gaming/AfterActionItem.h"
#include <boost/bind.hpp>
#include <qpainter.h>
#include <numeric>
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem constructor
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasItem::AfterActionCanvasItem( QCanvas* canvas, const QPalette& palette, const AfterActionItem& item, const QPoint& pos, unsigned id )
    : QCanvasRectangle( QRect( pos, QSize( 80, 30 ) ), canvas )
    , palette_        ( palette )
    , id_             ( id )
{
    item.Build( *this );
    setEnabled( true );
    show();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem destructor
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasItem::~AfterActionCanvasItem()
{
    hide();
    setCanvas( 0 );
    for( IT_Connections it = connections_.begin(); it != connections_.end(); ++it )
        (*it)->Disconnect( this );
    for( IT_Items it = subItems_.begin(); it != subItems_.end(); ++it )
        delete *it;
    for( IT_Connections it = connections_.begin(); it != connections_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::StartConnection
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasConnection* AfterActionCanvasItem::StartConnection( const QPoint& point )
{
    AfterActionCanvasConnection* result = 0;
    for( int i = 0; i < inputs_.size() && !result; ++i )
        result = StartConnection( *inputs_[i], point, -i-1 );
    for( int i = 0; i < outputs_.size() && !result; ++i )
        result = StartConnection( *outputs_[i], point, i+1 );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::StartConnection
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasConnection* AfterActionCanvasItem::StartConnection( const QCanvasItem& item, const QPoint& point, int index )
{
    const QRect rect = item.boundingRect();
    if( rect.contains( point ) )
    {
        connections_.push_back( new AfterActionCanvasConnection( palette_, this, index, rect.center() ) );
        return connections_.back();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::Remove
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::Remove( AfterActionCanvasConnection* connection )
{
    IT_Connections it = std::find( connections_.begin(), connections_.end(), connection );
    if( it != connections_.end() )
    {
        std::swap( *it, connections_.back() );
        connections_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::EndConnection
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
bool AfterActionCanvasItem::EndConnection( AfterActionCanvasConnection* connection, const QPoint& point )
{
    bool found = false;
    for( int i = 0; i < inputs_.size() && !found; ++i )
        found = IsFree( -i-1 ) && EndConnection( connection, *inputs_[i], point, -i-1 );
    for( int i = 0; i < outputs_.size() && !found; ++i )
        found = EndConnection( connection, *outputs_[i], point, i+1 );
    return found;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::EndConnection
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
bool AfterActionCanvasItem::EndConnection( AfterActionCanvasConnection* connection, const QCanvasItem& item, const QPoint& point, int index )
{
    const QRect rect = item.boundingRect();
    if( rect.contains( point ) && connection->Close( this, index, rect.center() ) )
    {
        connections_.push_back( connection );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::IsFree
// Created: AGE 2007-09-19
// -----------------------------------------------------------------------------
bool AfterActionCanvasItem::IsFree( int index )
{
    for( IT_Connections it = connections_.begin(); it != connections_.end(); ++it )
        if( (*it)->IsConnected( this, index ) )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::moveBy
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::moveBy( double dx, double dy )
{
    std::for_each( subItems_.begin(), subItems_.end(), boost::bind( &QCanvasItem::moveBy, _1, dx, dy ) );
    std::for_each( connections_.begin(), connections_.end(), boost::bind( &AfterActionCanvasConnection::MoveBy, _1, this, dx, dy ) );
    QCanvasRectangle::moveBy( dx, dy );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::setSelected
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::setSelected( bool yes )
{
    std::for_each( subItems_.begin(), subItems_.end(), boost::bind( &QCanvasItem::setSelected, _1, yes ) );
    QCanvasRectangle::setSelected( yes );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::setVisible
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::setVisible( bool yes )
{
    std::for_each( subItems_.begin(), subItems_.end(), boost::bind( &QCanvasItem::setVisible, _1, yes ) );
    QCanvasRectangle::setVisible( yes );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::boundingRect
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
QRect AfterActionCanvasItem::boundingRect() const
{
    return std::accumulate( subItems_.begin(), subItems_.end(),
                            QCanvasRectangle::boundingRect(),
                            boost::bind( QRect::unite, _1, boost::bind( &QCanvasItem::boundingRect, _2 ) ) );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::drawShape
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::drawShape( QPainter& p )
{
    const QPalette::ColorGroup colorGroup = isEnabled() ? ( isSelected() ? QPalette::Active : QPalette::Inactive ) : QPalette::Disabled;
    p.setPen( palette_.color( colorGroup, QColorGroup::Foreground ) );
    QCanvasRectangle::drawShape( p );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::Polish
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::Polish( QCanvasItem* subItem, double dx, double dy )
{
    subItem->move( x() + dx, y() + dy );
    subItem->show();
    subItems_.push_back( subItem );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::AdjustInputs
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::AdjustInputs()
{
    const double offset = 30. / double( inputs_.size() );
    double y = this->y() + offset / 2;
    for( unsigned i = 0; i < inputs_.size(); ++i )
    {
        inputs_[i]->move( x()-4, y );
        y += offset;
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::Start
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::Start( const std::string& name )
{
    QCanvasText* text = new QCanvasText( name.c_str(), canvas() );
    text->setTextFlags( Qt::AlignCenter );
    Polish( text, 40, 15 );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::AddInput
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::AddInput( const std::string& type )
{
    QCanvasEllipse* circle = new QCanvasEllipse( 8, 8, canvas() );
    circle->setBrush( black );
    Polish( circle, -4, 15 );
    inputs_.push_back( circle );
    AdjustInputs();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::AddOutput
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::AddOutput( const std::string& type )
{
    QCanvasEllipse* circle = new QCanvasEllipse( 8, 8, canvas() );
    circle->setBrush( black );
    Polish( circle, 84, 15 );
    outputs_.push_back( circle );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::AddParameter
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::AddParameter( const std::string& type, const std::string& name )
{

}
