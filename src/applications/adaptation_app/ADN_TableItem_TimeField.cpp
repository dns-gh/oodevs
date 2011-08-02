// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_TableItem_TimeField.h"
#include "moc_ADN_TableItem_TimeField.cpp"
#include "ADN_TimeField.h"
#include "ADN_Connector_String.h"

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_TimeField constructor
// Created: SBO 2005-09-13
// -----------------------------------------------------------------------------
ADN_TableItem_TimeField::ADN_TableItem_TimeField( ADN_Table* parent, void *data, Q3TableItem::EditType nEditType /* = OnTyping*/ )
    : ADN_TableItem_ABC( parent, data, nEditType )
    , pTimeField_      ( 0 )
    , pValidator_      ( new QRegExpValidator( this ) )
    , nAlignment_      ( Qt::AlignRight | Qt::AlignVCenter )
{
    pConnector_ = new ADN_Connector_String< ADN_TableItem_TimeField >( this );
    assert( pConnector_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_TimeField destructor
// Created: SBO 2005-09-13
// -----------------------------------------------------------------------------
ADN_TableItem_TimeField::~ADN_TableItem_TimeField()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_TimeField::createEditor
// Created: SBO 2005-09-13
// -----------------------------------------------------------------------------
QWidget* ADN_TableItem_TimeField::createEditor() const
{
    pTimeField_ = new ADN_TimeField( table()->viewport() );
    pTimeField_->setDisabled( false );
    pTimeField_->setText( text() );
    connect( pTimeField_, SIGNAL( ValueChanged() ), this, SLOT( OnValueChanged() ) );
    return pTimeField_;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_TimeField::OnValueChanged
// Created: SBO 2005-09-16
// -----------------------------------------------------------------------------
void ADN_TableItem_TimeField::OnValueChanged()
{
    setContentFromEditor( pTimeField_ );
    DoValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_TimeField::setContentFromEditor
// Created: SBO 2005-09-13
// -----------------------------------------------------------------------------
void ADN_TableItem_TimeField::setContentFromEditor( QWidget* pWidget )
{
    setText( static_cast< ADN_TimeField* >( pWidget )->text() );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_TimeField::DoValueChanged
// Created: SBO 2005-09-13
// -----------------------------------------------------------------------------
void ADN_TableItem_TimeField::DoValueChanged()
{
    static_cast< ADN_Connector_String< ADN_TableItem_TimeField >* >( pConnector_ )->SetDataChanged( text() );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_TimeField::GetValidator
// Created: SBO 2005-09-13
// -----------------------------------------------------------------------------
QRegExpValidator& ADN_TableItem_TimeField::GetValidator() const
{
    assert( pValidator_ );
    return *pValidator_;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_TimeField::SetValidator
// Created: SBO 2005-09-13
// -----------------------------------------------------------------------------
void ADN_TableItem_TimeField::SetValidator( QRegExpValidator* pValidator )
{
    delete pValidator_;
    pValidator_ = pValidator;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_TimeField::SetAlignment
// Created: SBO 2005-09-13
// -----------------------------------------------------------------------------
void ADN_TableItem_TimeField::SetAlignment( int nAlignment )
{
    nAlignment_ = nAlignment;
}


// -----------------------------------------------------------------------------
// Name: ADN_TableItem_TimeField::alignment
// Created: SBO 2005-09-13
// -----------------------------------------------------------------------------
int ADN_TableItem_TimeField::alignment() const
{
    if( nAlignment_ == -1 )
        return Q3TableItem::alignment();
    return nAlignment_;
}
