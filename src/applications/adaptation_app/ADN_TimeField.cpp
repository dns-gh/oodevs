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
#include "ADN_EditLine.h"
#include "ADN_MainWindow.h"
#include "ADN_Validator.h"

#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qvalidator.h>

class ADN_TimeField_EditLine : public ADN_EditLine_ToolTiped
{
public:
    ADN_TimeField_EditLine( QWidget* pParent )
        : ADN_EditLine_ToolTiped( pParent )
        , bIsFocusing_( false )
    {
        setFrame( true );
    }

    ~ADN_TimeField_EditLine()
    {}

protected:
    void focusInEvent( QFocusEvent* pEvent )
    {
        QLineEdit::focusInEvent( pEvent );
        bIsFocusing_ = true;
        selectAll();
    }

    void focusOutEvent( QFocusEvent* pEvent )
    {
        QLineEdit::focusOutEvent( pEvent );
    }

    void mouseReleaseEvent( QMouseEvent* e )
    {
        QLineEdit::mouseReleaseEvent( e );
        if( bIsFocusing_ )
        {
            selectAll();
            bIsFocusing_ = false;
        }
    }

    virtual void mousePressEvent( QMouseEvent* event )
    {
        if( event && ( event->button() == Qt::XButton1 || event->button() == Qt::XButton2 ) )
            event->ignore();
        else
            QLineEdit::mousePressEvent( event );
    }

private:
    bool bIsFocusing_;
};

// -----------------------------------------------------------------------------
// Name: ADN_TimeField constructor
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
ADN_TimeField::ADN_TimeField( QWidget* pParent, const char* szName /* = 0*/ )
    : QWidget              ( pParent, szName )
    , ADN_Gfx_ABC          ()
    , secondsValue_        ( 0.f )
    , nMinimumSecondsValue_( 0 )
    , nMinimumMinutesValue_( 0 )
    , nMinimumHoursValue_  ( 0 )
    , bFreezeSlot_         ( false )
{
    // $$$$ ABR 2012-05-02: Need many transformation (locale().toString or QString::number) in order to manipulate non-localized data and to display localized data
    pConnector_ = new ADN_Connector_String<ADN_TimeField>(this);

    // objects
    Q3HBoxLayout *pLayout = new Q3HBoxLayout( this );
    pLayout->setMargin( 0 );

    pLineEdit_ = new ADN_TimeField_EditLine( this );
    pLineEdit_->setAlignment( qApp->isRightToLeft() ? Qt::AlignLeft : Qt::AlignRight );

    pLayout->addWidget( pLineEdit_ );

    setFocusProxy( pLineEdit_ );

    pComboBox_ = new QComboBox( this );
    pComboBox_->insertItem( "s" );
    pComboBox_->insertItem( "m" );
    pComboBox_->insertItem( "h" );
    pLayout->addWidget( pComboBox_ );

    connect( pLineEdit_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnValueChanged( const QString& ) ) );
    connect( pComboBox_, SIGNAL( activated  ( const QString& ) ), this, SLOT( OnUnitChanged ( const QString& ) ) );
    pValidator_ = new ADN_DoubleValidator( 0., INT_MAX, 10, pLineEdit_ );
    pLineEdit_->setValidator( pValidator_ );
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
void ADN_TimeField::OnValueChanged( const QString& strValue )
{
    if( bFreezeSlot_ )
        return;

    secondsValue_ = GetSeconds( strValue, pComboBox_->currentText() );
    if( secondsValue_ < static_cast< float >( nMinimumSecondsValue_ ) )
    {
        secondsValue_ = static_cast< float >( nMinimumSecondsValue_ );
        pLineEdit_->setText( locale().toString( GetDisplayMin( pComboBox_->currentText() ) ) );
    }

    NotifyDataChanged();
    emit ValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::OnUnitChanged
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
void ADN_TimeField::OnUnitChanged( const QString& strUnit )
{
    bFreezeSlot_ = true;
    pLineEdit_->setText( locale().toString( GetDisplayValue( strUnit ) ) );
    NotifyDataChanged();
    bFreezeSlot_ = false;
    pLineEdit_->setFocus();
    emit ValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::text
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
QString ADN_TimeField::text() const
{
    float value = locale().toFloat( pLineEdit_->text() );
    return QString::number( value ) + pComboBox_->currentText();
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::setText
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
void ADN_TimeField::setText( const QString& strText )
{
    if( strText.isEmpty() )
        return;
    QString strUnit( strText.right( 1 ) );
    QString strValue( strText.left( strText.size() - 1 ) );
    secondsValue_ = GetSeconds( strValue, strUnit );
    pComboBox_->setCurrentText( strUnit );
    pLineEdit_->setText( locale().toString( strValue.toFloat() ) );
    emit ValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::GetValidator
// Created: SBO 2006-01-17
// -----------------------------------------------------------------------------
ADN_DoubleValidator& ADN_TimeField::GetValidator()
{
    assert( pValidator_ );
    return *pValidator_;
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::SetMinimumValueInSecond
// Created: ABR 2011-02-11
// -----------------------------------------------------------------------------
void ADN_TimeField::SetMinimumValueInSecond( unsigned int value )
{
    nMinimumSecondsValue_ = value;
    nMinimumMinutesValue_ = value/60;
    nMinimumHoursValue_ = value/3600;

    if( value%60 > 0 )
        nMinimumMinutesValue_ += 1;
    if( value%3600 > 0 )
        nMinimumHoursValue_ += 1;
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
// Name: ADN_TimeField::NotifyDataChanged
// Created: MMC 2013-04-04
// -----------------------------------------------------------------------------
void ADN_TimeField::NotifyDataChanged()
{
    static_cast< ADN_Connector_String<ADN_TimeField>* >( pConnector_ )->SetDataChanged(
        QString::number( GetDisplayValue( pComboBox_->currentText() ) ) + pComboBox_->currentText() );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::GetSeconds
// Created: MMC 2013-04-04
// -----------------------------------------------------------------------------
float ADN_TimeField::GetSeconds( const QString& strValue, const QString& strUnit ) const
{
    if( strUnit == "s" )
        return locale().toFloat( strValue );
    else if( strUnit == "m" )
        return locale().toFloat( strValue ) * 60;
    else if( strUnit == "h" )
        return locale().toFloat( strValue ) * 3600;
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::GetDisplayValue
// Created: MMC 2013-04-04
// -----------------------------------------------------------------------------
float ADN_TimeField::GetDisplayValue( const QString& strUnit ) const
{
    if( strUnit == "s" )
        return secondsValue_;
    else if( strUnit == "m" )
        return std::max< float >( secondsValue_ / 60, static_cast< float >( nMinimumMinutesValue_ ) );
    else if( strUnit == "h" )
        return std::max< float >( secondsValue_ / 3600, static_cast< float >( nMinimumHoursValue_ ) );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::GetDisplayMin
// Created: MMC 2013-04-04
// -----------------------------------------------------------------------------
float ADN_TimeField::GetDisplayMin( const QString& strUnit ) const
{
    if( nMinimumSecondsValue_ > 0 )
    {
        if( strUnit == "s" )
            return static_cast< float >( nMinimumSecondsValue_ );
        else if( strUnit == "m" )
            return static_cast< float >( nMinimumMinutesValue_ );
        else if( strUnit == "h" )
            return static_cast< float >( nMinimumHoursValue_ );
    }
    return 0.f;
}
