//*****************************************************************************
//
// $Created: JDY 03-07-28 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Units_Composantes_GUI.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 16:14 $
// $Revision: 15 $
// $Workfile: ADN_Units_Composantes_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Units_Composantes_GUI.h"
#include "moc_ADN_Units_Composantes_GUI.cpp"
#include "ADN_CommonGfx.h"
#include "ADN_Gui_Tools.h"
#include "ADN_Units_Data.h"
#include "ADN_Workspace.h"

typedef ADN_Equipments_Data::EquipmentInfos   ComposanteInfos;
typedef ADN_Units_Data::ComposanteInfos       UnitComposanteInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Composantes_GUI::ADN_Units_Composantes_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table            ( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 6 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Equipments" )
                      << tr( "Qty" )
                      << tr( "Major" )
                      << tr( "Loadable" )
                      << tr( "Conveyor" )
                      << tr( "Crew size" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 2, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 3, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 4, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 5, QHeaderView::ResizeToContents );
    verticalHeader()->setVisible( false );

    delegate_.AddSpinBoxOnColumn( 1, 0, std::numeric_limits< int >::max() );
    delegate_.AddCheckBoxOnColumn( 2 );
    delegate_.AddCheckBoxOnColumn( 3 );
    delegate_.AddCheckBoxOnColumn( 4 );
    delegate_.AddSpinBoxOnColumn( 5, 0, std::numeric_limits< int >::max() );
    proxyModel_->setDynamicSortFilter( true );
    proxyModel_->sort( 0, Qt::AscendingOrder );
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Composantes_GUI::~ADN_Units_Composantes_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI::AddRow
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Units_Composantes_GUI::AddRow( int row, void* data )
{
    UnitComposanteInfos* composante = static_cast< UnitComposanteInfos* >( data );
    if( !composante )
        return;
    AddItem( row, 0, data, &composante->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &composante->nNb_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 2, data, &composante->bMajor_, ADN_StandardItem::eBool, Qt::ItemIsEditable );
    AddItem( row, 3, data, &composante->bLoadable_, ADN_StandardItem::eBool, Qt::ItemIsEditable );
    AddItem( row, 4, data, &composante->bConveyor_, ADN_StandardItem::eBool, Qt::ItemIsEditable );
    AddItem( row, 5, data, &composante->nNbrHumanInCrew_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_Units_Composantes_GUI::OnContextMenu( const QPoint& pt )
{
    ADN_Gui_Tools::GenerateStandardEditionDialog< ComposanteInfos, UnitComposanteInfos >(
        *this, pt, "equipment-list", tr( "Equipments" ), ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
        ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipments() );
}
