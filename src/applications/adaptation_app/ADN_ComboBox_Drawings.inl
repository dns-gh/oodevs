// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// ADN_ComboBox_Drawings
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Drawings constructor
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBox_Drawings< T >::ADN_ComboBox_Drawings( QWidget* pParent /*= 0*/, const char* szName /*= 0*/ )
    : ADN_ComboBox( pParent, szName )
{
    pConnector_ = new ADN_ComboBox_Drawings_Connector< T >( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Drawings destructor
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBox_Drawings< T >::~ADN_ComboBox_Drawings()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Drawings::insertItem
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline void ADN_ComboBox_Drawings< T >::insertItem( ADN_ComboBoxItem* item, int index /*= -1*/ )
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
// ADN_ComboBox_Drawings_Connector
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Drawings_Connector constructor
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBox_Drawings_Connector< T >::ADN_ComboBox_Drawings_Connector( ADN_ComboBox_Drawings< T >& combo )
    : ADN_Connector_ComboBox( &combo )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Drawings_Connector destructor
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBox_Drawings_Connector< T >::~ADN_ComboBox_Drawings_Connector()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Drawings_Connector::CreateItem
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBoxItem* ADN_ComboBox_Drawings_Connector< T >::CreateItem( void * obj )
{
    ADN_ComboBoxItem* pItem = new ADN_ComboBoxDrawingItem< T >( *pCombo_, obj);
    pItem->GetConnector().Connect( &( static_cast< T* > (obj) )->strName_ );
    return pItem;
};


// -----------------------------------------------------------------------------
// ADN_ComboBoxDrawingItem
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_ComboBoxDrawingItem::ADN_ComboBoxDrawingItem
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBoxDrawingItem< T >::ADN_ComboBoxDrawingItem( ADN_ComboBox& combo, void *data )
    : ADN_ComboBoxItem( combo, data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBoxDrawingItem::~ADN_ComboBoxDrawingItem
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline ADN_ComboBoxDrawingItem< T >::~ADN_ComboBoxDrawingItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBoxDrawingItem::setText
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
template< typename T >
inline void ADN_ComboBoxDrawingItem< T >::setText(const QString& txt)
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
