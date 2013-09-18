// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_TimeField.h"
#include "moc_ADN_TimeField.cpp"
#include "ADN_Connector_ABC.h"
#include "ADN_Connector_String.h"
#include "ADN_App.h"
#include "ADN_Validator.h"
#include <tools/Exception.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qvalidator.h>
#include <QtGui/qspinbox.h>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>

namespace
{
    const int maxCentiSeconds = 359999999;

    class Orientation : public boost::noncopyable
    {
    public:
        virtual QString StringWithOrientation( const std::string& hours, const std::string& minutes,
                                               const std::string& seconds, const std::string& microseconds,
                                               const std::string& decimalPoint ) const = 0;
        virtual int ComputeModifier( const std::string& text, int cursor ) const = 0;
        virtual int ComputeValue( const QStringList& buffer ) const = 0;
    };

    class LeftToRight : public Orientation
    {
    public:
        virtual QString StringWithOrientation( const std::string& hours, const std::string& minutes,
                                               const std::string& seconds, const std::string& microseconds,
                                               const std::string& decimalPoint ) const
        {
            return ( hours + ":" + minutes + ":" + seconds + decimalPoint + microseconds ).c_str();
        }
        virtual int ComputeModifier( const std::string& text, int cursor ) const
        {
            const std::size_t ucursor = boost::numeric_cast< std::size_t >( cursor );
            return ucursor <= text.find_first_of( ':' )                               ? 360000
                 : ucursor <= text.find_last_of( ':' )                                ? 6000
                 : ucursor <= text.find_last_of( QLocale().decimalPoint().toAscii() ) ? 100
                                                                                      : 1;
        }
        virtual int ComputeValue( const QStringList& buffer ) const
        {
            const QStringList seconds = buffer.at( 2 ).split( QLocale().decimalPoint() );
            return buffer.at( 0 ).toInt() * 360000 + buffer.at( 1 ).toInt() * 6000 + seconds.at( 0 ).toInt() * 100 + seconds.at( 1 ).toInt();
        }
    };

    class RightToLeft : public Orientation
    {
    public:
        virtual QString StringWithOrientation( const std::string& hours, const std::string& minutes,
                                               const std::string& seconds, const std::string& microseconds,
                                               const std::string& decimalPoint ) const
        {
            return ( seconds + decimalPoint + microseconds + ":" + minutes + ":" + hours ).c_str();
        }
        virtual int ComputeModifier( const std::string& text, int cursor ) const
        {
            const std::size_t ucursor = boost::numeric_cast< std::size_t >( cursor );
            return ucursor <= text.find_last_of( QLocale().decimalPoint().toAscii() ) ? 100
                 : ucursor <= text.find_first_of( ':' )                               ? 1
                 : ucursor <= text.find_last_of( ':' )                                ? 6000
                                                                                      : 360000;
        }
        virtual int ComputeValue( const QStringList& buffer ) const
        {
            const QStringList seconds = buffer.at( 0 ).split( QLocale().decimalPoint() );
            return buffer.at( 2 ).toInt() * 360000 + buffer.at( 1 ).toInt() * 6000 + seconds.at( 0 ).toInt() * 100 + seconds.at( 1 ).toInt();
        }
    };

