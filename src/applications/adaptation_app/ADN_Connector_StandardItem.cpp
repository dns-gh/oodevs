// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Connector_StandardItem.h"
#include "ADN_Tools.h"
#include "clients_gui/Roles.h"

// -----------------------------------------------------------------------------
// Name: ADN_Connector_StandardItem::ADN_Connector_StandardItem
// Created: ABR 2012-10-30
// -----------------------------------------------------------------------------
ADN_Connector_StandardItem::ADN_Connector_StandardItem( ADN_StandardItem& item, ADN_StandardItem::E_Type type, const QStringList* enumContent /* = 0 */ )
    : ADN_Connector_ABC()
    , item_( item )
    , type_( type )
    , enumContent_( enumContent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_StandardItem::~ADN_Connector_StandardItem
// Created: ABR 2012-10-30
// -----------------------------------------------------------------------------
ADN_Connector_StandardItem::~ADN_Connector_StandardItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_StandardItem::SetDataPrivate
// Created: ABR 2012-10-30
// -----------------------------------------------------------------------------
void ADN_Connector_StandardItem::SetDataPrivate( void* data )
{
    static QLocale locale;
    if( !data )
        return;
    switch( type_ )
    {
    case ADN_StandardItem::eInt:
        {
            int* value = static_cast< int* >( data );
            item_.setData( locale.toString( *value ), Qt::EditRole );
            item_.setData( *value, gui::Roles::DataRole );
            break;
        }
    case ADN_StandardItem::eDouble:
        {
            double* value = static_cast< double* >( data );
            item_.setData( locale.toString( *value ), Qt::EditRole );
            item_.setData( *value, gui::Roles::DataRole );
            break;
        }
    case ADN_StandardItem::eColor:
        {
            QString text = static_cast< std::string* >( data )->c_str();
            QColor color( "#" + text.right( 6 ) );
            if( color.isValid() )
            {
                const QBrush brush( color );
                item_.setBackground( brush );
                item_.setData( brush, gui::Roles::OtherRole );
            }
            item_.setData( text, gui::Roles::DataRole );
            break;
        }
    case ADN_StandardItem::eDelay:
        {
            std::string* text = static_cast< std::string* >( data );
            item_.setData( ADN_Tools::ConvertDelayToLongString( text->c_str() ), Qt::EditRole );
            item_.setData( ADN_Tools::ConvertDelayToCentiseconds( text->c_str() ), gui::Roles::DataRole );
            break;
        }
    case ADN_StandardItem::eTime:
        {
            const QString time = ADN_Tools::ConvertXmlTimeToLocalTime( static_cast< std::string* >( data )->c_str() );
            item_.setData( time, Qt::EditRole );
            item_.setData( time, gui::Roles::DataRole );
            break;
        }
    case ADN_StandardItem::eString:
    case ADN_StandardItem::eLocalizedString:
        {
            std::string* text = static_cast< std::string* >( data );
            item_.setData( text->c_str(), Qt::EditRole );
            item_.setData( text->c_str(), gui::Roles::DataRole );
            break;
        }
    case ADN_StandardItem::eEnum:
        {
            assert( enumContent_ );
            int* value = static_cast< int* >( data );
            assert( *value < enumContent_->size() );
            item_.setData( enumContent_->at( *value ), Qt::EditRole );
            item_.setData( *value, gui::Roles::DataRole );
            break;
        }
    case ADN_StandardItem::eBool:
        {
            bool* value = static_cast< bool* >( data );
            assert( item_.isCheckable() );
            item_.setCheckState( ( *value ) ? Qt::Checked : Qt::Unchecked );
            break;
        }
    case ADN_StandardItem::ePtrInVector:
    default:
        assert( 0 );
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_StandardItem::SetDataChanged
// Created: ABR 2012-10-30
// -----------------------------------------------------------------------------
void ADN_Connector_StandardItem::SetDataChanged( const QString& text )
{
    switch( type_ )
    {
    case ADN_StandardItem::eInt:
        {
            int newval = text.toInt();
            emit DataChanged( &newval );
            break;
        }
    case ADN_StandardItem::eDouble:
        {
            double newval = text.toDouble();
            emit DataChanged( &newval );
            break;
        }
    case ADN_StandardItem::eDelay:
        {
            std::string newval = ADN_Tools::ConvertLongStringToDelay( text );
            emit DataChanged( &newval );
            break;
        }
    case ADN_StandardItem::eTime:
    case ADN_StandardItem::eColor:
    case ADN_StandardItem::eString:
    case ADN_StandardItem::eLocalizedString:
        {
            std::string newval = text.toStdString();
            emit DataChanged( &newval );
            break;
        }
    case ADN_StandardItem::eEnum:
        {
            assert( enumContent_ );
            int index = enumContent_->indexOf( text );
            emit DataChanged( &index );
            break;
        }
    case ADN_StandardItem::eBool:
        {
            assert( item_.isCheckable() && text.isEmpty() );
            bool checked = item_.checkState() == Qt::Checked;
            emit DataChanged( &checked );
            break;
        }
    case ADN_StandardItem::ePtrInVector:
    default:
        assert( 0 );
        break;
    }
}
