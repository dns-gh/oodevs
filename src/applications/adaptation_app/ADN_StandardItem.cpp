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

    virtual void Rename( void* data )
    {
        if( !data )
            return;
        const std::string& str = *static_cast< std::string* >( data );
        item_.setData( str.c_str(), Qt::EditRole );
        item_.setData( str.c_str(), gui::Roles::DataRole );
    }

private:
    ADN_StandardItem& item_;
};

}

// -----------------------------------------------------------------------------
// Name: ADN_StandardItem::ADN_StandardItem
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
ADN_StandardItem::ADN_StandardItem( void* parentData, E_Type type /* = eString */ )
    : parentData_( parentData )
    , type_( type )
    , connector_( 0 )
{
    setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

    setData( QVariant( gui::StandardModel::showValue_ ), gui::Roles::FilterRole );
    setData( QVariant(), gui::Roles::DataRole );
    setData( QVariant(), gui::Roles::SafeRole );
    setData( QVariant(), gui::Roles::MimeTypeRole );
    setData( QBrush( Qt::transparent ), gui::Roles::OtherRole );
}

// -----------------------------------------------------------------------------
// Name: ADN_StandardItem::~ADN_StandardItem
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
ADN_StandardItem::~ADN_StandardItem()
{
    parentData_ = 0;
    delete connector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_StandardItem::GetData
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
void* ADN_StandardItem::GetData() const
{
    return parentData_;
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

// -----------------------------------------------------------------------------
// Name: ADN_StandardItem::Warn
// Created: ABR 2013-09-18
// -----------------------------------------------------------------------------
void ADN_StandardItem::Warn( ADN_ErrorStatus elementStatus )
{
    QBrush brush = data( gui::Roles::OtherRole ).value< QBrush >();
    switch( elementStatus )
    {
    case eWarning:
        brush = brush == Qt::lightGray ? Qt::darkYellow : Qt::yellow;
        break;
    case eError:
        brush = brush == Qt::lightGray ? Qt::darkRed : Qt::red;
        break;
    case eNoError:
    default:
        break;
    }
    setBackground( brush );
}
