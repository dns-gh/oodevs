//*****************************************************************************
//
// $Created: JDY 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ComboBox.cpp $
// $Author: Nld $
// $Modtime: 1/06/05 11:15 $
// $Revision: 7 $
// $Workfile: ADN_ComboBox.cpp $
//
//*****************************************************************************
#include "ADN_Pch.h"
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
ADN_ComboBox::ADN_ComboBox(QWidget * parent, const char * name)
:   QComboBox(parent,name)
,   ADN_Gfx_ABC()
,   pCurData_(0)
,   vItems_()
{
    connect(this,SIGNAL(activated(int)),this,SLOT(ItemSelected(int)));
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox destructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_ComboBox::~ADN_ComboBox()
{
    disconnect(this,SIGNAL(activated(int)),this,SLOT(ItemSelected(int)));
    pCurData_=0;
    
    MT_DELETEOWNED(vItems_);
    vItems_.clear();
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::ConnectItem
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
void ADN_ComboBox::ConnectItem()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::DisconnectItem
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
void ADN_ComboBox::DisconnectItem()
{
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
void ADN_ComboBox::SetCurrentData(void *data)
{
    if ( !data )
        SetAutoClear(vItemConnectors_,true);
    
    DisconnectItem();

    pCurData_=data;

    ConnectItem();

    if ( !data )
        SetAutoClear(vItemConnectors_,false);
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::setCurrentItem
// Created: JDY 03-08-21
//-----------------------------------------------------------------------------
void ADN_ComboBox::setCurrentItem( int index )
{
    if (index == -1)
        SetCurrentData(0);
    else
    {
        QComboBox::setCurrentItem(index);
        emit activated(index);
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::ItemSelected
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
void ADN_ComboBox::ItemSelected( int ndx )
{
    DisconnectItem();

    pCurData_=static_cast<ADN_Connector_Combo_ABC*>(pConnector_)->SetNdxChanged(ndx);

    emit Activated( ndx );

    ConnectItem();
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::insertItem 
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_ComboBox::insertItem ( const QString& txt, int index )
{
    // If no index is specified, find an index so that the combo is sorted alphabeticaly.
    /*
    if( index == -1 )
    {
        index = 0;
        for( int n = 0; n < this->count(); ++n )
        {
            if( txt.compare( this->text( n ) ) < 0 )
                break;
            index++;
        }
    }*/

    // Insert string in combo
    QComboBox::insertItem( txt , index );
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::insertItem 
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_ComboBox::insertItem ( ADN_ComboBoxItem* item, int index )
{
    if ( !item )
        return;

    int nRealIndex = index;
    if( nRealIndex == -1 )
        nRealIndex = 0;

/*
    if( nRealIndex == -1 )
    {
        nRealIndex = 0;

        for( int n = 0; n < this->count(); ++n )
        {
            if( item->text().compare( this->text( n ) ) < 0 )
                break;
            nRealIndex++;
        }
    }
    */

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
    ADN_ComboBoxItem* item=vItems_[index];
    vItems_.erase(vItems_.begin()+index );
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
    MT_DELETEOWNED(vItems_);
    vItems_.clear();

    // clear combo
    QComboBox::clear();
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::FindNdx
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
int ADN_ComboBox::FindNdx( const ADN_ComboBoxItem* item ) const
{
    int ndx=0;
    for ( CIT_PtrComboBoxItem_Vector it=vItems_.begin();it!=vItems_.end();++it,++ndx)
        if ( (*it)==item)
            return ndx;
    return -1;
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::FindNdx
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
int ADN_ComboBox::FindNdx( void * data) const
{
    int ndx=0;
    for ( CIT_PtrComboBoxItem_Vector it=vItems_.begin();it!=vItems_.end();++it,++ndx)
        if ( (*it) && (*it)->GetData() == data)
            return ndx;
    return -1;
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::GetItem
// Created: JDY 03-09-11
//-----------------------------------------------------------------------------
ADN_ComboBoxItem* ADN_ComboBox::GetItem( int ndx )
{
    if (ndx==-1 || ndx >=(int)vItems_.size() )
        return 0;
    return vItems_[ndx];
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
