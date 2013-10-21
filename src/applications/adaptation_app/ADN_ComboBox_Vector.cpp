// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_ComboBoxItem.h"
#include "ADN_RefWithName.h"

namespace
{

class ADN_CBV_Connector : public ADN_Connector_ComboBox
{
public:
    explicit ADN_CBV_Connector( ADN_ComboBox_Vector& combo )
        : ADN_Connector_ComboBox( &combo )
    {}

    virtual ~ADN_CBV_Connector()
    {}

    virtual ADN_ComboBoxItem* CreateItem( void * obj )
    {
        // Create the new combo item.
        ADN_ComboBoxItem* pItem = new ADN_ComboBoxItem( *pCombo_, obj );

        // Connect it with the object's name.
        pItem->GetConnector().Connect( &( static_cast< ADN_RefWithName* >( obj ) )->strName_ );

        return pItem;
    };
};

}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Vector constructor
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_ComboBox_Vector::ADN_ComboBox_Vector( QWidget* pParent )
    : ADN_ComboBox( pParent )
{
    pConnector_.reset( new ADN_CBV_Connector( *this ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Vector destructor
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_ComboBox_Vector::~ADN_ComboBox_Vector()
{
    // NOTHING
}
