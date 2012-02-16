//*****************************************************************************
//
// $Created: JDY 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TableItem_ComboBox.cpp $
// $Author: Ape $
// $Modtime: 28/01/05 11:40 $
// $Revision: 2 $
// $Workfile: ADN_TableItem_ComboBox.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_TableItem_ComboBox.h"

int ADN_TableItem_ComboBox::RTTI = 50;

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox constructor
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
ADN_TableItem_ComboBox::ADN_TableItem_ComboBox( ADN_Table *table ,void* data )
    : ADN_TableItem_ABC( table, data )
    , entries_( )
    , currentComboIndex_( 0 )
    , edit_( false )
    , vItems_()
{
    setReplaceable( false );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox destructor
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
ADN_TableItem_ComboBox::~ADN_TableItem_ComboBox()
{
    clear_owned_ptrs(vItems_);
    vItems_.clear();
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::setStringList
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::setStringList( const QStringList &l )
{
    entries_ = l;
    currentComboIndex_ = 0;
    table()->updateCell( row(), col() );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::insertItem
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::insertItem( const QString& txt, int index )
{
    // If no index is specified, find an index so that the combo is sorted alphabetically.
    if( index == -1 )
        for( index = 0; index < count(); ++index )
            if( txt.compare( text( index ) ) < 0 )
                break;

    // Insert index value in model at view place.
    vItemsEnum_.insert( vItemsEnum_.begin() + index, static_cast< int >( vItemsEnum_.size() ) );
    entries_.push_back( txt );

    // update table
    table()->updateCell( row(), col() );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::insertItem
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::insertItem( ADN_TableItem_ComboBoxItem* item, int index )
{
    if( !item )
        return;

    int nRealIndex = index;
    if( nRealIndex == -1 )
        for( nRealIndex = 0; nRealIndex < count(); ++nRealIndex )
            if( item->text().compare( text( nRealIndex ) ) < 0 )
                break;
    // insert string in combo
    insertItem( item->text(), nRealIndex );

    // insert item in list items
    vItems_.insert(vItems_.begin() + nRealIndex, item );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::changeItem
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::changeItem( const QString & t, int index )
{
    if( index == -1)
        return;

    int i=0;
    QStringList::iterator it=entries_.begin();
    for ( ;it!=entries_.end();++it ,++i)
        if( i == index )
            break;
    if( it == entries_.end() )
        return;
    else
        (*it)=t;
    table()->updateCell( row(), col() );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::removeItem
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::removeItem ( int index )
{
    // remove item from list
    ADN_TableItem_ComboBoxItem* item=vItems_[index];
    vItems_.erase(vItems_.begin()+index );
    delete item;

    // remove item from enum
    if( !vItemsEnum_.empty() )
        vItemsEnum_.erase( vItemsEnum_.begin() + index );

    // remove entry
    int i=0;
    QStringList::iterator it=entries_.begin();
    for ( ;it!=entries_.end();++it ,++i)
        if( i == index )
            break;
    if( it == entries_.end() )
        return;
    else
        entries_.erase(it);
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::clear
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::clear()
{
    // clear items
    clear_owned_ptrs(vItems_);
    vItems_.clear();
    vItemsEnum_.clear();

    // clear entries
    entries_.clear();
    currentComboIndex_ = 0;
    table()->updateCell( row(), col() );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::SetCurrentData
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::SetCurrentData( void* /*data*/ )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::createEditor
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
QWidget *ADN_TableItem_ComboBox::createEditor() const
{
    QComboBox* cb = new QComboBox( edit_, table()->viewport(), "qt_editor_cb" );

    for( std::vector< int >::const_iterator it = vItemsEnum_.begin(); it != vItemsEnum_.end(); ++it )
    {
        assert( *it >= 0 && *it < vItemsEnum_.size() );
        cb->insertItem( entries_[ *it ] );
    }

    cb->setCurrentItem( currentComboIndex_ );
    QObject::connect( cb, SIGNAL( activated( int ) ), table(), SLOT( doValueChanged() ) );
    return cb;
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::setContentFromEditor
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::setContentFromEditor( QWidget *w )
{
    if( w && w->inherits( "QComboBox" ) )
    {
        QComboBox *cb = (QComboBox*)w;
        currentComboIndex_ = cb->currentItem();
        QString str = entries_.at( GetEnumIndexFromGUI( currentComboIndex_ ) );
        setText( str );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::setCurrentItem
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::setCurrentItem( int index )
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

        currentComboIndex_ = newIndex;

        QWidget *w = table()->cellWidget( row(), col() );
        if( w && w->inherits( "QComboBox" ) )
        {
            ( (QComboBox*)w )->setCurrentItem( currentComboIndex_ );
            setText( ( (QComboBox*)w )->currentText() );
        }
        else
        {
            QString str = entries_.at( GetEnumIndexFromGUI( currentComboIndex_ ) );
            setText( str );
            table()->updateCell( row(), col() );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::setCurrentItem
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::setCurrentItem( const QString &s )
{
    assert( false );
    int i = entries_.findIndex( s );
    if( i != -1 )
        setCurrentItem( i );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::currentItem
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
int ADN_TableItem_ComboBox::currentItem() const
{
    QWidget *w = table()->cellWidget( row(), col() );
    if( w && w->inherits( "QComboBox" ) )
        return ( (QComboBox*)w )->currentItem();
    return currentComboIndex_;
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::currentText
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
QString ADN_TableItem_ComboBox::currentText() const
{
    QWidget *w = table()->cellWidget( row(), col() );
    if( w && w->inherits( "QComboBox" ) )
        return ( (QComboBox*)w )->currentText();
    return *entries_.at( GetEnumIndexFromGUI( currentComboIndex_ ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::count
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
int ADN_TableItem_ComboBox::count() const
{
    QWidget *w = table()->cellWidget( row(), col() );
    if( w && w->inherits( "QComboBox" ) )
        return ( (QComboBox*)w )->count();
    return static_cast< int >( entries_.count() );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::text
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
QString ADN_TableItem_ComboBox::text( int i ) const
{
    QWidget *w = table()->cellWidget( row(), col() );
    if( w && w->inherits( "QComboBox" ) )
        return ( (QComboBox*)w )->text( i );
    return entries_.at( GetEnumIndexFromGUI( i ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::setEditable
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::setEditable( bool b )
{
    edit_ = b;
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::isEditable
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
bool ADN_TableItem_ComboBox::isEditable() const
{
    return edit_;
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::rtti
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
int ADN_TableItem_ComboBox::rtti() const
{
    return RTTI;
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::DoValueChanged
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::DoValueChanged()
{
    static_cast< ADN_Connector_Combo_ABC* >( pConnector_ )->SetNdxChanged( GetEnumIndexFromGUI( currentItem() ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::FindNdx
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
int ADN_TableItem_ComboBox::FindNdx( const ADN_TableItem_ComboBoxItem* item ) const
{
    int ndx=0;
    for ( CIT_Ptr_TableItem_ComboBoxItem_Vector it=vItems_.begin();it!=vItems_.end();++it,++ndx)
        if( (*it)==item)
            return ndx;
    return -1;
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::FindNdx
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
int ADN_TableItem_ComboBox::FindNdx( void * data) const
{
    int ndx=0;
    for ( CIT_Ptr_TableItem_ComboBoxItem_Vector it=vItems_.begin();it!=vItems_.end();++it,++ndx)
        if( (*it) && (*it)->GetData() == data)
            return ndx;
    return -1;
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::GetItem
// Created: JDY 03-09-11
//-----------------------------------------------------------------------------
ADN_TableItem_ComboBoxItem* ADN_TableItem_ComboBox::GetItem( int ndx )
{
    if (ndx==-1 || ndx >=(int)vItems_.size() )
        return 0;
    return vItems_[ndx];
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::GetEnumIndexFromGUI
// Created: ABR 2012-02-16
// -----------------------------------------------------------------------------
int ADN_TableItem_ComboBox::GetEnumIndexFromGUI( int index ) const
{
    return ( vItemsEnum_.empty() ) ? index : vItemsEnum_.at( index );
}
