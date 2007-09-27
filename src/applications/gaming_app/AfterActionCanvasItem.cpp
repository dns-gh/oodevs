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
#include "AfterActionCanvas.h"
#include "gaming/AfterActionItem_ABC.h"
#include <boost/bind.hpp>
#include <qpainter.h>
#include <numeric>
#include <boost/lexical_cast.hpp>

#pragma warning (disable : 4018)

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem constructor
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasItem::AfterActionCanvasItem( AfterActionCanvas& canvas, AfterActionItem_ABC& item, const QPoint& pos )
    : QCanvasRectangle( QRect( pos, QSize( 80, 30 ) ), canvas.canvas() )
    , canvas_         ( canvas )
    , item_           ( item )
    , output_         ( 0 )
{
    item_.Build( *this );
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
    for( IT_Items it = subItems_.begin(); it != subItems_.end(); ++it )
        delete *it;

    T_Connections toDelete( connections_ );
    for( IT_Connections it = toDelete.begin(); it != toDelete.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::StartConnection
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasConnection* AfterActionCanvasItem::StartConnection( const QPoint& point )
{
    if( IsOnOutput( point ) )
        return new AfterActionCanvasConnection( canvas_.palette(), this, x() + 84, y() + 15 );
    for( int i = 0; i < inputs_.size(); ++i )
        if( IsOnInput( i, point ) && item_.CanConnect( i ) )
            return new AfterActionCanvasConnection( canvas_.palette(), this, i, x() - 4, y() + InputPosition( i ) );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::EndConnection
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
bool AfterActionCanvasItem::EndConnection( AfterActionCanvasConnection& connection, const QPoint& point )
{
    if( IsOnOutput( point ) )
        return Connect( connection, this, connection.To(), connection.ToIndex() );

    for( int i = 0; i < inputs_.size(); ++i )
        if( IsOnInput( i, point ) )
            return Connect( connection, connection.From(), this, i );

    return false;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::Remove
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::Remove( AfterActionCanvasConnection* connection )
{
    IT_Connections it  = std::find( connections_.begin(), connections_.end(), connection );
    if( it != connections_.end() )
    {
        if( connection->From() == this && connection->To() )
            item_.Disconnect( &connection->To()->item_ );
        else if( connection->To() == this && connection->From() )
            item_.Disconnect( & connection->From()->item_, connection->ToIndex() );
        std::swap( *it, connections_.back() );
        connections_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::Connect
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
bool AfterActionCanvasItem::Connect( AfterActionCanvasConnection& connection, AfterActionCanvasItem* from, AfterActionCanvasItem* to, int i, bool alreadyConnected /*= false*/ )
{
    if( from && to && ( alreadyConnected || to->item_.CanConnect( i, & from->item_ ) ) )
    {
        if( ! alreadyConnected )
            to->item_.Connect( i, from->item_ );
        from->connections_.push_back( &connection );
        to  ->connections_.push_back( &connection );
        connection.Close( from, to, i, from->x() + 84, from->y() + 15,
                                         to->x() -  4, to->y() + to->InputPosition( i ) );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::IsOnOutput
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
bool AfterActionCanvasItem::IsOnOutput( const QPoint& point ) const
{
    return output_ && output_->boundingRect().contains( point );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::IsOnInput
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
bool AfterActionCanvasItem::IsOnInput( int i, const QPoint& point ) const
{
    return i >= 0 && i < inputs_.size() && inputs_[i]->boundingRect().contains( point );
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
    p.setPen( canvas_.palette().color( colorGroup, QColorGroup::Foreground ) );
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
// Name: AfterActionCanvasItem::SpreadInputs
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::SpreadInputs()
{
    for( int i = 0; i < inputs_.size(); ++i )
        inputs_[i]->move( x() - 4, y() + InputPosition( i ) );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::InputPosition
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
double AfterActionCanvasItem::InputPosition( int i ) const
{
    return 30. * ( 0.5 + i ) / inputs_.size();
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
    SpreadInputs();
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
    output_ = circle;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::AddParameter
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::AddParameter( const std::string& type, const std::string& name )
{
    // $$$$ AGE 2007-09-27: TODO
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::Connect
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::Connect( AfterActionItem_ABC* target, int targetSlot )
{
    // $$$$ AGE 2007-09-27: ...
    if( ! isEnabled() )
        return;
    AfterActionCanvasConnection* connection = new AfterActionCanvasConnection( canvas_.palette(), this, x() + 84, y() + 15 );
    Connect( *connection, this, canvas_.Resolve( target ), targetSlot, true );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::Holds
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
bool AfterActionCanvasItem::Holds( AfterActionItem_ABC* item ) const
{
    return item == &item_;
}

namespace
{
    struct Connector : public AfterActionBuilder_ABC
    {
        Connector( AfterActionBuilder_ABC& next ) : next_( &next ) {}
        virtual void Start( const std::string& ){};
        virtual void AddInput( const std::string& ){};
        virtual void AddOutput( const std::string& ){};
        virtual void AddParameter( const std::string& , const std::string& ){};
        virtual void Connect( AfterActionItem_ABC* target, int targetSlot )
        {
            next_->Connect( target, targetSlot );
        }
        AfterActionBuilder_ABC* next_;
    };
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasItem::Reconnect
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void AfterActionCanvasItem::Reconnect()
{
    Connector connector( *this );
    item_.Build( connector );
}
