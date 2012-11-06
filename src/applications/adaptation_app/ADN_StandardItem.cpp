// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_StandardItem.h"

#include "ADN_Connector_StandardItem.h"
#include "clients_gui/Roles.h"
#include "clients_gui/StandardModel.h"

#include "ADN_RefWithName.h"

namespace
{

class ADN_Connector_StandardItemVector : public ADN_Connector_Vector_ABC
{
public:
    ADN_Connector_StandardItemVector( ADN_StandardItem& item )
        : ADN_Connector_Vector_ABC()
        , item_( item )
    {
        // NOTHING
    }

    virtual ~ADN_Connector_StandardItemVector()
    {
        // NOTHING
    }

    virtual void SetDataPrivate( void* data )
    {
        if( !data )
            return;
        ADN_RefWithName* ref = static_cast< ADN_RefWithName* >( data );
        if( ref )
        {
            item_.setData( ref->strName_.GetData().c_str(), Qt::EditRole );
            item_.setData( ref->strName_.GetData().c_str(), gui::Roles::DataRole );
        }
    }

    // $$$$ ABR 2012-11-06: TODO: Handle item renaming

private:
    ADN_StandardItem& item_;
};

}

// -----------------------------------------------------------------------------
// Name: ADN_StandardItem::ADN_StandardItem
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
ADN_StandardItem::ADN_StandardItem( void* data, E_Type type /* = eString */ )
    : pData_( data )
    , type_( type )
    , connector_( 0 )
{
    setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

    setData( *new QVariant( gui::StandardModel::showValue_ ), gui::Roles::FilterRole );
    setData( *new QVariant(), gui::Roles::DataRole );
    setData( *new QVariant(), gui::Roles::SafeRole );
    setData( *new QVariant(), gui::Roles::MimeTypeRole );
}

// -----------------------------------------------------------------------------
// Name: ADN_StandardItem::~ADN_StandardItem
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
ADN_StandardItem::~ADN_StandardItem()
{
    pData_ = 0;
    delete connector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_StandardItem::GetData
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
void* ADN_StandardItem::GetData() const
{
    return pData_;
}

// -----------------------------------------------------------------------------
// Name: ADN_StandardItem::GetConnector
// Created: ABR 2012-10-30
// -----------------------------------------------------------------------------
ADN_Connector_ABC* ADN_StandardItem::GetConnector() const
{
    return connector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_StandardItem::GetType
// Created: ABR 2012-10-30
// -----------------------------------------------------------------------------
ADN_StandardItem::E_Type ADN_StandardItem::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: ADN_StandardItem::Connect
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
void ADN_StandardItem::Connect( ADN_Connector_ABC* data, const QStringList* enumContent /* = 0 */ )
{
    // create new internal connector
    assert( connector_ == 0 );

    if( type_ == ePtrInVector )
        connector_ = new ADN_Connector_StandardItemVector( *this );
    else
        connector_ = new ADN_Connector_StandardItem( *this, type_, enumContent );

    assert( connector_ );

    // connect internal connector and external data
    connector_->Connect( data );
}
