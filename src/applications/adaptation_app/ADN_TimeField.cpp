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
#include "ADN_Connector_String.h"
#include "ADN_Tools.h"
#include <boost/numeric/conversion/cast.hpp>

namespace
{
    const int maxCentiSeconds = 359999999;

    int ComputeModifier( const std::string& text, int cursor )
    {
        const std::size_t ucursor = boost::numeric_cast< std::size_t >( cursor );
        if( qApp->isLeftToRight() )
            return ucursor <= text.find_first_of( ':' )                                 ? 360000
                   : ucursor <= text.find_last_of( ':' )                                ? 6000
                   : ucursor <= text.find_last_of( QLocale().decimalPoint().toAscii() ) ? 100
                                                                                        : 1;

        return ucursor <= text.find_last_of( QLocale().decimalPoint().toAscii() ) ? 100
               : ucursor <= text.find_first_of( ':' )                             ? 1
               : ucursor <= text.find_last_of( ':' )                              ? 6000
               :                                                                    360000;
    }

    class DurationSpinBox : public QSpinBox
    {
    public:
        DurationSpinBox( QWidget* parent = 0 )
            : QSpinBox( parent )
            , exact_       ( ADN_Tools::BuildLongString( "\\d+", "[0-5]\\d", "[0-5]\\d", "\\d\\d", QRegExp::escape( QLocale().decimalPoint() ).toStdString() ) )
            , intermediate_( ADN_Tools::BuildLongString( "\\d*", "\\d*", "\\d*", "\\d*", QRegExp::escape( QLocale().decimalPoint() ).toStdString() ) )
        {
            setValue( 0 );
            setMaximum( maxCentiSeconds );
        }
        virtual ~DurationSpinBox() {}
        virtual int valueFromText( const QString& text ) const
        {
            return ADN_Tools::ConvertLongStringToCentiseconds( text );
        }
        virtual QString textFromValue( int value ) const
        {
            return ADN_Tools::ConvertCentisecondsToLongString( value );
        }
        virtual void stepBy( int steps )
        {
            const std::string text( lineEdit()->text().toStdString() );
            const int cursor = lineEdit()->cursorPosition();
            setValue( value() + steps * ComputeModifier( text, cursor ) );
        }
    protected:
        virtual QValidator::State validate( QString& text, int& ) const
        {
            return exact_.exactMatch( text )        ? QValidator::Acceptable :
                   intermediate_.exactMatch( text ) ? QValidator::Intermediate :
                                                      QValidator::Invalid;
        }
    protected:
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
    pConnector_.reset( new ADN_Connector_String< ADN_TimeField >( this ) );
    // objects
    Q3HBoxLayout *pLayout = new Q3HBoxLayout( this );
    pLayout->setMargin( 0 );
    pSpinBox_ = new DurationSpinBox( this );
    pLayout->addWidget( pSpinBox_ );
    setFocusProxy( pSpinBox_ );
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
    static_cast< ADN_Connector_String< ADN_TimeField >& >( *pConnector_ ).SetDataChanged( text() );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::text
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
QString ADN_TimeField::text() const
{
    return ADN_Tools::ConvertCentisecondsToDelay( pSpinBox_->value() );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::setText
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
void ADN_TimeField::setText( const QString& strText )
{
    if( strText.isEmpty() )
        return;
    pSpinBox_->setValue( ADN_Tools::ConvertDelayToCentiseconds( strText ) );
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
// Name: ADN_TimeField::setObjectName
// Created: ABR 2013-10-21
// -----------------------------------------------------------------------------
void ADN_TimeField::setObjectName( const QString& name )
{
    QWidget::setObjectName( name );
    pSpinBox_->setObjectName( name + "-spinbox" );
}
