// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamEquipmentList.h"
#include "moc_ParamEquipmentList.cpp"
#include "App.h"

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::ParamEquipmentList( ASN1T_MaintenancePriorites& asnListEquipment, const std::string& /*strLabel*/, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : QTable                 ( 0, 1, pParent )
    , Param_ABC          ( pOptional )
    , pAsnEquipmentList_     ( &asnListEquipment )
    , pEquipmentsStringList_ ( 0 )
{
    horizontalHeader()->setLabel( 0, tr( "Equipement" ) );
    setColumnWidth  ( 0, 200 );
    setLeftMargin   ( 0 );
    setShowGrid     ( false );
    setSorting      ( false );

    // intialize equipments table
    // use empty element to determine if an equipment type is specified
    pEquipmentsStringList_ = new QStringList();
    pEquipmentsStringList_->append( "" );

    const T_MosId_String_Map& equipments = App::GetApp().GetEquipmentNames();
    for( CIT_MosId_String_Map it = equipments.begin(); it != equipments.end(); ++it )
        pEquipmentsStringList_->append( it->second.c_str() );

    setNumRows( 0 );
    insertRows( 0, 1 );
    setItem( 0, 0, new QComboTableItem( this, *pEquipmentsStringList_ ) );
    setMinimumHeight( rowHeight( 0 ) * 8 );

    connect( this, SIGNAL( valueChanged( int, int ) ), SLOT( OnEquipmentChanged( int, int ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: ParamEquipmentList destructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::~ParamEquipmentList()
{
    delete pEquipmentsStringList_;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::WriteMsg
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void ParamEquipmentList::WriteMsg( std::stringstream& /*strMsg*/ )
{
    assert( pAsnEquipmentList_ );

    pAsnEquipmentList_->n = numRows() - 1;
    if( !pAsnEquipmentList_->n )
        return;

    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    ASN1T_TypeEquipement* pAsnEquipement = new ASN1T_TypeEquipement[ pAsnEquipmentList_->n ]; //$$$ RAM
    for( uint nRow = 0; nRow < pAsnEquipmentList_->n; ++nRow )
    {
        QComboTableItem* pEquipementItem  = static_cast< QComboTableItem* >( item( nRow, 0 ) );
        assert( pEquipementItem );
        pAsnEquipement[ nRow ] = App::GetApp().GetEquipementID( pEquipementItem->currentText().ascii() );
    }

    pAsnEquipmentList_->elem = pAsnEquipement;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::CheckValidity
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
bool ParamEquipmentList::CheckValidity()
{
    // check for duplicate
    int nNbRows = numRows() - 1;
    for( int i = 0; i < nNbRows; ++i )
    {
        QComboTableItem* pItem1  = static_cast< QComboTableItem* >( item( i, 0 ) );
        assert( pItem1 );
        for( int j = i + 1; j < nNbRows; ++j )
        {
            QComboTableItem* pItem2  = static_cast< QComboTableItem* >( item( j, 0 ) );
            assert( pItem2 );
            if( pItem1->currentText() == pItem2->currentText() )
            {
                std::stringstream ss;
                ss << tr( "Equipment" ) << " '" 
                << pItem1->currentText()  << "' "
                << tr( "appears more than once." );
                QMessageBox::critical( 0, "Light 2", ss.str().c_str() );
                return false;
            }
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnEquipmentChanged
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnEquipmentChanged( int nRow, int /*nCol*/ )
{
    QComboTableItem* pComboTableItem = static_cast< QComboTableItem* >( item( nRow, 0 ) );
    assert( pComboTableItem );

    if( pComboTableItem->currentItem() == 0 )
    {
        // if not last row, delete empty row
        if( nRow != numRows() - 1 )
        {
            removeRow( nRow );
            // select last row quantity field
            setCurrentCell( numRows() - 1, 0 );
        }
    }
    else
    {
        // if last row is set, add a new row to table
        if( nRow == numRows() - 1 )
        {
            // need to save combo box selected element before to insert a line
            int nCurrentItem = pComboTableItem->currentItem();
            uint nPos = nRow + 1;
            insertRows( nPos, 1 );
            setItem( nPos, 0, new QComboTableItem( this, *pEquipmentsStringList_ ) );
            pComboTableItem->setCurrentItem( nCurrentItem );
        }
        setCurrentCell( nRow, 0 );
    }
}
