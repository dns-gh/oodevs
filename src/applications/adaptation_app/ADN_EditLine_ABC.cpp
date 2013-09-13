// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_EditLine_ABC.h"
#include "moc_ADN_EditLine_ABC.cpp"
#include "ADN_Ref_ABC.h"

// -----------------------------------------------------------------------------
// ADN_EditLine_ToolTiped
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ToolTiped constructor
// Created: ABR 2012-04-11
// -----------------------------------------------------------------------------
ADN_EditLine_ToolTiped::ADN_EditLine_ToolTiped( QWidget* parent, const char * name /* = 0 */ )
    : QLineEdit( parent, name )
{
    setAlignment( Qt::AlignLeft );
}

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ToolTiped destructor
// Created: ABR 2012-04-11
// -----------------------------------------------------------------------------
ADN_EditLine_ToolTiped::~ADN_EditLine_ToolTiped()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ToolTiped::ConvertMinMaxValue
// Created: ABR 2012-04-11
// -----------------------------------------------------------------------------
QString ADN_EditLine_ToolTiped::ConvertMinMaxValue( double value, int decimals /*= -1*/ )
{
    QString result;
    if( value <= std::numeric_limits< int >::min() )
        result = '-' + QChar( ( ushort ) 0x221E );
    else if( value >= std::numeric_limits< int >::max() )
        result = '+' + QChar( ( ushort ) 0x221E );
    else
    {
        if( decimals != -1 )
        {
            result = locale().toString( value, 'f', decimals );
            int i = result.find( locale().decimalPoint() );
            if( i >= 0 )
            {
                int j = result.size() - 1;
                while( j > i && result[ j ] == '0' ){ --j; }
                result.truncate( j == i ? j : j + 1 );
            }
        }
        else
            result = locale().toString( value );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ToolTiped::event
// Created: ABR 2012-04-11
// -----------------------------------------------------------------------------
bool ADN_EditLine_ToolTiped::event( QEvent* event )
{
    if( event->type() == QEvent::ToolTip && this->validator() )
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        QString toolTips;

        if( const QIntValidator* validator = dynamic_cast< const QIntValidator* >( this->validator() ) )
            toolTips += QString( "[%1 .. %2]" ).arg( ConvertMinMaxValue( validator->bottom() ) )
            .arg( ConvertMinMaxValue( validator->top() ) );
        else if( const QDoubleValidator* validator = dynamic_cast< const QDoubleValidator* >( this->validator() ) )
            toolTips += QString( "[%1 .. %2]" ).arg( ConvertMinMaxValue( validator->bottom(), validator->decimals() ) )
            .arg( ConvertMinMaxValue( validator->top(), validator->decimals() ) );

        if( !toolTips.isEmpty() )
            QToolTip::showText( helpEvent->globalPos(), toolTips );
    }
    return QLineEdit::event( event );
}

// -----------------------------------------------------------------------------
// ADN_EditLine_ABC
// -----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC constructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_EditLine_ABC::ADN_EditLine_ABC( QWidget* parent, const char* name )
    : ADN_EditLine_ToolTiped( parent, name )
    , ADN_Gfx_ABC()
    , originalPalette_( palette() )
    , originalToolTip_( "" )
{
    // connect edit line & connector
    connect( this, SIGNAL( textChanged( const QString & ) ), this, SLOT( TextChanged( const QString & ) ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC destructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_EditLine_ABC::~ADN_EditLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC::focusOutEvent
// Created: APE 2005-04-13
// -----------------------------------------------------------------------------
void ADN_EditLine_ABC::focusOutEvent( QFocusEvent* pEvent )
{
    QLineEdit::focusOutEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC::focusInEvent
// Created: ABR 2012-11-19
// -----------------------------------------------------------------------------
void ADN_EditLine_ABC::focusInEvent( QFocusEvent* pEvent )
{
    emit StartEditing();
    QLineEdit::focusInEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC::SetToolTip
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_EditLine_ABC::SetToolTip( const QString& toolTip )
{
    originalToolTip_ = toolTip;
    setToolTip( originalToolTip_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC::ConnectWithRefValidity
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_EditLine_ABC::ConnectWithRefValidity( const ADN_Ref_ABC& ref )
{
    connect( this, SIGNAL( textChanged( const QString& ) ), &ref, SLOT( CheckValidity() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC::Warn
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
void ADN_EditLine_ABC::Warn( ADN_ErrorStatus errorStatus, const QString& errorMessage /*= ""*/ )
{
    switch( errorStatus )
    {
    case eNoError:
        {
            setToolTip( originalToolTip_ );
            setPalette( originalPalette_ );
        }
        break;
    case eWarning:
        {
            if( !errorMessage.isEmpty() )
                setToolTip( errorMessage );
            QPalette palette;
            palette.setColor( QPalette::Text, Qt::black );
            palette.setColor( QPalette::Base, Qt::yellow );
            palette.setColor( QPalette::Shadow, Qt::yellow );
            palette.setColor( QPalette::Window, Qt::yellow );
            palette.setColor( QPalette::Highlight, QColor( Qt::yellow ).lighter( 50 ) );
            palette.setColor( QPalette::HighlightedText, Qt::black );
            setPalette( palette );
        }
        break;
    case eError:
        {
            if( !errorMessage.isEmpty() )
                setToolTip( errorMessage );
            QPalette palette;
            palette.setColor( QPalette::Text, Qt::black );
            palette.setColor( QPalette::Base, Qt::red );
            palette.setColor( QPalette::Shadow, Qt::red );
            palette.setColor( QPalette::Window, Qt::red );
            palette.setColor( QPalette::Highlight, QColor( Qt::red ).lighter( 150 ) );
            palette.setColor( QPalette::HighlightedText, Qt::black );
            setPalette( palette );
        }
        break;
    default:
        break;
    }
}
