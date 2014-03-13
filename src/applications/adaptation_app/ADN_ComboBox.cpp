// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ComboBox.h"
#include "moc_ADN_ComboBox.cpp"
#include "ADN_ComboBoxItem.h"
#include "ADN_Connector_Combo_ABC.h"
#include "ADN_Workspace.h"
#include "ADN_Tools.h"

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox constructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_ComboBox::ADN_ComboBox( QWidget* parent )
    : QComboBox  ( parent )
    , ADN_Gfx_ABC()
    , pCurData_  ( 0 )
    , fromView_  ( false )
{
    connect( this, SIGNAL( activated( int ) ), this, SLOT( ItemSelected( int ) ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox destructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_ComboBox::~ADN_ComboBox()
{
    disconnect( this, SIGNAL( activated( int ) ), this, SLOT( ItemSelected( int ) ) );
    pCurData_ = 0;

    clear_owned_ptrs( vItems_ );
    vItems_.clear();
    vItemsEnum_.clear();
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::ConnectItem
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
void ADN_ComboBox::ConnectItem()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::DisconnectItem
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
void ADN_ComboBox::DisconnectItem()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::SetCurrentData
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_ComboBox::SetCurrentData( void* data )
{
    if( !data )
        ADN_Tools::SetAutoClear( vItemConnectors_, true );

    DisconnectItem();

    pCurData_ = data;

    ConnectItem();

    if( !data )
        ADN_Tools::SetAutoClear( vItemConnectors_, false );
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::setCurrentItem
// Created: JDY 03-08-21
//-----------------------------------------------------------------------------
void ADN_ComboBox::setCurrentItem( int index )
{
    if( index == -1 )
        SetCurrentData( 0 );
    else
    {
        int newIndex = 0;
        if( vItemsEnum_.empty() )
            newIndex = index;
        else
            for( std::vector< int >::const_iterator it = vItemsEnum_.begin(); it != vItemsEnum_.end(); ++it, ++newIndex )
                if( *it == index )
                    break;
        fromView_ = true;
        QComboBox::setCurrentItem( newIndex );
        emit activated( newIndex );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::ItemSelected
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
void ADN_ComboBox::ItemSelected( int ndx )
{
    DisconnectItem();
    if( !fromView_ && !vItemsEnum_.empty() )
        ndx = vItemsEnum_.at( ndx );
    fromView_ = false;
    if( pConnector_.get() )
    {
        if( ADN_Connector_Combo_ABC* connectorCombo = dynamic_cast< ADN_Connector_Combo_ABC* >( pConnector_.get() ) )
            pCurData_ = connectorCombo->SetNdxChanged( ndx );
    }
    else
        pCurData_ = 0;
    ConnectItem();
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::insertItem
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_ComboBox::insertItem( const QString& txt, int index )
{
   // If no index is specified, find an index so that the combo is sorted alphabetically.
   if( index == -1 )
        for( index = 0; index < this->count(); ++index )
            if( txt.compare( this->text( index ) ) < 0 )
                break;
    // Insert index value in model at view place.
    vItemsEnum_.insert( vItemsEnum_.begin() + index, static_cast< int >( vItemsEnum_.size() ) );
    QComboBox::insertItem( txt, index );
    if( !pCurData_ )
        ItemSelected( index >= 0 ? index : 0 );
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::insertItem
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_ComboBox::insertItem( ADN_ComboBoxItem* item, int index )
{
    if( !item )
        return;

    int nRealIndex = index;
    if( nRealIndex == -1 )
        for( nRealIndex = 0; nRealIndex < this->count(); ++nRealIndex )
            if( item->text().compare( this->text( nRealIndex ) ) < 0 )
                break;
    // insert string in combo
    QComboBox::insertItem( item->text(), nRealIndex );

    // insert item in list items
    vItems_.insert(vItems_.begin() + nRealIndex, item );

    if( !pCurData_ )
        ItemSelected( nRealIndex >= 0 ? nRealIndex : 0 );
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::removeItem
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_ComboBox::removeItem ( int index )
{
    ADN_ComboBoxItem* item = 0;
    if( index < static_cast< int >( vItems_.size() ) )
    {
        item = vItems_[ index ];
        vItems_.erase( vItems_.begin() + index );
    }
    if( index < static_cast< int >( vItemsEnum_.size() ) )
        vItemsEnum_.erase( vItemsEnum_.begin() + index );
    delete item;
    QComboBox::removeItem(index);
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::clear
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_ComboBox::clear()
{
    // clear list of items
    clear_owned_ptrs(vItems_);
    vItems_.clear();
    vItemsEnum_.clear();
    QComboBox::clear();
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::FindNdx
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
int ADN_ComboBox::FindNdx( const ADN_ComboBoxItem* item ) const
{
    const std::size_t s = vItems_.size();
    for( register std::size_t i = 0; i < s; ++i )
        if( vItems_[ i ] == item )
            return static_cast< int >( i );
    return -1;
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::FindNdx
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
int ADN_ComboBox::FindNdx( void* data) const
{
    const std::size_t s = vItems_.size();
    for( register std::size_t i = 0; i < s; ++i )
        if( vItems_[ i ] && vItems_[ i ]->GetData() == data )
            return static_cast< int >( i );
    return -1;
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::GetItem
// Created: JDY 03-09-11
//-----------------------------------------------------------------------------
ADN_ComboBoxItem* ADN_ComboBox::GetItem( int ndx )
{
    if( ndx == -1 || ndx >= static_cast< int >( vItems_.size() ) )
        return 0;
    return vItems_[ ndx ];
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox::wheelEvent
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
void ADN_ComboBox::wheelEvent( QWheelEvent* event )
{
    event->ignore();
}
