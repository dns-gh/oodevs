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

QComboBox *ADN_TableItem_ComboBox::fakeCombo = 0;
QWidget *fakeComboWidget = 0;

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox constructor
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
ADN_TableItem_ComboBox::ADN_TableItem_ComboBox( ADN_Table *table ,void* data )
: ADN_TableItem_ABC( table, data )
, entries( )
, current( 0 )
, edit( false )
, vItems_()
{
    setReplaceable( false );

    if( !fakeCombo )
    {
        fakeComboWidget = new QWidget( 0, 0 );
        fakeCombo = new QComboBox( false, fakeComboWidget, 0 );
        fakeCombo->hide();
    }
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
    entries = l;
    current = 0;
    table()->updateCell( row(), col() );
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::insertItem
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::insertItem ( const QString & t, int index )
{
    if( index == -1 )
        entries.push_back(t);
    else
    {
        int i=0;
        QStringList::iterator it=entries.begin();
        for ( ;it!=entries.end();++it ,++i)
            if( i == index )
                break;
        if( it == entries.end() )
            entries.push_back(t);
        else
            entries.insert(it,t);
    }
    // $$$$$ Joss pq on reset la selection???
    //current = 0;
    table()->updateCell( row(), col() );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::insertItem
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::insertItem ( ADN_TableItem_ComboBoxItem* item, int index )
{
    if( !item )
        return;

    // insert string in combo
    insertItem( item->text() , index );

    // insert item in vector
    if( index == -1)
        vItems_.push_back(item);
    else
        vItems_.insert(vItems_.begin()+index,item);


}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::changeItem
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::changeItem ( const QString & t, int index )
{
    if( index == -1)
        return;
    int i=0;
    QStringList::iterator it=entries.begin();
    for ( ;it!=entries.end();++it ,++i)
        if( i == index )
            break;
    if( it == entries.end() )
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

    // remove entrie
    int i=0;
    QStringList::iterator it=entries.begin();
    for ( ;it!=entries.end();++it ,++i)
        if( i == index )
            break;
    if( it == entries.end() )
        return;
    else
        entries.erase(it);
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

    // clear entries
    entries.clear();
    current = 0;
    table()->updateCell( row(), col() );
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::SetCurrentData
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::SetCurrentData( void* /*data*/ )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::createEditor
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
QWidget *ADN_TableItem_ComboBox::createEditor() const
{
    // create an editor - a combobox in our case
    ( (ADN_TableItem_ComboBox*)this )->cb = new QComboBox( edit, table()->viewport(), "qt_editor_cb" );
    cb->insertStringList( entries );
    cb->setCurrentItem( current );
    QObject::connect( cb, SIGNAL( activated( int ) ), table(), SLOT( doValueChanged() ) );
    return cb;
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::setContentFromEditor
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::setContentFromEditor( QWidget *w )
{
    if( w->inherits( "QComboBox" ) )
    {
        QComboBox *cb = (QComboBox*)w;
        entries.clear();
        for ( int i = 0; i < cb->count(); ++i )
            entries << cb->text( i );
        current = cb->currentItem();
        QString str = entries.at( current );
        setText( str );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::setCurrentItem
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::setCurrentItem( int i )
{
    QWidget *w = table()->cellWidget( row(), col() );
    if( w && w->inherits( "QComboBox" ) ) {
    ( (QComboBox*)w )->setCurrentItem( i );
    current = i;
    setText( ( (QComboBox*)w )->currentText() );
    } else {
    current = i;
    QString str = entries.at( i );
    setText( str );
    table()->updateCell( row(), col() );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::setCurrentItem
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::setCurrentItem( const QString &s )
{
    int i = entries.findIndex( s );
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
    return current;
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
    return *entries.at( current );
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
    return (int)entries.count();    //### size_t/int cast
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
    return *entries.at( i );
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::setEditable
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBox::setEditable( bool b )
{
    edit = b;
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBox::isEditable
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
bool ADN_TableItem_ComboBox::isEditable() const
{
    return edit;
}


int ADN_TableItem_ComboBox::RTTI = 50;

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
    static_cast<ADN_Connector_Combo_ABC*>(pConnector_)->SetNdxChanged(currentItem());
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