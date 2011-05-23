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
#include "ADN_Workspace.h"
#include "ADN_Enums.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"
#include "ADN_Connector_Combo_ABC.h"

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox constructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_ComboBox::ADN_ComboBox( QWidget* parent, const char* name )
    : QComboBox  ( parent, name )
    , ADN_Gfx_ABC()
    , pCurData_  ( 0 )
    , fromView_  ( false )
{
    connect( this, SIGNAL( activated( int ) ), this, SLOT( ItemSelected( int ) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL( OpenModeToggled() ), this, SLOT( UpdateEnableState() ) );
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

inline void SetAutoClear(T_ConnectorVector& v,bool b)
{
    for ( T_ConnectorVector::iterator itConnector=v.begin();itConnector!=v.end();++itConnector)
        (*itConnector)->SetAutoClear(b);
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::SetCurrentData
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_ComboBox::SetCurrentData( void* data )
{
    if( !data )
        SetAutoClear( vItemConnectors_, true );

    DisconnectItem();

    pCurData_ = data;

    ConnectItem();

    if( !data )
        SetAutoClear( vItemConnectors_, false );
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
    pCurData_ = static_cast< ADN_Connector_Combo_ABC* >( pConnector_ )->SetNdxChanged( ndx );
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
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::removeItem
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_ComboBox::removeItem ( int index )
{
    ADN_ComboBoxItem* item = vItems_[ index ];
    vItems_.erase( vItems_.begin() + index );
    if( !vItemsEnum_.empty() )
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
    int ndx = 0;
    for( CIT_PtrComboBoxItem_Vector it = vItems_.begin(); it != vItems_.end(); ++it, ++ndx )
        if( ( *it ) == item )
            return ndx;
    return -1;
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::FindNdx
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
int ADN_ComboBox::FindNdx( void* data) const
{
    int ndx = 0;
    for( CIT_PtrComboBoxItem_Vector it = vItems_.begin(); it != vItems_.end(); ++it, ++ndx )
        if( ( *it ) && ( *it )->GetData() == data )
            return ndx;
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
// Name: ADN_ComboBox::setEnabled
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_ComboBox::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        QComboBox::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        QComboBox::setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_ComboBox::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_Combo_ABC* >( pConnector_ )->IsConnected() );
}
