// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GradientButton.h"
#include "moc_GradientButton.cpp"

#include "GradientItem.h"
#include "Gradient.h"

using namespace gui;

namespace
{
    class GradientCanvas : public Q3Canvas
    {
    public:
        GradientCanvas( QWidget* parent, const std::vector< GradientItem* >& colors )
            : Q3Canvas( parent )
            , colors_      ( colors )
            , topMargin_   ( 5 )
            , bottomMargin_( 40 )
        {
            setBackgroundColor( parent->backgroundColor() );
            resize( 140, 50 );
            retune( 140, 1 );
        }
        virtual ~GradientCanvas()
        {
            // NOTHING
        }

        virtual void drawBackground( QPainter& painter, const QRect& )
        {
            if( colors_.size() < 2 )
                return;
            QRect area( rect() );
            Q3Canvas::drawBackground( painter, area );
            const float xStep = float( area.width() ) / 100;
            QRect current( area );
            current.setTop( current.top() + topMargin_ );
            current.setBottom( current.bottom() - bottomMargin_ );
            painter.drawLine( 0, current.bottom() + 5, current.right(), current.bottom() + 5 );
            for( unsigned int i = 0; i < colors_.size() - 1; ++i )
            {
                current.setLeft ( area.left() + int( colors_.at( i )->GetPercentage() * xStep ) );
                current.setRight( current.left() + int( ( colors_.at( i + 1 )->GetPercentage() - colors_.at( i )->GetPercentage() ) * xStep ) );
                DrawGradient( painter, current, i, i + 1 );
            }
        }

    private:
        void DrawGradient( QPainter& painter, const QRect& rect, int from, int to )
        {
            GradientItem* item1 = colors_.at( from );
            GradientItem* item2 = colors_.at( to );
            const int range = item2->GetPercentage() - item1->GetPercentage();
            if( range < 1 )
                return;
            const int rStep = ( item2->GetColor().red() - item1->GetColor().red() ) / range;
            const int gStep = ( item2->GetColor().green() - item1->GetColor().green() ) / range;
            const int bStep = ( item2->GetColor().blue() - item1->GetColor().blue() ) / range;
            const float xStep = float( rect.width() ) / range;

            QColor color( item1->GetColor() );
            QRect region( rect );
            for( unsigned int i = 0; i < unsigned int( range ); ++i )
            {
                color.setRgb( item1->GetColor().red() + rStep * i, item1->GetColor().green() + gStep * i, item1->GetColor().blue() + bStep * i );
                region.setLeft ( rect.left() + int( xStep * i ) );
                region.setRight( rect.left() + int( xStep * ( i + 1 ) ) );
                painter.fillRect( region, color );
            }
        }

    private:
        const std::vector< GradientItem* >& colors_;
        unsigned short topMargin_;
        unsigned short bottomMargin_;
    };

