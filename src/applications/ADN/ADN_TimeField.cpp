// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-09 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "ADN_pch.h"
#include "ADN_TimeField.h"

#include "moc_ADN_TimeField.cpp"

#include "ADN_Connector_ABC.h"
#include "ADN_Connector_String.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"
#include "ADN_Validator.h"

#include <qlayout.h>
#include <qlineedit.h>
#include <qcombobox.h>


class ADN_TimeField_EditLine : public QLineEdit
{
public:
    ADN_TimeField_EditLine( QWidget* pParent )
        : QLineEdit( pParent )
        , bIsFocusing_( false )
    {
        setAlignment( Qt::AlignRight );
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

    void mouseReleaseEvent( QMouseEvent* e )
    {
        QLineEdit::mouseReleaseEvent( e );
        if( bIsFocusing_ )
        {
            selectAll();
            bIsFocusing_ = false;
        }
    }

private:
    bool bIsFocusing_;
};

// -----------------------------------------------------------------------------
// Name: ADN_TimeField constructor
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
ADN_TimeField::ADN_TimeField( QWidget* pParent, const char* szName /*= 0*/ )
    : QWidget        ( pParent, szName )
    , ADN_Gfx_ABC    ()
    , nSecondsValue_ ( 0 )
    , bFreezeSlot_   ( false )
{
    pConnector_ = new ADN_Connector_String<ADN_TimeField>(this);

    // objects
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->setMargin( 0 );

    pLineEdit_ = new ADN_TimeField_EditLine( this );
    pLayout->addWidget( pLineEdit_ );

    setFocusProxy( pLineEdit_ );

    pComboBox_ = new QComboBox( this );
    pComboBox_->insertItem( "s" );
    pComboBox_->insertItem( "m" );
    pComboBox_->insertItem( "h" );
    pLayout->addWidget( pComboBox_ );

    connect( pLineEdit_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnValueChanged( const QString& ) ) );
    connect( pComboBox_, SIGNAL( activated  ( const QString& ) ), this, SLOT( OnUnitChanged ( const QString& ) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
    pLineEdit_->setValidator( new ADN_DoubleValidator( 0.0, 1000000.0, 2, pLineEdit_ ) );
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
    if( pComboBox_->currentText() == "s" )
        nSecondsValue_ = strValue.toUInt();
    else if( pComboBox_->currentText() == "m" )
        nSecondsValue_ = strValue.toUInt() * 60;
    else if( pComboBox_->currentText() == "h" )
        nSecondsValue_ = strValue.toUInt() * 3600;
    
    // $$$$ SBO 2005-10-12: to avoid line field beeing empty
    if( strValue.isEmpty() )
    {
        pLineEdit_->setText( "0" );
        static_cast<ADN_Connector_String<ADN_TimeField>*>(pConnector_)->SetDataChanged( "0" + pComboBox_->currentText() );
    }
    else
        static_cast<ADN_Connector_String<ADN_TimeField>*>(pConnector_)->SetDataChanged( strValue + pComboBox_->currentText() );
    emit ValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::OnUnitChanged
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
void ADN_TimeField::OnUnitChanged( const QString& strUnit )
{
    bFreezeSlot_ = true;
    if( strUnit == "s" )
        pLineEdit_->setText( QString::number( nSecondsValue_ ) );
    else if( strUnit == "m" )
        pLineEdit_->setText( QString::number( nSecondsValue_ / 60 ) );
    else if( strUnit == "h" )
        pLineEdit_->setText( QString::number( nSecondsValue_ / 3600 ) );
    static_cast<ADN_Connector_String<ADN_TimeField>*>(pConnector_)->SetDataChanged( pLineEdit_->text() + strUnit );
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
    return pLineEdit_->text() + pComboBox_->currentText();
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::setText
// Created: SBO 2005-09-09
// -----------------------------------------------------------------------------
void ADN_TimeField::setText( const QString& strText )
{
    if( strText.isEmpty() )
        return;

    const std::string strValue = std::string( strText );

    const char cUnit = *strValue.rbegin();

    std::string       strTimeValue = strValue.substr( 0, strValue.size() - 1 );
    std::stringstream strTmp( strTimeValue );
    strTmp >> nSecondsValue_;

    switch( cUnit )
    {
        case 's': break;
        case 'm': nSecondsValue_ *= 60  ; break;
        case 'h': nSecondsValue_ *= 3600; break;
        default:
            return;
    }

    std::stringstream strUnit;
    strUnit << cUnit;
    pComboBox_->setCurrentText( strUnit.str().c_str() );
    pLineEdit_->setText( strTimeValue.c_str() );
    emit ValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeField::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_TimeField::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_String<ADN_TimeField>* >( pConnector_ )->IsConnected() );
}

