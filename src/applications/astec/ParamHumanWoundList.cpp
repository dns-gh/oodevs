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
#include "ParamHumanWoundList.h"
#include "moc_ParamHumanWoundList.cpp"
#include "App.h"

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamHumanWoundList::ParamHumanWoundList( ASN1T_SantePriorites& asnListHumanWound, const std::string& /*strLabel*/, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : QTable                  ( 0, 1, pParent )
    , Param_ABC           ( pOptional )
    , pAsnHumanWoundList_     ( &asnListHumanWound )
    , pHumanWoundsStringList_ ( 0 )
{
    horizontalHeader()->setLabel( 0, tr( "Wounds" ) );
    setColumnWidth( 0, 200 );
    setLeftMargin( 0 );
    setShowGrid( false );
    setSorting( false );

    // intialize HumanWounds table
    // use empty element to determine if an HumanWound type is specified
    pHumanWoundsStringList_ = new QStringList();
    pHumanWoundsStringList_->append( "" );

    for( uint i = eHumanWound_BlesseUrgence1; i < eNbrHumanWound; ++i )
        pHumanWoundsStringList_->append( ENT_Tr::ConvertFromHumanWound( ( E_HumanWound )i ).c_str() );

    setNumRows( 0 );
    insertRows( 0, 1 );
    setItem( 0, 0, new QComboTableItem( this, *pHumanWoundsStringList_ ) );
    setMinimumHeight( rowHeight( 0 ) * 8 );

    connect( this, SIGNAL( valueChanged( int, int ) ), SLOT( OnHumanWoundChanged( int, int ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList destructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamHumanWoundList::~ParamHumanWoundList()
{
    delete pHumanWoundsStringList_;
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::WriteMsg
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void ParamHumanWoundList::WriteMsg( std::stringstream& /*strMsg*/ )
{
    assert( pAsnHumanWoundList_ );

    pAsnHumanWoundList_->n = numRows() - 1;
    if( !pAsnHumanWoundList_->n )
        return;

    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    ASN1T_EnumHumanWound* pAsnHumanWound = new ASN1T_EnumHumanWound[ pAsnHumanWoundList_->n ]; //$$$ RAM
    for( uint nRow = 0; nRow < pAsnHumanWoundList_->n; ++nRow )
    {
        QComboTableItem* pHumanWoundItem  = static_cast< QComboTableItem* >( item( nRow, 0 ) );
        assert( pHumanWoundItem );
        pAsnHumanWound[ nRow ] = ( ASN1T_EnumHumanWound )ENT_Tr::ConvertToHumanWound( pHumanWoundItem->currentText().ascii() );
    }

    pAsnHumanWoundList_->elem = pAsnHumanWound;
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::CheckValidity
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
bool ParamHumanWoundList::CheckValidity()
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
                ss << tr( "Human Wound" ) << " '" 
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
// Name: ParamHumanWoundList::OnHumanWoundChanged
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnHumanWoundChanged( int nRow, int /*nCol*/ )
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
            setItem( nPos, 0, new QComboTableItem( this, *pHumanWoundsStringList_ ) );
            pComboTableItem->setCurrentItem( nCurrentItem );
        }
        setCurrentCell( nRow, 0 );
    }
}