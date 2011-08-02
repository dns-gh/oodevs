//*****************************************************************************
//
// $Created: AGN 03-07-30 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Point_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:55 $
// $Revision: 9 $
// $Workfile: ADN_Point_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Point_GUI.h"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Units_Data.h"
#include "ADN_Tr.h"

typedef ADN_Units_Data::PointInfos PointInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with ADN_Point_GUI
//-----------------------------------------------------------------------------
class ADN_CT_PointDistances
:public ADN_Connector_Table_ABC
{
public:

    ADN_CT_PointDistances(ADN_Point_GUI& tab)
    : ADN_Connector_Table_ABC(tab,false)
    {}

    void  AddSubItems(int i,void *obj)
    {

        assert(obj);
        ADN_TableItem_String *pItemString=0;
        ADN_TableItem_Int    *pItemInt=0;

        // add a new row & set new values
        tab_.setItem(i,0,pItemString=new ADN_TableItem_String(&tab_,obj));
        tab_.setItem(i,1,pItemInt=new ADN_TableItem_Int(&tab_,obj));
        pItemInt->GetValidator().setBottom( 0 );

        // set table item properties
        pItemString->setEnabled(false);
        pItemString->setText(ADN_Tr::ConvertFromKeyPoint(static_cast<PointInfos*>(obj)->nTypeTerrain_, ADN_Tr::eToApp ).c_str());

        // connect items & datas
        pItemInt->GetConnector().Connect(&static_cast<PointInfos*>(obj)->nDistance_);
    }


private:
    ADN_CT_PointDistances& operator=( const ADN_CT_PointDistances& );
};


//-----------------------------------------------------------------------------
// Name: ADN_Point_GUI constructor
// Created: AGN 03-07-30
//-----------------------------------------------------------------------------
ADN_Point_GUI::ADN_Point_GUI(QWidget* pParent )
: ADN_Table2( pParent, "ADN_Point_GUI" )
{
    // Selection and sorting.
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );

    this->setMaximumHeight( 300 );

    // Hide the vertical header.
    verticalHeader()->hide();

    // Setup 2 columns.
    setNumCols( 2 );
    setNumRows( 0 );

    horizontalHeader()->setLabel(0, tr( "Ground Type"));
    horizontalHeader()->setLabel(1, tr( "Distance (m)"));

    // Create the connector.
    pConnector_ = new ADN_CT_PointDistances(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_Point_GUI destructor
// Created: AGN 03-07-30
//-----------------------------------------------------------------------------
ADN_Point_GUI::~ADN_Point_GUI()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_Point_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_Point_GUI::OnContextMenu( int /*nRow*/, int /*nCol*/, const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu subMenu( &menu );

    menu.insertItem( tr( "Add point" ), &subMenu );
    menu.insertItem( tr( "Remove point" ), 100 );

    for( int n = 0; n < eNbrKeyPoints; ++n )
    {
        if( !Contains( (E_KeyPoint)n ) )
            subMenu.insertItem( ADN_Tr::ConvertFromKeyPoint( (E_KeyPoint)n ).c_str(), n );
    }

    int nMenuResult = menu.exec( pt );
    if( nMenuResult == 100 )
        DeleteCurrentElement();
    else if( nMenuResult >= 0 )
        AddNewElement( (E_KeyPoint)nMenuResult );
}


// -----------------------------------------------------------------------------
// Name: ADN_Point_GUI::AddNewElement
// Created: AGN 2003-12-08
// -----------------------------------------------------------------------------
void ADN_Point_GUI::AddNewElement( E_KeyPoint nKeyPoint )
{
    assert( nKeyPoint >= 0 && nKeyPoint < eNbrKeyPoints );
    PointInfos* pNewInfo = new PointInfos();
    pNewInfo->nTypeTerrain_ = nKeyPoint;

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Point_GUI::DeleteCurrentElement
// Created: AGN 2003-12-08
// -----------------------------------------------------------------------------
void ADN_Point_GUI::DeleteCurrentElement()
{
    PointInfos* pCurComposante = (PointInfos*)GetCurrentData();
    if( pCurComposante )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurComposante);
}

// -----------------------------------------------------------------------------
// Name: ADN_Point_GUI::Contains
// Created: AGN 2004-02-25
// -----------------------------------------------------------------------------
bool ADN_Point_GUI::Contains( E_KeyPoint nKeyPoint )
{
    int n = 0;
    while( item( n, 1 ) != 0 )
    {
        ADN_TableItem_ABC* pItem = static_cast<ADN_TableItem_ABC*>( item( n, 1 ) );
        PointInfos* pInfos = static_cast<PointInfos*>( pItem->GetData() );
        if( pInfos->nTypeTerrain_ == nKeyPoint )
            return true;
        ++n;
    }
    return false;
}

