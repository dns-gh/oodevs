// *****************************************************************************
//
// ADN_AutomatLogCategory_ListView file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AutomatLogCategory_ListView.h"
#include "ADN_Equipments_Data.h"
#include "ADN_GridDelegate.h"
#include "ADN_Automata_Data.h"
#include "ADN_Rich_ListViewItem.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLogCategory_ListView constructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_AutomatLogCategory_ListView::ADN_AutomatLogCategory_ListView( ADN_Automata_Data& data, QWidget* pParent )
    : ADN_ListView  ( pParent, "ADN_AutomatLogCategory_ListView" )
    , data_         ( data )
    , unitTotal_    ()
    , automatTotal_ ()
    , categoryTotal_()
    , dotationTotal_()
{
    setItemDelegate( new ADN_GridDelegate( this ) );
    setRootIsDecorated( true );
    setAllColumnsShowFocus( true );

    BuildHeaders();
    BuildBody   ();
    expandAll();
    for( int i = 0; i < header()->count(); ++i )
        resizeColumnToContents( i );
    collapseAll();
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLogCategory_ListView destructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_AutomatLogCategory_ListView::~ADN_AutomatLogCategory_ListView()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLogCategory_ListView::BuildHeaders
// Created: SBO 2006-01-05
// -----------------------------------------------------------------------------
void ADN_AutomatLogCategory_ListView::BuildHeaders()
{
    QStringList list;
    list << tools::translate( "ADN_AutomatLogCategory_ListView", "Resource type > Resource > Automaton > Unit > Equipment" )
         << tools::translate( "ADN_AutomatLogCategory_ListView", "Units" )
         << tools::translate( "ADN_AutomatLogCategory_ListView", "Equipments" )
         << tools::translate( "ADN_AutomatLogCategory_ListView", "Resources" )
         << tools::translate( "ADN_AutomatLogCategory_ListView", "Norm. Consumption" )
         << tools::translate( "ADN_AutomatLogCategory_ListView", "Consumption\nMoving" )
         << tools::translate( "ADN_AutomatLogCategory_ListView", "Autonomy\nMoving" )
         << tools::translate( "ADN_AutomatLogCategory_ListView", "Consumption\nParked engine stopped" )
         << tools::translate( "ADN_AutomatLogCategory_ListView", "Autonomy\nParked engine Stopped" )
         << tools::translate( "ADN_AutomatLogCategory_ListView", "Consumption\nParked engine running" )
         << tools::translate( "ADN_AutomatLogCategory_ListView", "Autonomy\nParked engine running" );

    dataModel_.setHorizontalHeaderLabels( list );
}

namespace
{
    class Context
    {
    public:
        Context ( ADN_Rich_ListViewItem*& pAutomatItem,
                  ADN_Rich_ListViewItem*& pUnitItem,
                  ADN_Rich_ListViewItem*& pCatIem,
                  ADN_Automata_Data::AutomatonInfos& automaton,
                  uint nUnitInAutomat,
                  uint nUnit,
                  ADN_Units_Data::UnitInfos& unit,
                  ADN_AutomatLogCategory_ListView::T_Entry& compTotal )
            : pAutomatItem_( pAutomatItem )
            , pUnitItem_( pUnitItem )
            , pCatItem_( pCatIem )
            , automaton_( automaton )
            , nUnitInAutomat_( nUnitInAutomat )
            , nUnit_( nUnit )
            , unit_( unit )
            , compTotal_( compTotal )
        {
        }
        ADN_Rich_ListViewItem* GetAutomat()
        {
            if( !pAutomatItem_ )
            {
                pAutomatItem_ = new ADN_Rich_ListViewItem( pCatItem_, Qt::AlignVCenter | Qt::AlignRight );
                pAutomatItem_->setText( ADN_AutomatLogCategory_ListView::eColumnTarget, automaton_.strName_.GetData().c_str() );
            }
            return pAutomatItem_;
        }
        ADN_Rich_ListViewItem* GetUnit()
        {
            if( !pUnitItem_ )
            {
                pUnitItem_ = new ADN_Rich_ListViewItem( GetAutomat(), Qt::AlignVCenter | Qt::AlignRight );
                if( nUnitInAutomat_ == 0 )
                    pUnitItem_->setText( ADN_AutomatLogCategory_ListView::eColumnTarget, QString( "[PC] - " ) + unit_.strName_.GetData().c_str() );
                else
                    pUnitItem_->setText( ADN_AutomatLogCategory_ListView::eColumnTarget, unit_.strName_.GetData().c_str() );
                pUnitItem_->setText( ADN_AutomatLogCategory_ListView::eColumnNbrUnit, QString::number( nUnit_ ) );
            }
            return pUnitItem_;
        }
    public:
        ADN_AutomatLogCategory_ListView::T_Entry& compTotal_;
    private:
        Context& operator= ( const Context& );
        ADN_Rich_ListViewItem*& pAutomatItem_;
        ADN_Rich_ListViewItem*& pUnitItem_;
        ADN_Rich_ListViewItem*& pCatItem_;
        ADN_Automata_Data::AutomatonInfos& automaton_;
        uint nUnitInAutomat_;
        uint nUnit_;
        ADN_Units_Data::UnitInfos& unit_;
    };

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLogCategory_ListView::FillComponentItem
// Created: SBO 2006-01-12
// -----------------------------------------------------------------------------
    ADN_Rich_ListViewItem* FillComponentItem( Context& context,
                                              ADN_Resources_Data::CategoryInfo& category,
                                              ADN_Units_Data::ComposanteInfos&   comp )
    {
        // retrieve corresponding dotation category
        ADN_Equipments_Data::T_CategoryInfos_Vector& categories = comp.GetCrossedElement()->resources_.categories_;
        auto itCategory = categories.begin();
        for( ; itCategory != categories.end(); ++itCategory )
            if( ( *itCategory )->GetCrossedElement() == &category )
                break;
        if( itCategory == categories.end() )
            return 0;
        ADN_Rich_ListViewItem* item = new ADN_Rich_ListViewItem( context.GetUnit(), Qt::AlignVCenter | Qt::AlignRight );
        item->setText( ADN_AutomatLogCategory_ListView::eColumnTarget, comp.GetCrossedElement()->strName_.GetData().c_str() );
        item->setText( ADN_AutomatLogCategory_ListView::eColumnNbrComp, QString::number( comp.nNb_.GetData() ) );
        item->setText( ADN_AutomatLogCategory_ListView::eColumnContenance     , QString::number( (*itCategory)->rNbr_.GetData(), 'f', 2 ) );
        item->setText( ADN_AutomatLogCategory_ListView::eColumnNormalizedConso, QString::number( (*itCategory)->rNormalizedConsumption_.GetData(), 'f', 2 ) );
        context.compTotal_.rNbr_                   = (*itCategory)->rNbr_.GetData();
        context.compTotal_.rNormalizedConsumption_ = (*itCategory)->rNormalizedConsumption_.GetData();

        // consumption
        ADN_Equipments_Data::T_ConsumptionItem_Vector& consumptions = comp.GetCrossedElement()->consumptions_.vConsumptions_;
        for( auto itConso = consumptions.begin(); itConso != consumptions.end(); ++itConso )
        {
            ADN_Equipments_Data::ConsumptionItem& conso = **itConso;
            if( conso.GetCrossedElement() && conso.GetCrossedElement()->GetCrossedElement() == &category )
            {
                if( conso.nConsumptionType_ != eMoving && conso.nConsumptionType_ != eEngineStopped )
                    continue;

                double rAutonomy = std::numeric_limits< double >::max();
                if( conso.nQuantityUsedPerHour_.GetData() > 0 )
                    rAutonomy = (*itCategory)->rNbr_.GetData() / conso.nQuantityUsedPerHour_.GetData();

                if( conso.nConsumptionType_ == eMoving )
                {
                    item->SetValueGreaterThan( ADN_AutomatLogCategory_ListView::eColumnMoveAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
                    item->setText( ADN_AutomatLogCategory_ListView::eColumnMoveConso, QString::number( conso.nQuantityUsedPerHour_.GetData(), 'f', 2 ) );
                    context.compTotal_.rMoveAutonomy_            = rAutonomy;
                    context.compTotal_.rMoveQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
                }
                else if( conso.nConsumptionType_ == eEngineStopped )
                {
                    item->SetValueGreaterThan( ADN_AutomatLogCategory_ListView::eColumnEngineStoppedAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
                    item->setText( ADN_AutomatLogCategory_ListView::eColumnEngineStoppedConso, QString::number( conso.nQuantityUsedPerHour_.GetData(), 'f', 2 ) );
                    context.compTotal_.rEngineStoppedAutonomy_            = rAutonomy;
                    context.compTotal_.rEngineStoppedQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
                }
                else if( conso.nConsumptionType_ == eEngineStarted )
                {
                    item->SetValueGreaterThan( ADN_AutomatLogCategory_ListView::eColumnEngineStartedAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
                    item->setText( ADN_AutomatLogCategory_ListView::eColumnEngineStartedConso, QString::number( conso.nQuantityUsedPerHour_.GetData(), 'f', 2 ) );
                    context.compTotal_.rEngineStartedAutonomy_            = rAutonomy;
                    context.compTotal_.rEngineStartedQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
                }
            }
        }
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLogCategory_ListView::BuildBody
// Created: SBO 2006-01-05
// -----------------------------------------------------------------------------
void ADN_AutomatLogCategory_ListView::BuildBody()
{
    // Dotation (eg. ration)
    ADN_Resources_Data::T_ResourceInfos_Vector& dotations = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResources();
    for( auto itDotation = dotations.begin(); itDotation != dotations.end(); ++itDotation )
    {
        ADN_Resources_Data::ResourceInfos& dotation = **itDotation;
        ADN_Rich_ListViewItem* pDotationItem = new ADN_Rich_ListViewItem( this, Qt::AlignVCenter | Qt::AlignRight );
        pDotationItem->setText( eColumnTarget, dotation.strName_.GetData().c_str() );

        // Dotation category (eg. bouffe)
        ADN_Resources_Data::T_CategoryInfos_Vector& categories = dotation.GetCategories();
        for( auto itCat = categories.begin(); itCat != categories.end(); ++itCat )
        {
            ADN_Resources_Data::CategoryInfo& category = **itCat;
            ADN_Rich_ListViewItem* pCatItem = new ADN_Rich_ListViewItem( pDotationItem, Qt::AlignVCenter | Qt::AlignRight );
            pCatItem->setText( eColumnTarget, category.strName_.GetData().c_str() );

            // Automat
            for( auto it = data_.vAutomata_.begin(); it != data_.vAutomata_.end(); ++it )
            {
                ADN_Automata_Data::AutomatonInfos& automaton = **it;
                if( automaton.vSubUnits_.empty() )
                    continue;
                ADN_Rich_ListViewItem* pAutomatItem = 0;
                ADN_Automata_Data::UnitInfos* pPC = automaton.ptrUnit_.GetData();

                // Unit
                uint nUnitInAutomat = 0;
                uint nCompInAutomat = 0;
                for( auto it2 = automaton.vSubUnits_.begin(); nUnitInAutomat == 0 || it2 != automaton.vSubUnits_.end(); )
                {
                    ADN_Automata_Data::UnitInfos* pUnit = 0;
                    ADN_Rich_ListViewItem* pUnitItem = 0;

                    uint nUnit = 0;
                    if( nUnitInAutomat == 0 && pPC )
                    {
                        pUnit = pPC;
                        ++nUnit;
                    }
                    else
                    {
                        pUnit = *it2;
                        ++it2;
                        if( pUnit == pPC )
                            continue;
                        nUnit += pUnit->min_.GetData();
                    }
                    assert( pUnit->GetCrossedElement() );
                    if( !pUnit->GetCrossedElement() )
                        continue;
                    ADN_Units_Data::UnitInfos& unit = *pUnit->GetCrossedElement();
                    nUnitInAutomat += nUnit;

                    // Component
                    uint nCompInUnit = 0;
                    for( auto it3 = unit.vComposantes_.begin(); it3 != unit.vComposantes_.end(); ++it3 )
                    {
                        T_Entry compTotal;
                        Context context( pAutomatItem, pUnitItem, pCatItem, automaton, nUnitInAutomat, nUnit, unit, compTotal );
                        ADN_Rich_ListViewItem* compItem = FillComponentItem( context, category, **it3 );
                        if( compItem )
                        {
                            AddEntryToTotal( compTotal, unitTotal_, (*it3)->nNb_.GetData() );
                            nCompInUnit += (*it3)->nNb_.GetData();
                        }
                    }
                    if( pUnitItem )
                    {
                        pUnitItem->setText( eColumnNbrComp, QString::number( nCompInUnit ) );
                        AddEntryToTotal( unitTotal_, automatTotal_, nUnit );
                        FillTotalItem( *pUnitItem, unitTotal_ );
                    }
                    unitTotal_.Clear();
                    nCompInAutomat += nCompInUnit * nUnit;
                }
                if( pAutomatItem )
                {
                pAutomatItem->setText( eColumnNbrUnit, QString::number( nUnitInAutomat ) );
                pAutomatItem->setText( eColumnNbrComp, QString::number( nCompInAutomat ) );
                AddEntryToTotal( automatTotal_, categoryTotal_, 1 );
                FillTotalItem( *pAutomatItem, automatTotal_ );
                }
                automatTotal_.Clear();
            }
            AddEntryToTotal( categoryTotal_, dotationTotal_, 1 );
            FillTotalItem( *pCatItem, categoryTotal_ );
            categoryTotal_.Clear();
        }
        FillTotalItem( *pDotationItem, dotationTotal_ );
        dotationTotal_.Clear();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLogCategory_ListView::AddEntryToTotal
// Created: SBO 2006-01-11
// -----------------------------------------------------------------------------
void ADN_AutomatLogCategory_ListView::AddEntryToTotal( const T_Entry& entry, T_Entry& total, uint nArity )
{
    total.rNbr_                     += entry.rNbr_                     * nArity;
    total.rNormalizedConsumption_   += entry.rNormalizedConsumption_   * nArity;
    total.rMoveAutonomy_            = std::min( total.rMoveAutonomy_         , entry.rMoveAutonomy_ );
    total.rEngineStoppedAutonomy_   = std::min( total.rEngineStoppedAutonomy_, entry.rEngineStoppedAutonomy_ );
    total.rEngineStartedAutonomy_   = std::min( total.rEngineStartedAutonomy_, entry.rEngineStartedAutonomy_ );
    total.rMoveQuantityUsedPerHour_          += entry.rMoveQuantityUsedPerHour_          * nArity;
    total.rEngineStoppedQuantityUsedPerHour_ += entry.rEngineStoppedQuantityUsedPerHour_ * nArity;
    total.rEngineStartedQuantityUsedPerHour_ += entry.rEngineStartedQuantityUsedPerHour_ * nArity;
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLogCategory_ListView::FillTotalItem
// Created: SBO 2006-01-11
// -----------------------------------------------------------------------------
void ADN_AutomatLogCategory_ListView::FillTotalItem( ADN_Rich_ListViewItem& item, const T_Entry& total ) const
{
    item.setText( eColumnContenance     , QString::number( total.rNbr_, 'f', 2 ) );
    item.setText( eColumnNormalizedConso, QString::number( total.rNormalizedConsumption_, 'f', 2 ) );
    item.setText( eColumnMoveConso         , QString::number( total.rMoveQuantityUsedPerHour_, 'f', 2 ) );
    item.setText( eColumnEngineStoppedConso, QString::number( total.rEngineStoppedQuantityUsedPerHour_, 'f', 2 ) );
    item.setText( eColumnEngineStartedConso, QString::number( total.rEngineStartedQuantityUsedPerHour_, 'f', 2 ) );

    if( total.rMoveAutonomy_ > 0. )
        item.SetValueGreaterThan( eColumnMoveAutonomy, total.rMoveAutonomy_, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
    else
        item.SetValueGreaterThan( eColumnMoveAutonomy, std::numeric_limits< double >::max(), 2., 3., ADN_Rich_ListViewItem::eUnitHour );

    if( total.rEngineStoppedAutonomy_ > 0 )
        item.SetValueGreaterThan( eColumnEngineStoppedAutonomy, total.rEngineStoppedAutonomy_, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
    else
        item.SetValueGreaterThan( eColumnEngineStoppedAutonomy, std::numeric_limits< double >::max(), 2., 3., ADN_Rich_ListViewItem::eUnitHour );

    if( total.rEngineStartedAutonomy_ > 0 )
        item.SetValueGreaterThan( eColumnEngineStartedAutonomy, total.rEngineStartedAutonomy_, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
    else
        item.SetValueGreaterThan( eColumnEngineStartedAutonomy, std::numeric_limits< double >::max(), 2., 3., ADN_Rich_ListViewItem::eUnitHour );
}
