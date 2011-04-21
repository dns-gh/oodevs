// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// ADN_ComboBox_Symbols
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Symbols constructor
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBox_Symbols< T >::ADN_ComboBox_Symbols( QWidget* pParent /*= 0*/, const char* szName /*= 0*/ )
    : ADN_ComboBox( pParent, szName )
{
    pConnector_ = new ADN_ComboBox_Symbols_Connector< T >( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Symbols destructor
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBox_Symbols< T >::~ADN_ComboBox_Symbols()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Symbols::insertItem
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline void ADN_ComboBox_Symbols< T >::insertItem( ADN_ComboBoxItem* item, int index /*= -1*/ )
{
    if( !item )
        return;
    int nRealIndex = index;
    if( nRealIndex == -1 )
        for( nRealIndex = 0; nRealIndex < this->count(); ++nRealIndex )
            if( item->text().compare( this->text( nRealIndex ) ) < 0 )
                break;
    T* symbol = static_cast< T* >( item->GetData() );
    QComboBox::insertItem( symbol->GetPixmap(), item->text(), nRealIndex );
    vItems_.insert(vItems_.begin() + nRealIndex, item );
}


// -----------------------------------------------------------------------------
// ADN_ComboBox_Symbols_Connector
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Symbols_Connector constructor
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBox_Symbols_Connector< T >::ADN_ComboBox_Symbols_Connector( ADN_ComboBox_Symbols< T >& combo )
    : ADN_Connector_ComboBox( &combo )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Symbols_Connector destructor
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBox_Symbols_Connector< T >::~ADN_ComboBox_Symbols_Connector()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Symbols_Connector::CreateItem
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBoxItem* ADN_ComboBox_Symbols_Connector< T >::CreateItem( void * obj )
{
    ADN_ComboBoxItem* pItem = new ADN_ComboBoxSymbolItem< T >( *pCombo_, obj);
    pItem->GetConnector().Connect( &( static_cast< T* > (obj) )->strName_ );
    return pItem;
};


// -----------------------------------------------------------------------------
// ADN_ComboBoxSymbolItem
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_ComboBoxSymbolItem::ADN_ComboBoxSymbolItem
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBoxSymbolItem< T >::ADN_ComboBoxSymbolItem( ADN_ComboBox& combo, void *data )
    : ADN_ComboBoxItem( combo, data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBoxSymbolItem::~ADN_ComboBoxSymbolItem
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBoxSymbolItem< T >::~ADN_ComboBoxSymbolItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBoxSymbolItem::setText
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline void ADN_ComboBoxSymbolItem< T >::setText(const QString& txt)
{
    int ndx = combo_.FindNdx( this );
    if( ndx != -1 )
    {
        T* symbol = static_cast< T* > ( pData_ );
        combo_.changeItem( symbol->GetPixmap(), txt, ndx );
    }
    else
        szTmpTxt_ = txt;
}
