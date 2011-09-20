// *****************************************************************************
//
// ADN_AutomatLog_ListView file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-04 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AutomatLog_ListView.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 15:53 $
// $Revision: 3 $
// $Workfile: ADN_AutomatLog_ListView.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Composantes_Data.h"
#include "ADN_AutomatLog_ListView.h"

#include "ADN_Automata_Data.h"
#include "ADN_Rich_ListViewItem.h"

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView constructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_AutomatLog_ListView::ADN_AutomatLog_ListView( ADN_Automata_Data& data, QWidget* pParent )
: ADN_ListView( pParent )
, data_       ( data )
, compTotal_    ()
, unitTotal_    ()
, automatTotal_ ()
{
    setRootIsDecorated( true );
    setAllColumnsShowFocus( true );

    BuildHeaders();
    BuildBody   ();
}


// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView destructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_AutomatLog_ListView::~ADN_AutomatLog_ListView()
{
    ClearEntry( compTotal_ );
    ClearEntry( unitTotal_ );
    ClearEntry( automatTotal_ );
}


// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::BuildHeaders
// Created: SBO 2006-01-05
// -----------------------------------------------------------------------------
void ADN_AutomatLog_ListView::BuildHeaders()
{
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Automat > Unit > Comp > Item" ) );
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Units" ) );
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Equipments" ) );
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Resources" ) );
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Norm. Consumption" ) );
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Consumption\nMoving" ) );
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Autonomy\nMoving" ) );
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Consumption\nParked engine stopped" ) );
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Autonomy\nParked engine Stopped" ) );
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Consumption\nParked engine running" ) );
    addColumn( tools::translate( "ADN_AutomatLog_ListView", "Autonomy\nParked engine running" ) );
    for( int i = 1; i < columns(); ++i )
        setColumnAlignment( i, Qt::AlignCenter );
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::BuildBody
// Created: SBO 2006-01-05
// -----------------------------------------------------------------------------
void ADN_AutomatLog_ListView::BuildBody()
{
    // Automat
    for( ADN_Automata_Data::IT_AutomatonInfosVector it = data_.vAutomata_.begin(); it != data_.vAutomata_.end(); ++it )
    {
        ADN_Automata_Data::AutomatonInfos& automaton = **it;
        ADN_Rich_ListViewItem* pAutomatItem = new ADN_Rich_ListViewItem( this, true );
        pAutomatItem->setText( eColumnTarget, automaton.strName_.GetData().c_str() );

        // Total Unit/Dotation
        ADN_Rich_ListViewItem* pAutomatTotalItem = new ADN_Rich_ListViewItem( pAutomatItem, true );
        pAutomatTotalItem->setText( eColumnTarget, tools::translate( "ADN_AutomatLog_ListView", "Total" ) );
        pAutomatTotalItem->OverrideSorting( ADN_Rich_ListViewItem::eSortingConstraint_Last );

        // Unit
        uint nUnitInAutomat = 0;
        uint nCompInAutomat = 0;
        for( ADN_Automata_Data::IT_UnitInfosVector it2 = automaton.vSubUnits_.begin(); nUnitInAutomat == 0 || it2 != automaton.vSubUnits_.end(); )
        {
            ADN_Automata_Data::UnitInfos* pUnitInfos = 0;
            ADN_Units_Data::UnitInfos*    pUnit;
            ADN_Rich_ListViewItem* pUnitItem = new ADN_Rich_ListViewItem( pAutomatItem, true );

            uint nUnit = 0;
            /*if( nUnitInAutomat == 0 )
            {
                pUnit = automaton.ptrUnit_.GetData();
                ++nUnit;
            }
            else*/
            {
                pUnitInfos = *it2;
                assert( pUnitInfos->ptrUnit_.GetData() != 0 );
                pUnit = pUnitInfos->ptrUnit_.GetData();
                ++it2;
                nUnit += pUnitInfos->min_.GetData();
            }
            ADN_Units_Data::UnitInfos& unit = *pUnit;
            /*if( nUnitInAutomat == 0 )
                pUnitItem->setText( eColumnTarget, QString( "[PC] - " ) + unit.strName_.GetData().c_str() );
            else*/
                pUnitItem->setText( eColumnTarget, unit.strName_.GetData().c_str() );
            pUnitItem->setText( eColumnNbrUnit, QString::number( nUnit ) );
            nUnitInAutomat += nUnit;

            // Total Component/Dotation
            ADN_Rich_ListViewItem* pUnitTotalItem = new ADN_Rich_ListViewItem( pUnitItem, true );
            pUnitTotalItem->setText( eColumnTarget, tools::translate( "ADN_AutomatLog_ListView", "Total" ) );
            pUnitTotalItem->OverrideSorting( ADN_Rich_ListViewItem::eSortingConstraint_Last );

            // Component
            uint nCompInUnit = 0;
            for( ADN_Units_Data::IT_ComposanteInfos_Vector it3 = unit.vComposantes_.begin(); it3 != unit.vComposantes_.end(); ++it3 )
            {
                ADN_Rich_ListViewItem* pCompItem = new ADN_Rich_ListViewItem( pUnitItem, true );
                pCompItem->setText( eColumnTarget, (*it3)->ptrComposante_.GetData()->strName_.GetData().c_str() );
                pCompItem->setText( eColumnNbrComp, QString::number( (*it3)->nNb_.GetData() ) );
                nCompInUnit += (*it3)->nNb_.GetData();

                // Consumption
                ADN_Composantes_Data::T_ConsumptionItem_Vector& consumptions = (*it3)->ptrComposante_.GetData()->consumptions_.vConsumptions_;
                ADN_Composantes_Data::T_CategoryInfos_Vector&   categories   = (*it3)->ptrComposante_.GetData()->resources_.categories_;
                for( ADN_Composantes_Data::IT_ConsumptionItem_Vector itCompCons = consumptions.begin(); itCompCons != consumptions.end(); ++itCompCons )
                {
                    ADN_Composantes_Data::IT_CategoryInfos_Vector itCategory = categories.begin();
                    for( ; itCategory != categories.end(); ++itCategory )
                        if( (*itCategory)->ptrCategory_.GetData() == (*itCompCons)->ptrCategory_.GetData() )
                            break;
                    if( itCategory != categories.end() )
                        InsertCategory( *pCompItem, **itCategory, **itCompCons );
                }
                for( ADN_Composantes_Data::IT_CategoryInfos_Vector it = categories.begin(); it != categories.end(); ++it )
                    InsertCategory( *pCompItem, **it );
                pCompItem->SetValueGreaterThan( eColumnMoveAutonomy         , GetMinMoveAutonomy         ( compTotal_ ), 2., 3., ADN_Rich_ListViewItem::eUnitHour );
                pCompItem->SetValueGreaterThan( eColumnEngineStoppedAutonomy, GetMinEngineStoppedAutonomy( compTotal_ ), 2., 3., ADN_Rich_ListViewItem::eUnitHour );
                pCompItem->SetValueGreaterThan( eColumnEngineStartedAutonomy, GetMinEngineStartedAutonomy( compTotal_ ), 2., 3., ADN_Rich_ListViewItem::eUnitHour );
                AddEntryToTotal( compTotal_, unitTotal_, (*it3)->nNb_.GetData() );
                ClearEntry( compTotal_ );
            }
            pUnitItem->setText( eColumnNbrComp, QString::number( nCompInUnit ) );
            pUnitItem->SetValueGreaterThan( eColumnMoveAutonomy         , GetMinMoveAutonomy         ( unitTotal_ ), 2., 3., ADN_Rich_ListViewItem::eUnitHour );
            pUnitItem->SetValueGreaterThan( eColumnEngineStoppedAutonomy, GetMinEngineStoppedAutonomy( unitTotal_ ), 2., 3., ADN_Rich_ListViewItem::eUnitHour );
            pUnitItem->SetValueGreaterThan( eColumnEngineStartedAutonomy, GetMinEngineStartedAutonomy( unitTotal_ ), 2., 3., ADN_Rich_ListViewItem::eUnitHour );
            AddEntryToTotal( unitTotal_, automatTotal_, nUnit );
            FillTotalItem( *pUnitTotalItem, unitTotal_ );
            ClearEntry( unitTotal_ );
            nCompInAutomat += nCompInUnit * nUnit;
        }
        pAutomatItem->setText( eColumnNbrUnit, QString::number( nUnitInAutomat ) );
        pAutomatItem->setText( eColumnNbrComp, QString::number( nCompInAutomat ) );
        pAutomatItem->SetValueGreaterThan( eColumnMoveAutonomy         , GetMinMoveAutonomy         ( automatTotal_ ), 2., 3., ADN_Rich_ListViewItem::eUnitHour );
        pAutomatItem->SetValueGreaterThan( eColumnEngineStoppedAutonomy, GetMinEngineStoppedAutonomy( automatTotal_ ), 2., 3., ADN_Rich_ListViewItem::eUnitHour );
        pAutomatItem->SetValueGreaterThan( eColumnEngineStartedAutonomy, GetMinEngineStartedAutonomy( automatTotal_ ), 2., 3., ADN_Rich_ListViewItem::eUnitHour );
        FillTotalItem( *pAutomatTotalItem, automatTotal_ );
        ClearEntry( automatTotal_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::InsertCategory
// Created: SBO 2006-01-05
// -----------------------------------------------------------------------------
void ADN_AutomatLog_ListView::InsertCategory( Q3ListViewItem&                         parent,
                                              ADN_Composantes_Data::CategoryInfos&   category,
                                              ADN_Composantes_Data::ConsumptionItem& conso )
{
    if( conso.nConsumptionType_ != eMoving
        && conso.nConsumptionType_ != eEngineStopped
        && conso.nConsumptionType_ != eEngineStarted )
        return;

    ADN_Rich_ListViewItem* pItem = 0;
    // try to find existing item
    for( Q3ListViewItem* pTmpItem = parent.firstChild(); pTmpItem != 0; )
    {
        if( std::string( pTmpItem->text( eColumnTarget ) ) == conso.ptrCategory_.GetData()->strName_.GetData() )
        {
            pItem = static_cast< ADN_Rich_ListViewItem* >( pTmpItem );
            break;
        }
        pTmpItem = pTmpItem->nextSibling();
    }

    if( pItem == 0 )
    {
        pItem = new ADN_Rich_ListViewItem( &parent, true );
        pItem->setText( eColumnTarget         , conso.ptrCategory_.GetData()->strName_.GetData().c_str() );
        pItem->setText( eColumnContenance     , QString::number( category.rNbr_.GetData() ) );
        pItem->setText( eColumnNormalizedConso, QString::number( category.rNormalizedConsumption_.GetData() ) );
    }

    double rAutonomy = std::numeric_limits< double >::max();
    if( conso.nQuantityUsedPerHour_.GetData() > 0 )
        rAutonomy = category.rNbr_.GetData() / conso.nQuantityUsedPerHour_.GetData();

    if( compTotal_[ conso.ptrCategory_.GetData() ] == 0 )
        compTotal_[ conso.ptrCategory_.GetData() ] = new ADN_AutomatLog_Entry();

    compTotal_[ conso.ptrCategory_.GetData() ]->rNbr_ = category.rNbr_.GetData();
    compTotal_[ conso.ptrCategory_.GetData() ]->rNormalizedConsumption_ = category.rNormalizedConsumption_.GetData();

    if( conso.nConsumptionType_ == eMoving )
    {
        pItem->SetValueGreaterThan( eColumnMoveAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
        pItem->setText( eColumnMoveConso, QString::number( conso.nQuantityUsedPerHour_.GetData() ) );
        compTotal_[ conso.ptrCategory_.GetData() ]->rMoveAutonomy_ = std::min( compTotal_[ conso.ptrCategory_.GetData() ]->rMoveAutonomy_, rAutonomy );
        compTotal_[ conso.ptrCategory_.GetData() ]->rMoveQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
    }
    else if( conso.nConsumptionType_ == eEngineStopped )
    {
        pItem->SetValueGreaterThan( eColumnEngineStoppedAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
        pItem->setText( eColumnEngineStoppedConso, QString::number( conso.nQuantityUsedPerHour_.GetData() ) );
        compTotal_[ conso.ptrCategory_.GetData() ]->rEngineStoppedAutonomy_ = std::min( compTotal_[ conso.ptrCategory_.GetData() ]->rEngineStoppedAutonomy_, rAutonomy );
        compTotal_[ conso.ptrCategory_.GetData() ]->rEngineStoppedQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
    }
    else if( conso.nConsumptionType_ == eEngineStarted )
    {
        pItem->SetValueGreaterThan( eColumnEngineStartedAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
        pItem->setText( eColumnEngineStartedConso, QString::number( conso.nQuantityUsedPerHour_.GetData() ) );
        compTotal_[ conso.ptrCategory_.GetData() ]->rEngineStartedAutonomy_ = std::min( compTotal_[ conso.ptrCategory_.GetData() ]->rEngineStartedAutonomy_, rAutonomy );
        compTotal_[ conso.ptrCategory_.GetData() ]->rEngineStartedQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::InsertCategory
// Created: SBO 2006-01-05
// -----------------------------------------------------------------------------
void ADN_AutomatLog_ListView::InsertCategory( Q3ListViewItem&                         parent,
                                              ADN_Composantes_Data::CategoryInfos&   category )
{
    ADN_Rich_ListViewItem* pItem = 0;
    // try to find existing item
    for( Q3ListViewItem* pTmpItem = parent.firstChild(); pTmpItem != 0; )
    {
        if( std::string( pTmpItem->text( eColumnTarget ) ) == category.ptrCategory_.GetData()->strName_.GetData() )
        {
            pItem = static_cast< ADN_Rich_ListViewItem* >( pTmpItem );
            break;
        }
        pTmpItem = pTmpItem->nextSibling();
    }

    if( pItem == 0 )
    {
        pItem = new ADN_Rich_ListViewItem( &parent, true );
        pItem->setText( eColumnTarget         , category.ptrCategory_.GetData()->strName_.GetData().c_str() );
        pItem->setText( eColumnContenance     , QString::number( category.rNbr_.GetData() ) );
        pItem->setText( eColumnNormalizedConso, QString::number( category.rNormalizedConsumption_.GetData() ) );
    }

    if( compTotal_[ category.ptrCategory_.GetData() ] == 0 )
        compTotal_[ category.ptrCategory_.GetData() ] = new ADN_AutomatLog_Entry();

    compTotal_[ category.ptrCategory_.GetData() ]->rNbr_ = category.rNbr_.GetData();
    compTotal_[ category.ptrCategory_.GetData() ]->rNormalizedConsumption_ = category.rNormalizedConsumption_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::GetMinMoveAutonomy
// Created: SBO 2006-01-11
// -----------------------------------------------------------------------------
double ADN_AutomatLog_ListView::GetMinMoveAutonomy( const T_CategoryEntry& entry ) const
{
    double rMin = std::numeric_limits< double >::max();
    for( ADN_AutomatLog_ListView::CIT_CategoryEntry it = entry.begin(); it != entry.end(); ++it )
        if( it->second && it->second->rMoveAutonomy_ < rMin )
            rMin = it->second->rMoveAutonomy_;
    return ( rMin >= 0 ) ? rMin : std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::GetMinEngineStoppedAutonomy
// Created: SBO 2006-01-11
// -----------------------------------------------------------------------------
double ADN_AutomatLog_ListView::GetMinEngineStoppedAutonomy( const T_CategoryEntry& entry ) const
{
    double rMin = std::numeric_limits< double >::max();
    for( ADN_AutomatLog_ListView::CIT_CategoryEntry it = entry.begin(); it != entry.end(); ++it )
        if( it->second && it->second->rEngineStoppedAutonomy_ < rMin )
            rMin = it->second->rEngineStoppedAutonomy_;
    return ( rMin >= 0 ) ? rMin : std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::GetMinEngineStartedAutonomy
// Created: SBO 2006-01-11
// -----------------------------------------------------------------------------
double ADN_AutomatLog_ListView::GetMinEngineStartedAutonomy( const T_CategoryEntry& entry ) const
{
    double rMin = std::numeric_limits< double >::max();
    for( ADN_AutomatLog_ListView::CIT_CategoryEntry it = entry.begin(); it != entry.end(); ++it )
        if( it->second && it->second->rEngineStartedAutonomy_ < rMin )
            rMin = it->second->rEngineStartedAutonomy_;
    return ( rMin >= 0 ) ? rMin : std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::AddEntryToTotal
// Created: SBO 2006-01-11
// -----------------------------------------------------------------------------
void ADN_AutomatLog_ListView::AddEntryToTotal( const T_CategoryEntry& entry, T_CategoryEntry& total, uint nArity )
{
    for( CIT_CategoryEntry it = entry.begin(); it != entry.end(); ++it )
    {
        const ADN_Equipement_Data::CategoryInfo* pCat = it->first;
        if( total[ pCat ] == 0 )
            total[ pCat ] = new ADN_AutomatLog_Entry();
        total[ pCat ]->rNbr_                   += it->second->rNbr_                   * nArity;
        total[ pCat ]->rNormalizedConsumption_ += it->second->rNormalizedConsumption_ * nArity;
        total[ pCat ]->rMoveAutonomy_          = std::min( total[ pCat ]->rMoveAutonomy_         , it->second->rMoveAutonomy_ );
        total[ pCat ]->rEngineStoppedAutonomy_ = std::min( total[ pCat ]->rEngineStoppedAutonomy_, it->second->rEngineStoppedAutonomy_ );
        total[ pCat ]->rEngineStartedAutonomy_ = std::min( total[ pCat ]->rEngineStartedAutonomy_, it->second->rEngineStartedAutonomy_ );
        total[ pCat ]->rMoveQuantityUsedPerHour_          += it->second->rMoveQuantityUsedPerHour_          * nArity;
        total[ pCat ]->rEngineStoppedQuantityUsedPerHour_ += it->second->rEngineStoppedQuantityUsedPerHour_ * nArity;
        total[ pCat ]->rEngineStartedQuantityUsedPerHour_ += it->second->rEngineStartedQuantityUsedPerHour_ * nArity;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::ClearEntry
// Created: SBO 2006-01-11
// -----------------------------------------------------------------------------
void ADN_AutomatLog_ListView::ClearEntry( T_CategoryEntry& entry )
{
    for( IT_CategoryEntry it = entry.begin(); it != entry.end(); )
    {
        delete it->second;
        it = entry.erase( it );
    }
    entry.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::FillTotalItem
// Created: SBO 2006-01-11
// -----------------------------------------------------------------------------
void ADN_AutomatLog_ListView::FillTotalItem( Q3ListViewItem& item, const T_CategoryEntry& entry ) const
{
    for( CIT_CategoryEntry it = entry.begin(); it != entry.end(); ++it )
    {
        ADN_Rich_ListViewItem* pSubItem = new ADN_Rich_ListViewItem( &item, true );
        pSubItem->OverrideSorting( ADN_Rich_ListViewItem::eSortingConstraint_Last );
        pSubItem->setText( eColumnTarget            , const_cast< ADN_Type_String* >( &it->first->strName_ )->GetData().c_str() );
        pSubItem->setText( eColumnContenance        , QString::number( it->second->rNbr_ ) );
        pSubItem->setText( eColumnNormalizedConso   , QString::number( it->second->rNormalizedConsumption_ ) );
        pSubItem->setText( eColumnMoveConso         , QString::number( it->second->rMoveQuantityUsedPerHour_ ) );
        pSubItem->setText( eColumnEngineStoppedConso, QString::number( it->second->rEngineStoppedQuantityUsedPerHour_ ) );
        pSubItem->setText( eColumnEngineStartedConso, QString::number( it->second->rEngineStartedQuantityUsedPerHour_ ) );
    }
}
