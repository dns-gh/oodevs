// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Table_Objects_LocationScore.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 12 $
// $Workfile: ADN_Table_Objects_LocationScore.cpp $
//
// *****************************************************************************


#include "adaptation_app_pch.h"
#include "ADN_Table_Objects_LocationScore.h"

#include "ADN_App.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Objects_Data.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Objects_Data::ScoreLocationInfos ScoreLocationInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with T_TargetInfos_Vector
//-----------------------------------------------------------------------------
class ADN_CT_ScoreLocation
:public ADN_Connector_Table_ABC
{
public:

    ADN_CT_ScoreLocation(ADN_Table_Objects_LocationScore& tab)
    : ADN_Connector_Table_ABC(tab,false)
    {}

    void  AddSubItems(int i,void *obj)
    {
        assert(obj);
        ADN_TableItem_String *pItemString=0;
        ADN_TableItem_Int    *pItemInt=0;

        // set new values
        tab_.setItem(i,0,pItemString=new ADN_TableItem_String(&tab_,obj));
        tab_.setItem(i,1,pItemInt=new ADN_TableItem_Int(&tab_,obj));

        // set items validators range
        pItemInt->GetValidator().setBottom( 0 );

        // disable object name
        pItemString->setEnabled(false);
        pItemString->SetAutoEnabled(false);

        // connect items & datas
        pItemString->setText( ADN_Tr::ConvertFromLocation( static_cast<ScoreLocationInfos*>(obj)->nLocation_.GetData(),ENT_Tr_ABC::eToTr).c_str());
        pItemInt->GetConnector().Connect(&static_cast<ScoreLocationInfos*>(obj)->nScore_);
    }

private:
    ADN_CT_ScoreLocation& operator=( const ADN_CT_ScoreLocation& );
};

// =============================================================================
//
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore constructor
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
ADN_Table_Objects_LocationScore::ADN_Table_Objects_LocationScore(QWidget * parent )
: ADN_Table2 ( parent, "ADN_Table_Objects_LocationScore" )
{
    SetIsEnabledOnlyInAdminMode( true );

    // peut etre selectionne & trie
    setSorting(true);
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid(false);
    setMinimumHeight( 205 );

    // hide vertical header
    verticalHeader()->hide();
    setLeftMargin(0);

    // tab with 2 columns
    setNumCols(2);
    setNumRows(0);
    setColumnStretchable(0,true);
    setColumnStretchable(1,true);

    horizontalHeader()->setLabel(0, tr( "Ground Type" ) );
    horizontalHeader()->setLabel(1, tr( "Weight" ) );

    // connector creation
    pConnector_ = new ADN_CT_ScoreLocation(*this);
}

// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore destructor
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
ADN_Table_Objects_LocationScore::~ADN_Table_Objects_LocationScore()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore::OnContextMenu
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
void ADN_Table_Objects_LocationScore::OnContextMenu(int /*row*/,int /*col*/,const QPoint& pt)
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( &popupMenu );

    // Get the list of the possible munitions
    for( int i = 0; i < eNbrLocation; ++i )
    {
        std::string strTerrainName = ADN_Tr::ConvertFromLocation( (E_Location)i, ENT_Tr_ABC::eToTr );
        if( Contains( strTerrainName ) )
            continue;

        addMenu.insertItem( strTerrainName.c_str(), 2 + i );
    }

    ADN_Tools::SortMenu( addMenu );
    popupMenu.insertItem( tr( "Add terrain" ), &addMenu , 0 );
    if( this->GetCurrentData() != 0 )
        popupMenu.insertItem( tr( "Remove terrain" ), 1 );

    int nResult = popupMenu.exec( pt );
    if( nResult == 1 )
    {
        RemoveCurrentElement();
    }
    else if( nResult > 1 )
    {
        assert( nResult - 2 < eNbrLocation );
        AddNewElement( nResult - 2 );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore::AddNewElement
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
void ADN_Table_Objects_LocationScore::AddNewElement( int n )
{
    // Get the list of the defined targets
    ScoreLocationInfos* pNewInfo = new ScoreLocationInfos();
    pNewInfo->nLocation_ = (E_Location)n;

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore::RemoveCurrentElement
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
void ADN_Table_Objects_LocationScore::RemoveCurrentElement()
{
    // delete composante
    ScoreLocationInfos* pCurComposante = (ScoreLocationInfos*)GetCurrentData();
    assert( pCurComposante != 0 );
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurComposante);
}


// -----------------------------------------------------------------------------
// Name: ADN_Table_Objects_LocationScore::Contains
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
bool ADN_Table_Objects_LocationScore::Contains( const std::string& strMunitionName )
{
    int i = 0;
    while( text( i, 0 ) != QString::null )
    {
        if( text( i, 0 ) == strMunitionName.c_str() )
            return true;
        ++i;
    }
    return false;
}