    class DurationSpinBox : public QSpinBox
    {
    public:
        DurationSpinBox( QWidget* parent = 0 )
            : QSpinBox( parent )
            , orientation_ ( qApp->isRightToLeft() ? static_cast< Orientation* >( new RightToLeft() )
                                                   : static_cast< Orientation* >( new LeftToRight() ) )
            , exact_       ( orientation_->StringWithOrientation( "\\d+", "[0-5]\\d", "[0-5]\\d", "\\d\\d", "[" + QString( QLocale().decimalPoint() ).toStdString() + "]" ) )
            , intermediate_( orientation_->StringWithOrientation( "\\d*", "\\d*", "\\d*", "\\d*", "[" + QString( QLocale().decimalPoint() ).toStdString() + "]" ) )
        {
            setValue( 0 );
            setMaximum( maxCentiSeconds );
        }
        virtual ~DurationSpinBox() {}
        virtual int valueFromText( const QString& text ) const
        {
            QStringList buffer = text.split( ":" );
            if( buffer.size() < 3 )
                throw MASA_EXCEPTION( tools::translate( "TimeField", "Invalid time duration '%1'" ).arg( text ).toStdString() );
            return orientation_->ComputeValue( buffer );
        }
        virtual QString textFromValue( int value ) const
        {
            const int minutes = ( value / 6000 ) % 60;
            const int hours = value / 360000;
            const int seconds = value / 100 - hours * 3600 - minutes * 60;
            const int centisec = value % 100;
            return orientation_->StringWithOrientation( boost::lexical_cast< std::string >( hours ), Fill( minutes ),
                                                        Fill( seconds ), Fill( centisec ), QString( QLocale().decimalPoint() ).toStdString() );
        }
        virtual void stepBy( int steps )
        {
            const std::string text( lineEdit()->text().toStdString() );
            const int cursor = lineEdit()->cursorPosition();
            setValue( value() + steps * orientation_->ComputeModifier( text, cursor ) );
        }
    protected:
        virtual QValidator::State validate( QString& text, int& ) const
        {
            return exact_.exactMatch( text )        ? QValidator::Acceptable :
                   intermediate_.exactMatch( text ) ? QValidator::Intermediate :
                                                      QValidator::Invalid;
        }
        std::string Fill( int number ) const
        {
            return ( number  < 10 ? "0" : "" ) + boost::lexical_cast< std::string >( number );
        }
    protected:
        boost::scoped_ptr< Orientation > orientation_;
        QRegExp exact_;
        QRegExp intermediate_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField constructor
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
ADN_TimeField::ADN_TimeField( QWidget* pParent, const char* szName /* = 0*/ )
    : QWidget( pParent, szName )
{
    pConnector_ = new ADN_Connector_String< ADN_TimeField >( this );
    // objects
    Q3HBoxLayout *pLayout = new Q3HBoxLayout( this );
    pLayout->setMargin( 0 );
    pSpinBox_ = new DurationSpinBox( this );
    pLayout->addWidget( pSpinBox_ );
    setFocusProxy( pSpinBox_ );
    connect( ADN_App::GetMainWindow(), SIGNAL( OpenModeToggled() ), this, SLOT( UpdateEnableState() ) );
    connect( pSpinBox_, SIGNAL( valueChanged( int ) ), this, SLOT( OnValueChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField destructor
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
ADN_TimeField::~ADN_TimeField()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::OnValueChanged
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
void ADN_TimeField::OnValueChanged()
{
    static_cast< ADN_Connector_String< ADN_TimeField >* >( pConnector_ )->SetDataChanged( text() );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::text
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
QString ADN_TimeField::text() const
{
    const int seconds = pSpinBox_->value() / 100;
    const int centiSeconds = pSpinBox_->value() % 100;
    return ( boost::lexical_cast< std::string >( seconds ) + "." // $$$$ _RC_ SLI 2013-09-18: we always want to save decimal time with US format '.'
           + boost::lexical_cast< std::string >( centiSeconds ) + "s" ).c_str();
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::setText
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
void ADN_TimeField::setText( const QString& strText )
{
    if( strText.isEmpty() )
        return;
    const QString strUnit( strText.right( 1 ) );
    const QString strValue( strText.left( strText.size() - 1 ) );
    pSpinBox_->setValue( GetCentiSeconds( strValue, strUnit ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_TimeField::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_String< ADN_TimeField >* >( pConnector_ )->IsConnected() );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::SetMinimumValueInSecond
// Created: ABR 2011-02-11
// -----------------------------------------------------------------------------
void ADN_TimeField::SetMinimumValueInSecond( unsigned int value )
{
    pSpinBox_->setMinimum( value );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::Warn
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
void ADN_TimeField::Warn( ADN_ErrorStatus, const QString& )
{
    // NOTHING (For now)
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::GetCentiSeconds
// Created: MMC 2013-04-04
// -----------------------------------------------------------------------------
int ADN_TimeField::GetCentiSeconds( const QString& strValue, const QString& strUnit ) const
{
    const int modifier = strUnit == "s" ? 100
                       : strUnit == "m" ? 6000
                       : strUnit == "h" ? 360000
                       :                  0;
    return boost::numeric_cast< int >( boost::lexical_cast< double >( strValue.toStdString() ) * modifier );
}
