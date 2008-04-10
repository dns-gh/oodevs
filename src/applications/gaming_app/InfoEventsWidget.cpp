// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoEventsWidget.h"
#include "gaming/Attributes.h"
#include "gaming/Contaminations.h"
#include "gaming/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/NBCAgent.h"
#include "icons.h"
#include <qimage.h>
#include <qgrid.h>
#include <qobjectlist.h>
#include <qpainter.h>

namespace
{
    class MinimalBox : public QHBox
    {
    public:
        explicit MinimalBox( QWidget* parent )
            : QHBox( parent )
        {
            setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );
        }

    protected:
        virtual QSize sizeHint() const
        {
            QObjectList* list = queryList( "QButton" );
            QObjectListIt it( *list );
            int width = 0;
            while( QObject* obj = it.current() )
            {
                QButton* btn = static_cast< QButton* >( obj );
                if( btn->isShown() )
                    width += btn->width() + 2;
                ++it;
            }
            return QSize( width, height() );
        }
    };

    class TransparentButton : public QButton
    {
    public:
        TransparentButton( QWidget* parent, const QPixmap& pixmap, const QString& text )
            : QButton( parent )
        {
            parent->setFixedSize( 27, 27 );
            QImage img; // $$$$ SBO 2007-02-09: TODO: make new icons instead of resizing
            img = pixmap;
            img = img.smoothScale( 26, 26, QImage::ScaleMax );
            QPixmap pix( img );
            parent->setPaletteBackgroundPixmap( pix );
            QFont font;
            font.setPointSize( 9 );
            setFont( font );
            setBackgroundOrigin( QWidget::ParentOrigin );
            setFixedSize( 27, 27 );
            QToolTip::add( this, text );
            hide();
        }

    protected:
        virtual void drawButton( QPainter* paint )
        {
            QPen pen( QColor( 100, 100, 100 ) );
            paint->setPen( pen );
            paint->drawRoundRect( 0, 0, width(), height() );
            if( text().isEmpty() )
                return;
            const QRect rect = paint->boundingRect( QRect( 0, 0, width() - 2, height() - 1 ), Qt::AlignRight | Qt::AlignBottom, text() );
            const QRect out( rect.left() - 1, rect.top() - 1, rect.width() + 3, rect.height() + 2 );
            paint->fillRect( out, Qt::white );
            paint->drawRoundRect( out );
            pen.setColor( Qt::black );
            paint->setPen( pen );
            paint->drawText( rect, Qt::AlignRight | Qt::AlignBottom, text() );
        }
        
        virtual void show()
        {
            QButton::show();
            parentWidget()->show();
        }

        virtual void hide()
        {
            QButton::hide();
            parentWidget()->hide();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget constructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoEventsWidget::InfoEventsWidget( QWidget* parent, kernel::Controllers& controllers )
    : QGroupBox( 1, Qt::Horizontal, parent )
    , controllers_( controllers )
    , selected_( controllers )
{
    setFixedHeight( 31 );
    setMargin( 0 );
    setInsideMargin( 2 );
    setAlignment( Qt::AlignLeft );
    QHBox* box = new MinimalBox( this );
    box->setMargin( 0 );
    InitializeEventButtons( box );
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget destructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoEventsWidget::~InfoEventsWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::InitializeEventButtons
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void InfoEventsWidget::InitializeEventButtons( QWidget* parent )
{
    eventButtons_["jammed"]        = MakeButton( parent, MAKE_PIXMAP( brouillage )     , tools::translate( "InfoEventsWidget", "Communication: jammed" ) );
    eventButtons_["silence"]       = MakeButton( parent, MAKE_PIXMAP( talkie_interdit ), tools::translate( "InfoEventsWidget", "Communication: radio silence" ) );
    eventButtons_["radar"]         = MakeButton( parent, MAKE_PIXMAP( radars_on )      , tools::translate( "InfoEventsWidget", "Communication: radar enabled" ) );
    eventButtons_["stealth"]       = MakeButton( parent, MAKE_PIXMAP( csword )         , tools::translate( "InfoEventsWidget", "Communication: stealth mode" ) );
    eventButtons_["nbc suit"]      = MakeButton( parent, MAKE_PIXMAP( nbc )            , tools::translate( "InfoEventsWidget", "NBC: suit on" ) );
    eventButtons_["contamination"] = MakeButton( parent, MAKE_PIXMAP( nbc )            , "" );
    eventButtons_["refugees"]      = MakeButton( parent, MAKE_PIXMAP( csword )         , tools::translate( "InfoEventsWidget", "Refugees handled" ) );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::MakeButton
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
QButton* InfoEventsWidget::MakeButton( QWidget* parent, const QPixmap& pixmap, const QString& text )
{
    return new TransparentButton( new QWidget( parent ), pixmap, text );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::AddEventButton
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
void InfoEventsWidget::AddEventButton( const std::string& event, bool add )
{
    eventButtons_[event]->setShown( add );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::SetAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void InfoEventsWidget::SetAttributes( const Attributes& attributes )
{
    AddEventButton( "jammed"  , attributes.bCommJammed_ );
    AddEventButton( "silence" , attributes.bRadioSilence_ );
    AddEventButton( "radar"   , attributes.bRadarEnabled_ );
    AddEventButton( "stealth" , attributes.bStealthModeEnabled_ );
    AddEventButton( "refugees", attributes.bRefugeesManaged_ );
    SetShown();
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::SetContaminations
// Created: SBO 2007-02-09
// -----------------------------------------------------------------------------
void InfoEventsWidget::SetContaminations( const Contaminations& attributes )
{
    AddEventButton( "nbc suit", attributes.bNbcProtectionSuitWorn_ );
    AddEventButton( "contamination", !attributes.contaminatingNbcAgents_.empty() );
    if( !attributes.contaminatingNbcAgents_.empty() )
    {
        QButton* btn = eventButtons_["contamination"];
        QStringList agents;
        for( unsigned int i = 0; i < attributes.contaminatingNbcAgents_.size(); ++i )
            agents.append( attributes.contaminatingNbcAgents_[i]->GetName().c_str() );
        btn->setText( QString::number( attributes.nContamination_ ) );
        QToolTip::add( btn, tools::translate( "InfoEventsWidget", "NBC: contamination of type '%1' level '%2'" ).arg( agents.join( ", " ) ).arg( attributes.nContamination_ ) );
    }
    SetShown();
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::NotifySelected
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
void InfoEventsWidget::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( selected_ != entity )
    {
        selected_ = entity;
        hide();
        Reset();
        if( selected_ )
        {
            if( const Attributes* attributes = static_cast< const Attributes* >( selected_->Retrieve< kernel::Attributes_ABC >() ) )
                SetAttributes( *attributes );
            if( const Contaminations* contaminations = selected_->Retrieve< Contaminations >() )
                SetContaminations( *contaminations );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::NotifyUpdated
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void InfoEventsWidget::NotifyUpdated( const kernel::Attributes_ABC& element )
{
    if( !selected_ || selected_->Retrieve< kernel::Attributes_ABC >() != &element )
        return;
    const Attributes& humans = static_cast< const Attributes& >( element );
    SetAttributes( humans );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::NotifyUpdated
// Created: SBO 2007-02-09
// -----------------------------------------------------------------------------
void InfoEventsWidget::NotifyUpdated( const Contaminations& element )
{
    if( !selected_ )
        return;
    const Contaminations* contaminations = selected_->Retrieve< Contaminations >();
    if( contaminations == &element )
        SetContaminations( element );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::SetShown
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void InfoEventsWidget::SetShown()
{
    for( CIT_EventButtons it = eventButtons_.begin(); it != eventButtons_.end(); ++it )
        if( it->second->isShown() )
        {
            show();
            return;
        }
    hide();
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::Reset
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void InfoEventsWidget::Reset()
{
    for( CIT_EventButtons it = eventButtons_.begin(); it != eventButtons_.end(); ++it )
        it->second->hide();
}
