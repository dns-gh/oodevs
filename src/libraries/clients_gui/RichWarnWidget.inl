// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: RichWarnWidget constructor
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
RichWarnWidget< T >::RichWarnWidget( const QString& objectName, QWidget* parent /* = 0 */ )
    : RichWidget< T >( objectName, parent )
    , baseWidget_( 0 )
    , timer_( 0 )
    , originalPalette_( palette() )
    , warningPalette_( CreatePalette() )
{
    baseWidget_ = new RichBaseObject( *this, objectName + "-base-widget",this );
    timer_ = new QTimer( this );
    connect( timer_, SIGNAL( timeout() ), baseWidget_, SLOT( OnBlink() ) );
}

// -----------------------------------------------------------------------------
// Name: RichWarnWidget destructor
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
RichWarnWidget< T >::~RichWarnWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichWarnWidget::Warn
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
inline
void RichWarnWidget< T >::Warn( int msec /* = 3000 */ )
{
    blink_ = false;
    timer_->start( 500 );
    QTimer::singleShot( msec, baseWidget_, SLOT( OnDone() ) );
    setPalette( warningPalette_ );
}

// -----------------------------------------------------------------------------
// Name: RichWarnWidget::OnBlink
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
inline
void RichWarnWidget< T >::OnBlink()
{
    setPalette( ( blink_ ) ? warningPalette_ : originalPalette_ );
    blink_ = !blink_;
}

// -----------------------------------------------------------------------------
// Name: RichWarnWidget::OnBlink
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
inline
void RichWarnWidget< T >::OnDone()
{
    timer_->stop();
    setPalette( originalPalette_ );
}

// -----------------------------------------------------------------------------
// Name: RichWarnWidget::CreatePalette
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
inline
QPalette RichWarnWidget< T >::CreatePalette() const
{
    QPalette p = palette();

    p.setColor( QPalette::WindowText, Qt::red );
    p.setColor( QPalette::Text, Qt::red );
    p.setColor( QPalette::ButtonText, Qt::red );
    p.setColor( QPalette::BrightText, Qt::red );

    p.setColor( QPalette::Light, Qt::red );
    p.setColor( QPalette::Midlight, Qt::red );
    return p;
}

// -----------------------------------------------------------------------------
// Name: RichWarnWidget::EnableStaticWarning
// Created: ABR 2013-10-31
// -----------------------------------------------------------------------------
template< typename T >
void RichWarnWidget< T >::EnableStaticWarning( bool warn )
{
    setPalette( ( warn ) ? warningPalette_ : originalPalette_ );
}

/*
QPalette::Window            // A general background color.
QPalette::WindowText        // A general foreground color.
QPalette::Base              // Used mostly as the background color for text entry widgets, but can also be used for other painting - such as the background of combobox drop down lists and toolbar handles. It is usually white or another light color.
QPalette::AlternateBase     // Used as the alternate background color in views with alternating row colors (see QAbstractItemView::setAlternatingRowColors()).
QPalette::ToolTipBase       // Used as the background color for QToolTip and QWhatsThis. Tool tips use the Inactive color group of QPalette, because tool tips are not active windows.
QPalette::ToolTipText       // Used as the foreground color for QToolTip and QWhatsThis. Tool tips use the Inactive color group of QPalette, because tool tips are not active windows.
QPalette::Text              // The foreground color used with Base. This is usually the same as the WindowText, in which case it must provide good contrast with Window and Base.
QPalette::Button            // The general button background color. This background can be different from Window as some styles require a different background color for buttons.
QPalette::ButtonText        // A foreground color used with the Button color.
QPalette::BrightText        // Text color that is very different from WindowText, and contrasts well with e.g. Dark.
                               Typically used for text that needs to be drawn where Text or WindowText would give poor contrast, such as on pressed push buttons.
                               Note that text colors can be used for things other than just words; text colors are usually used for text, but it's quite common to use the text color roles for lines, icons, etc.

QPalette::Light             // Lighter than Button color.
QPalette::Midlight          // Between Button and Light.
QPalette::Dark              // Darker than Button.
QPalette::Mid               // Between Button and Dark.
QPalette::Shadow            // A very dark color. By default, the shadow color is Qt::black.

QPalette::Highlight         // A color to indicate a selected item or the current item. By default, the highlight color is Qt::darkBlue.
QPalette::HighlightedText   // A text color that contrasts with Highlight. By default, the highlighted text color is Qt::white.

QPalette::Link              // A text color used for unvisited hyperlinks. By default, the link color is Qt::blue.
QPalette::LinkVisited       // A text color used for already visited hyperlinks. By default, the linkvisited color is Qt::magenta.
*/