    struct sComparator
    {
        bool operator()( const GradientItem* lhs, const GradientItem* rhs ) const
        {
            return lhs->GetPercentage() < rhs->GetPercentage();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: GradientButton constructor

// Created: LGY 2011-01-06
// -----------------------------------------------------------------------------
GradientButton::GradientButton( const QString& objectName,
                                bool disableState,
                                const GradientItem::T_Drawer& itemDrawer,
                                QColor begin /* = Qt::green */,
                                QColor end /* = Qt::red */,
                                QWidget* parent /* = 0 */ )
    : Q3CanvasView( parent )
    , selected_    ( 0 )
    , disableState_( disableState )
    , itemDrawer_  ( itemDrawer )
{
    setObjectName( objectName );
    setFrameStyle( Q3Frame::Raised | Q3Frame::Box );
    setMargin( 5 );
    setFixedHeight( 80 );
    setHScrollBarMode( Q3ScrollView::AlwaysOff );
    setVScrollBarMode( Q3ScrollView::AlwaysOff );

    setCanvas( new GradientCanvas( this, colors_ ) );

    AddItem( 0, begin );
    AddItem( 100, end );
    setFocusPolicy( Qt::StrongFocus );
}

// -----------------------------------------------------------------------------
// Name: GradientButton destructor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientButton::~GradientButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GradientButton::mousePressEvent
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientButton::mousePressEvent( QMouseEvent* event )
{
    if( !canvas()->onCanvas( event->pos() ) )
        return;
    ClearSelection();
    Q3CanvasItemList list = canvas()->collisions( inverseWorldMatrix().map( event->pos() ) );
    for( Q3CanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
    {
        SetSelected( static_cast< GradientItem& >( **it ) ); // $$$$ SBO 2007-07-02: rotate through selection...
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: GradientButton::mouseReleaseEvent
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientButton::mouseReleaseEvent( QMouseEvent* event )
{
    if( !canvas()->onCanvas( event->pos() ) )
        return;
    if( !selected_ )
    {
        GradientItem* item = AddItem( unsigned short( event->pos().x() * 100 / canvas()->rect().width() ), Qt::red );
        SetSelected( *item );
        Update();
    }
    setFocus();
}

// -----------------------------------------------------------------------------
// Name: GradientButton::mouseMoveEvent
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientButton::mouseMoveEvent( QMouseEvent* event )
{
    if( !canvas()->onCanvas( event->pos() ) || !selected_ )
        return;
    if( event->button() | Qt::LeftButton )
    {
        selected_->SetPercentage( unsigned short( event->pos().x() * 100.f / canvas()->rect().width() ) );
        std::sort( colors_.begin(), colors_.end(), sComparator() );
        Update();
    }
}

// -----------------------------------------------------------------------------
// Name: GradientButton::keyPressEvent
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientButton::keyPressEvent( QKeyEvent* event )
{
    if( !selected_ )
        return;
    if( event->key() == Qt::Key_Delete && colors_.size() > 2 )
    {
        auto it = std::find( colors_.begin(), colors_.end(), selected_ );
        if( it != colors_.end() )
        {
            (*it)->hide();
            colors_.erase( it );
            delete selected_;
            selected_ = 0;
            Update();
        }
    }
}

namespace
{
    class GradientBuilder : public GradientVisitor_ABC
    {
    public:
        explicit GradientBuilder( GradientButton& gradient ) : gradient_( &gradient ) {}
        virtual ~GradientBuilder() {}

        virtual void Visit( float position, const QColor& color )
        {
            gradient_->AddItem( unsigned int( position * 100.f ), color );
        }

    private:
        GradientButton* gradient_;
    };
}

// -----------------------------------------------------------------------------
// Name: GradientButton::LoadGradient
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientButton::LoadGradient( const T_Gradient& gradient )
{
    ClearSelection();
    for( auto it = colors_.begin(); it != colors_.end(); ++it )
        if( *it )
        {
            (*it)->hide();
            delete *it;
        }
    colors_.clear();
    gradient_ = gradient;
    GradientBuilder builder( *this );
    gradient_->Accept( builder );
    Update( false );
}

// -----------------------------------------------------------------------------
// Name: GradientButton::SetSelectedColor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientButton::SetSelectedColor( const QColor& color )
{
    if( !selected_ )
        return;
    selected_->SetColor( color );
    Update();
}

// -----------------------------------------------------------------------------
// Name: GradientButton::GetSelectedColor
// Created: ABR 2014-10-03
// -----------------------------------------------------------------------------
QColor GradientButton::GetSelectedColor() const
{
    if( !selected_ )
        return QColor();
    return selected_->GetColor();
}

// -----------------------------------------------------------------------------
// Name: GradientButton::SetSelected
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientButton::SetSelected( GradientItem& item )
{
    item.setSelected( true );
    selected_ = &item;
    emit SelectionChanged( item.GetColor() );
}

// -----------------------------------------------------------------------------
// Name: GradientButton::ClearSelection
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientButton::ClearSelection()
{
    if( !selected_ )
        return;
    Q3CanvasItemList list = canvas()->allItems();
    for( Q3CanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
        (*it)->setSelected( false );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: GradientButton::Update
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientButton::Update( bool emitSignal /* = true */ )
{
    if( !gradient_ )
        return;
    canvas()->setAllChanged();
    gradient_->LoadValues( "" );
    Q3CanvasItemList list = canvas()->allItems();
    for( Q3CanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
    {
        GradientItem* item = static_cast< GradientItem* >( *it );
        item->ToggleScale( disableState_ );
        gradient_->AddColor( item->GetPercentage() / 100.f, item->GetColor() );
    }
    canvas()->update();
    if( emitSignal )
        emit GradientChanged();
}

// -----------------------------------------------------------------------------
// Name: GradientButton::AddItem
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientItem* GradientButton::AddItem( unsigned int percentage, const QColor& color )
{
    GradientItem* item = new GradientItem( canvas(), itemDrawer_, unsigned short( percentage ), color, disableState_ );
    colors_.push_back( item );
    std::sort( colors_.begin(), colors_.end(), sComparator() );
    return item;
}

// -----------------------------------------------------------------------------
// Name: GradientButton::resizeEvent
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientButton::resizeEvent( QResizeEvent* event )
{
    Q3CanvasView::resizeEvent( event );
    canvas()->resize( event->size().width(), event->size().height() );
    canvas()->setAllChanged();
    canvas()->update();
}

// -----------------------------------------------------------------------------
// Name: GradientButton::OnEnableVariableGradient
// Created: LGY 2010-09-29
// -----------------------------------------------------------------------------
void GradientButton::OnFitToViewPortChanged( int state )
{
    disableState_ = state == Qt::Checked;
    Update();
}
