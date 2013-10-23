// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ComboBoxItem.h"
#include "moc_ADN_ComboBoxItem.cpp"
#include "ADN_ComboBox.h"
#include "ADN_Connector_String.h"

//-----------------------------------------------------------------------------
// Name: ADN_ComboBoxItem constructor
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
ADN_ComboBoxItem::ADN_ComboBoxItem(ADN_ComboBox& combo,void *data)
    : combo_(combo)
    , pData_(data)
    , szTmpTxt_()
{
    pConnector_.reset( new ADN_Connector_String< ADN_ComboBoxItem >( this ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBoxItem destructor
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
ADN_ComboBoxItem::~ADN_ComboBoxItem()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBoxItem::setText
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_ComboBoxItem::setText(const QString& txt)
{
    // look if combo item is present in combo box
    // and find its index
    int ndx=combo_.FindNdx(this);
    if( ndx != -1 )
        combo_.changeItem( txt , ndx);
    else
        szTmpTxt_=txt;
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBoxItem::text
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
QString ADN_ComboBoxItem::text() const
{
    // look if combo item is present in combo box
    // and find its index
    int ndx=combo_.FindNdx(this);
    if( ndx != -1 )
        return combo_.text( ndx );
    else
        return szTmpTxt_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBoxItem::setEnabled
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_ComboBoxItem::setEnabled(bool /*bEnable*/)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBoxItem::Warn
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
void ADN_ComboBoxItem::Warn( ADN_ErrorStatus, const QString& )
{
    // NOTHING
}
