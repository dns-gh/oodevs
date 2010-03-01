// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ActiveProtections_Data.h"
#include "ADN_ActiveProtections_WeaponsTable.h"
#include "moc_ADN_ActiveProtections_WeaponsTable.cpp"

#include "ADN_Equipement_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Workspace.h"

#include <qpopmenu.h>

typedef ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons ActiveProtectionsInfosWeapons;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with ADN_ActiveProtections_WeaponsTable
//-----------------------------------------------------------------------------
class ADN_CT_ActiveProtections_WeaponsTable
 : public ADN_Connector_Table_ABC
{
public:

    ADN_CT_ActiveProtections_WeaponsTable( ADN_ActiveProtections_WeaponsTable& table ) 
        : ADN_Connector_Table_ABC( table, false )
        , weaponsTable_          ( table )
    {}

    void  AddSubItems( int nRow, void* pObj )
    {
        assert( pObj != 0 );
        ActiveProtectionsInfosWeapons* pWeapons = static_cast<ActiveProtectionsInfosWeapons*>( pObj );

        ADN_TableItem_String* pItemName = new ADN_TableItem_String( &tab_, pObj, QTableItem::Never );
        ADN_TableItem_Double* pItemOdds = new ADN_TableItem_Double( &tab_, pObj );

        // add a new row & set new values
        tab_.setItem( nRow, 0, pItemName );
        tab_.setItem( nRow, 1, pItemOdds );

        // set table item properties
        pItemName->setEnabled( false );
        pItemName->GetConnector().Connect( &pWeapons->strName_ );

        // set table item properties
        //pItemOdds->GetValidator().setRange( 0.0, 100.0, 2 );
        pItemOdds->GetConnector().Connect( &pWeapons->coefficient_ );

        weaponsTable_.OnModified();
    }

private:
    ADN_ActiveProtections_WeaponsTable& weaponsTable_;

    ADN_CT_ActiveProtections_WeaponsTable& operator=( const ADN_CT_ActiveProtections_WeaponsTable& );
};



//-----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_WeaponsTable constructor
// Created: FDS 2010-02-24
//-----------------------------------------------------------------------------
ADN_ActiveProtections_WeaponsTable::ADN_ActiveProtections_WeaponsTable( const std::string& strName, QWidget* pParent )
:   ADN_Table2( pParent, "ADN_ActiveProtections_WeaponsTable" )
{
    // peut etre selectionne & trie
    setSorting( true );
    setSelectionMode( QTable::Single );
    setShowGrid( false );

    setMinimumHeight( 150 );
    setMaximumHeight( 150 );

    // hide vertical header
    verticalHeader()->hide();
    setLeftMargin( 0 );

    // tab with 2 columns
    setNumCols( 2 );
    setNumRows( 0 );

    horizontalHeader()->setLabel( 0, strName.c_str() );
    horizontalHeader()->setLabel( 1, tr( "Coefficient" ) );

    // connector creation
    pConnector_ = new ADN_CT_ActiveProtections_WeaponsTable(*this);

    connect( this, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnModified() ) );
}


//-----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_WeaponsTable destructor
// Created: FDS 2010-02-24
//-----------------------------------------------------------------------------
ADN_ActiveProtections_WeaponsTable::~ADN_ActiveProtections_WeaponsTable()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_WeaponsTable::OnContextMenu
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_WeaponsTable::OnContextMenu( int , int , const QPoint& pt )
{
    QPopupMenu menu( this );
    QPopupMenu addMenu( &menu );

    ADN_Equipement_Data::T_CategoryInfos_Vector& pWeapon = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition ).categories_;
    for( ADN_Equipement_Data::IT_CategoryInfos_Vector it = pWeapon.begin(); it != pWeapon.end(); ++it )
    {       
        addMenu.insertItem( (*it)->strName_.GetData().c_str(), (int)(*it) );
    }
    ADN_Tools::SortMenu( addMenu );
    
    menu.insertItem( tr( "New" ), &addMenu );
    menu.insertItem( tr( "Delete" ), 1 );
    menu.setItemEnabled( 1, GetCurrentData() != 0 );

    int nMenuResult = menu.exec( pt );

    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 1 )
    {
        // Delete the current element.
        ActiveProtectionsInfosWeapons* pCurrent = (ActiveProtectionsInfosWeapons*)GetCurrentData();
        if( pCurrent != 0 )
            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrent );
    }
    else
    {
        // Create a new element
        ActiveProtectionsInfosWeapons* pNewInfo = new ActiveProtectionsInfosWeapons();
        pNewInfo->ptrWeapon_ = (ADN_Equipement_Data::CategoryInfo*)nMenuResult;;
        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        pCTable->AddItem( 0 );  
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_WeaponsTable::OnModified
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_WeaponsTable::OnModified()
{
}
