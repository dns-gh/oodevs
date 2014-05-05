// *****************************************************************************
//
// ADN_AutomatLog_ListView file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AutomatLog_ListView.h"
#include "ADN_Equipments_Data.h"
#include "ADN_GridDelegate.h"
#include "ADN_Automata_Data.h"
#include "ADN_Rich_ListViewItem.h"

namespace
{
    bool AutomatLogLessThan( const QModelIndex& left, const QModelIndex& right, bool& valid, const gui::StandardModel& model )
    {
        QModelIndex mainLeft = model.GetMainModelIndex( left );
        QModelIndex mainRight = model.GetMainModelIndex( right );
        ADN_Rich_ListViewItem* leftItem = static_cast< ADN_Rich_ListViewItem* >( model.GetItemFromIndex( mainLeft ) );
        ADN_Rich_ListViewItem* rightItem = static_cast< ADN_Rich_ListViewItem* >( model.GetItemFromIndex( mainRight ) );
        if( leftItem->GetSorting() == ADN_Rich_ListViewItem::eSortingConstraint_First || rightItem->GetSorting() == ADN_Rich_ListViewItem::eSortingConstraint_Last )
        {
            valid = true;
            return false;
        }
        if( leftItem->GetSorting() == ADN_Rich_ListViewItem::eSortingConstraint_Last || rightItem->GetSorting() == ADN_Rich_ListViewItem::eSortingConstraint_First )
        {
            valid = true;
            return true;
        }
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView constructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_AutomatLog_ListView::ADN_AutomatLog_ListView( ADN_Automata_Data& data, QWidget* pParent )
    : ADN_ListView( pParent, "ADN_AutomatLog_ListView" )
    , data_( data )
    , compTotal_()
    , unitTotal_()
    , automatTotal_()
{
    setItemDelegate( new ADN_GridDelegate( this ) );
    setRootIsDecorated( true );
    setAllColumnsShowFocus( true );
    SetLessThanFunctor( boost::bind( &AutomatLogLessThan, _1, _2, _3, boost::cref( dataModel_ ) ) );

    BuildHeaders();
    BuildBody   ();
    expandAll();
    for( int i = 0; i < header()->count(); ++i )
        resizeColumnToContents( i );
    collapseAll();
    proxyModel_->sort( 0 );
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
    QStringList list;
    list << tools::translate( "ADN_AutomatLog_ListView", "Automat > Unit > Comp > Supplies" )
         << tools::translate( "ADN_AutomatLog_ListView", "Units" )
         << tools::translate( "ADN_AutomatLog_ListView", "Equipments" )
         << tools::translate( "ADN_AutomatLog_ListView", "Supplies" )
         << tools::translate( "ADN_AutomatLog_ListView", "Norm. Consumption" )
         << tools::translate( "ADN_AutomatLog_ListView", "Consumption\nMoving" )
         << tools::translate( "ADN_AutomatLog_ListView", "Autonomy\nMoving" )
         << tools::translate( "ADN_AutomatLog_ListView", "Consumption\nParked engine stopped" )
         << tools::translate( "ADN_AutomatLog_ListView", "Autonomy\nParked engine Stopped" )
         << tools::translate( "ADN_AutomatLog_ListView", "Consumption\nParked engine running" )
         << tools::translate( "ADN_AutomatLog_ListView", "Autonomy\nParked engine running" );
    dataModel_.setHorizontalHeaderLabels( list );
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::BuildBody
// Created: SBO 2006-01-05
// -----------------------------------------------------------------------------
void ADN_AutomatLog_ListView::BuildBody()
{
    // Automats
    for( auto it = data_.vAutomata_.begin(); it != data_.vAutomata_.end(); ++it )
    {
        ADN_Automata_Data::AutomatonInfos& automaton = **it;
        if( automaton.vSubUnits_.empty() )
            continue;
        ADN_Rich_ListViewItem* pAutomatItem = new ADN_Rich_ListViewItem( this, Qt::AlignCenter );
        pAutomatItem->setText( eColumnTarget, automaton.strName_.GetData().c_str() );

        // Total Unit/Dotation
        ADN_Rich_ListViewItem* pAutomatTotalItem = new ADN_Rich_ListViewItem( pAutomatItem, Qt::AlignCenter );
        pAutomatTotalItem->setText( eColumnTarget, tools::translate( "ADN_AutomatLog_ListView", "Total" ) );
        pAutomatTotalItem->OverrideSorting( ADN_Rich_ListViewItem::eSortingConstraint_Last );

        // Total units and equipment
        uint nUnitInAutomat = 1; // pc
        uint nCompInAutomat = 0;

        // Units - PC
        assert( automaton.ptrUnit_.GetData() != 0 );
        ADN_Automata_Data::UnitInfos* pUnit = automaton.ptrUnit_.GetData();
        std::string pcName = pUnit->strName_.GetData();
        nCompInAutomat += AddUnit( pAutomatItem, QString( "[PC] - %1" ).arg( pcName.c_str() ), *pUnit->GetCrossedElement(), 1 );

        // Units - Not PC
        for( auto it2 = automaton.vSubUnits_.begin(); it2 != automaton.vSubUnits_.end(); ++it2 )
        {
            ADN_Automata_Data::UnitInfos* pUnitInfos = *it2;
            if( !pUnitInfos->GetCrossedElement() )
                continue;
            std::string pionName = pUnitInfos->strName_.GetData();
            uint quantity = pUnitInfos->min_.GetData();
            if( quantity == 0 )
                continue;
            if( pionName == pcName )
            {
                if( quantity == 1 )
                    continue;
                else if( quantity > 1 )
                    --quantity;
            }
            nCompInAutomat += AddUnit( pAutomatItem, pionName.c_str(), *pUnitInfos->GetCrossedElement(), quantity );
            nUnitInAutomat += quantity;
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
// Name: ADN_AutomatLog_ListView::AddUnit
// Created: ABR 2012-07-04
// -----------------------------------------------------------------------------
uint ADN_AutomatLog_ListView::AddUnit( ADN_Rich_ListViewItem* parent, const QString& name, const ADN_Units_Data::UnitInfos& unit, uint quantity )
{
    ADN_Rich_ListViewItem* pUnitItem = new ADN_Rich_ListViewItem( parent, Qt::AlignCenter );

    // Name/Quantity
    pUnitItem->setText( eColumnTarget, name );
    pUnitItem->setText( eColumnNbrUnit, QString::number( quantity ) );

    // Total Component/Dotation
    ADN_Rich_ListViewItem* pUnitTotalItem = new ADN_Rich_ListViewItem( pUnitItem, Qt::AlignCenter );
    pUnitTotalItem->setText( eColumnTarget, tools::translate( "ADN_AutomatLog_ListView", "Total" ) );
    pUnitTotalItem->OverrideSorting( ADN_Rich_ListViewItem::eSortingConstraint_Last );

    // Component
    uint nCompInUnit = 0;
    for( auto it3 = unit.vComposantes_.begin(); it3 != unit.vComposantes_.end(); ++it3 )
    {
        ADN_Rich_ListViewItem* pCompItem = new ADN_Rich_ListViewItem( pUnitItem, Qt::AlignCenter );
        pCompItem->setText( eColumnTarget, (*it3)->GetCrossedElement()->strName_.GetData().c_str() );
        pCompItem->setText( eColumnNbrComp, QString::number( (*it3)->nNb_.GetData() ) );
        nCompInUnit += (*it3)->nNb_.GetData();

        // Consumption
        ADN_Equipments_Data::T_ConsumptionItem_Vector& consumptions = (*it3)->GetCrossedElement()->consumptions_.vConsumptions_;
        ADN_Equipments_Data::T_CategoryInfos_Vector&   categories   = (*it3)->GetCrossedElement()->resources_.categories_;
        for( auto itCompCons = consumptions.begin(); itCompCons != consumptions.end(); ++itCompCons )
        {
            auto itCategory = categories.begin();
            for( ; itCategory != categories.end(); ++itCategory )
                if( ( *itCategory ) == ( *itCompCons )->GetCrossedElement() )
                    break;
            if( itCategory != categories.end() )
                InsertCategory( *pCompItem, **itCategory, **itCompCons );
        }
        for( auto it = categories.begin(); it != categories.end(); ++it )
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
    AddEntryToTotal( unitTotal_, automatTotal_, quantity );
    FillTotalItem( *pUnitTotalItem, unitTotal_ );
    ClearEntry( unitTotal_ );
    return ( quantity > 0 ) ? nCompInUnit * quantity : nCompInUnit;
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::InsertCategory
// Created: SBO 2006-01-05
// -----------------------------------------------------------------------------
void ADN_AutomatLog_ListView::InsertCategory( QStandardItem&                         parent,
                                              ADN_Equipments_Data::CategoryInfos&   category,
                                              ADN_Equipments_Data::ConsumptionItem& conso )
{
    ADN_Resources_Data::CategoryInfo* ptrCategory = conso.GetCrossedElement() ? conso.GetCrossedElement()->GetCrossedElement() : 0;

    if( !ptrCategory || ( conso.nConsumptionType_ != eMoving && conso.nConsumptionType_ != eEngineStopped && conso.nConsumptionType_ != eEngineStarted ) )
        return;

    ADN_Rich_ListViewItem* pItem = 0;
    // try to find existing item
    const int rowCount = parent.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        QStandardItem* pTmpItem = parent.child( row, eColumnTarget );
        if( pTmpItem && pTmpItem->text().toStdString() == ptrCategory->strName_.GetData() )
        {
            pItem = static_cast< ADN_Rich_ListViewItem* >( parent.child( row ) );
            break;
        }
    }

    if( pItem == 0 )
    {
        pItem = new ADN_Rich_ListViewItem( &parent, Qt::AlignCenter );
        pItem->setText( eColumnTarget         , ptrCategory->strName_.GetData().c_str() );
        pItem->setText( eColumnContenance     , QString::number( category.rNbr_.GetData() ) );
        pItem->setText( eColumnNormalizedConso, QString::number( category.rNormalizedConsumption_.GetData() ) );
    }

    double rAutonomy = std::numeric_limits< double >::max();
    if( conso.nQuantityUsedPerHour_.GetData() > 0 )
        rAutonomy = category.rNbr_.GetData() / conso.nQuantityUsedPerHour_.GetData();

    if( compTotal_[ ptrCategory ] == 0 )
        compTotal_[ ptrCategory ] = new ADN_AutomatLog_Entry();

    compTotal_[ ptrCategory ]->rNbr_ = category.rNbr_.GetData();
    compTotal_[ ptrCategory ]->rNormalizedConsumption_ = category.rNormalizedConsumption_.GetData();

    if( conso.nConsumptionType_ == eMoving )
    {
        pItem->SetValueGreaterThan( eColumnMoveAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
        pItem->setText( eColumnMoveConso, QString::number( conso.nQuantityUsedPerHour_.GetData() ) );
        compTotal_[ ptrCategory ]->rMoveAutonomy_ = std::min( compTotal_[ ptrCategory ]->rMoveAutonomy_, rAutonomy );
        compTotal_[ ptrCategory ]->rMoveQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
    }
    else if( conso.nConsumptionType_ == eEngineStopped )
    {
        pItem->SetValueGreaterThan( eColumnEngineStoppedAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
        pItem->setText( eColumnEngineStoppedConso, QString::number( conso.nQuantityUsedPerHour_.GetData() ) );
        compTotal_[ ptrCategory ]->rEngineStoppedAutonomy_ = std::min( compTotal_[ ptrCategory ]->rEngineStoppedAutonomy_, rAutonomy );
        compTotal_[ ptrCategory ]->rEngineStoppedQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
    }
    else if( conso.nConsumptionType_ == eEngineStarted )
    {
        pItem->SetValueGreaterThan( eColumnEngineStartedAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
        pItem->setText( eColumnEngineStartedConso, QString::number( conso.nQuantityUsedPerHour_.GetData() ) );
        compTotal_[ ptrCategory ]->rEngineStartedAutonomy_ = std::min( compTotal_[ ptrCategory ]->rEngineStartedAutonomy_, rAutonomy );
        compTotal_[ ptrCategory ]->rEngineStartedQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::InsertCategory
// Created: SBO 2006-01-05
// -----------------------------------------------------------------------------
void ADN_AutomatLog_ListView::InsertCategory( QStandardItem&                       parent,
                                              ADN_Equipments_Data::CategoryInfos& category )
{
    ADN_Resources_Data::CategoryInfo* infos = category.GetCrossedElement();
    if( !infos )
        return;
    ADN_Rich_ListViewItem* pItem = 0;
    // try to find existing item
    const int rowCount = parent.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        QStandardItem* pTmpItem = parent.child( row, eColumnTarget );
        if( pTmpItem && pTmpItem->text().toStdString() == infos->strName_.GetData() )
        {
            pItem = static_cast< ADN_Rich_ListViewItem* >( parent.child( row ) );
            break;
        }
    }
    if( pItem == 0 )
    {
        pItem = new ADN_Rich_ListViewItem( &parent, Qt::AlignCenter );
        pItem->setText( eColumnTarget         , infos->strName_.GetData().c_str() );
        pItem->setText( eColumnContenance     , QString::number( category.rNbr_.GetData() ) );
        pItem->setText( eColumnNormalizedConso, QString::number( category.rNormalizedConsumption_.GetData() ) );
    }

    if( compTotal_[ infos ] == 0 )
        compTotal_[ infos ] = new ADN_AutomatLog_Entry();

    compTotal_[ infos ]->rNbr_ = category.rNbr_.GetData();
    compTotal_[ infos ]->rNormalizedConsumption_ = category.rNormalizedConsumption_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLog_ListView::GetMinMoveAutonomy
// Created: SBO 2006-01-11
// -----------------------------------------------------------------------------
double ADN_AutomatLog_ListView::GetMinMoveAutonomy( const T_CategoryEntry& entry ) const
{
    double rMin = std::numeric_limits< double >::max();
    for( auto it = entry.begin(); it != entry.end(); ++it )
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
    for( auto it = entry.begin(); it != entry.end(); ++it )
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
    for( auto it = entry.begin(); it != entry.end(); ++it )
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
    for( auto it = entry.begin(); it != entry.end(); ++it )
    {
        const ADN_Resources_Data::CategoryInfo* pCat = it->first;
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
    for( auto it = entry.begin(); it != entry.end(); )
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
void ADN_AutomatLog_ListView::FillTotalItem( QStandardItem& item, const T_CategoryEntry& entry ) const
{
    for( auto it = entry.begin(); it != entry.end(); ++it )
    {
        ADN_Rich_ListViewItem* pSubItem = new ADN_Rich_ListViewItem( &item, Qt::AlignCenter );
        pSubItem->OverrideSorting( ADN_Rich_ListViewItem::eSortingConstraint_Last );
        pSubItem->setText( eColumnTarget            , const_cast< ADN_Type_LocalizedString* >( &it->first->strName_ )->GetData().c_str() );
        pSubItem->setText( eColumnContenance        , QString::number( it->second->rNbr_ ) );
        pSubItem->setText( eColumnNormalizedConso   , QString::number( it->second->rNormalizedConsumption_ ) );
        pSubItem->setText( eColumnMoveConso         , QString::number( it->second->rMoveQuantityUsedPerHour_ ) );
        pSubItem->setText( eColumnEngineStoppedConso, QString::number( it->second->rEngineStoppedQuantityUsedPerHour_ ) );
        pSubItem->setText( eColumnEngineStartedConso, QString::number( it->second->rEngineStartedQuantityUsedPerHour_ ) );
    }
}
