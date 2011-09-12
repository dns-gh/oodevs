// *****************************************************************************
//
// ADN_AutomatLogCategory_ListView file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-04 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AutomatLogCategory_ListView.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 15:53 $
// $Revision: 3 $
// $Workfile: ADN_AutomatLogCategory_ListView.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Composantes_Data.h"
#include "ADN_AutomatLogCategory_ListView.h"

#include "ADN_Automata_Data.h"
#include "ADN_Rich_ListViewItem.h"

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLogCategory_ListView constructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_AutomatLogCategory_ListView::ADN_AutomatLogCategory_ListView( ADN_Automata_Data& data, QWidget* pParent )
: ADN_ListView  ( pParent )
, data_         ( data )
, compTotal_    ()
, unitTotal_    ()
, automatTotal_ ()
, categoryTotal_()
, dotationTotal_()

{
    setRootIsDecorated( true );
    setAllColumnsShowFocus( true );

    BuildHeaders();
    BuildBody   ();
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
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Item > Automat > Unit > Comp" ) );
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Units" ) );
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Equipments" ) );
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Resources" ) );
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Norm. Consumption" ) );
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Consumption\nMoving" ) );
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Autonomy\nMoving" ) );
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Consumption\nParked engine stopped" ) );
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Autonomy\nParked engine Stopped" ) );
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Consumption\nParked engine running" ) );
    addColumn( tools::translate( "ADN_AutomatLogCategory_ListView", "Autonomy\nParked engine running" ) );
    for( int i = 1; i < columns(); ++i )
        setColumnAlignment( i, Qt::AlignVCenter | Qt::AlignRight );
}

// -----------------------------------------------------------------------------
// Name: ADN_AutomatLogCategory_ListView::BuildBody
// Created: SBO 2006-01-05
// -----------------------------------------------------------------------------
void ADN_AutomatLogCategory_ListView::BuildBody()
{
    // Dotation (eg. ration)
    ADN_Equipement_Data::T_ResourceInfos_Vector& dotations = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotations();
    for( ADN_Equipement_Data::IT_ResourceInfos_Vector itDotation = dotations.begin(); itDotation != dotations.end(); ++itDotation )
    {
        ADN_Equipement_Data::ResourceInfos& dotation = **itDotation;
        ADN_Rich_ListViewItem* pDotationItem = new ADN_Rich_ListViewItem( this, true );
        pDotationItem->setText( eColumnTarget, dotation.strName_.GetData().c_str() );

        // Dotation category (eg. bouffe)
        ADN_Equipement_Data::T_CategoryInfos_Vector& categories = dotation.GetCategories();
        for( ADN_Equipement_Data::IT_CategoryInfos_Vector itCat = categories.begin(); itCat != categories.end(); ++itCat )
        {
            ADN_Equipement_Data::CategoryInfo& category = **itCat;
            ADN_Rich_ListViewItem* pCatItem = new ADN_Rich_ListViewItem( pDotationItem, true );
            pCatItem->setText( eColumnTarget, category.strName_.GetData().c_str() );

            // Automat
            for( ADN_Automata_Data::IT_AutomatonInfosVector it = data_.vAutomata_.begin(); it != data_.vAutomata_.end(); ++it )
            {
                ADN_Automata_Data::AutomatonInfos& automaton = **it;
                ADN_Rich_ListViewItem* pAutomatItem = new ADN_Rich_ListViewItem( pCatItem, true );
                pAutomatItem->setText( eColumnTarget, automaton.strName_.GetData().c_str() );

                // Unit
                uint nUnitInAutomat = 0;
                uint nCompInAutomat = 0;
                for( ADN_Automata_Data::IT_UnitInfosVector it2 = automaton.vSubUnits_.begin(); nUnitInAutomat == 0 || it2 != automaton.vSubUnits_.end(); )
                {
                    ADN_Automata_Data::UnitInfos* pUnitInfos = 0;
                    ADN_Units_Data::UnitInfos*    pUnit;
                    ADN_Rich_ListViewItem* pUnitItem = new ADN_Rich_ListViewItem( pAutomatItem, true );

                    uint nUnit = 0;
                    if( nUnitInAutomat == 0 )
                    {
                        pUnit = automaton.ptrUnit_.GetData();
                        ++nUnit;
                    }
                    else
                    {
                        pUnitInfos = *it2;
                        assert( pUnitInfos->ptrUnit_.GetData() != 0 );
                        pUnit = pUnitInfos->ptrUnit_.GetData();
                        ++it2;
                        nUnit += pUnitInfos->min_.GetData();
                    }
                    ADN_Units_Data::UnitInfos& unit = *pUnit;
                    if( nUnitInAutomat == 0 )
                        pUnitItem->setText( eColumnTarget, QString( "[PC] - " ) + unit.strName_.GetData().c_str() );
                    else
                        pUnitItem->setText( eColumnTarget, unit.strName_.GetData().c_str() );
                    pUnitItem->setText( eColumnNbrUnit, QString::number( nUnit ) );
                    nUnitInAutomat += nUnit;

                    // Component
                    uint nCompInUnit = 0;
                    for( ADN_Units_Data::IT_ComposanteInfos_Vector it3 = unit.vComposantes_.begin(); it3 != unit.vComposantes_.end(); ++it3 )
                    {
                        ADN_Rich_ListViewItem* pCompItem = new ADN_Rich_ListViewItem( pUnitItem, true );
                        FillComponentItem( *pCompItem, category, **it3 );
                        AddEntryToTotal( compTotal_, unitTotal_, (*it3)->nNb_.GetData() );
                        compTotal_.Clear();
                        nCompInUnit += (*it3)->nNb_.GetData();
                    }
                    pUnitItem->setText( eColumnNbrComp, QString::number( nCompInUnit ) );
                    AddEntryToTotal( unitTotal_, automatTotal_, nUnit );
                    FillTotalItem( *pUnitItem, unitTotal_ );
                    unitTotal_.Clear();
                    nCompInAutomat += nCompInUnit * nUnit;
                }
                pAutomatItem->setText( eColumnNbrUnit, QString::number( nUnitInAutomat ) );
                pAutomatItem->setText( eColumnNbrComp, QString::number( nCompInAutomat ) );
                AddEntryToTotal( automatTotal_, categoryTotal_, 1 );
                FillTotalItem( *pAutomatItem, automatTotal_ );
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
// Name: ADN_AutomatLogCategory_ListView::FillComponentItem
// Created: SBO 2006-01-12
// -----------------------------------------------------------------------------
void ADN_AutomatLogCategory_ListView::FillComponentItem( ADN_Rich_ListViewItem&             item,
                                                         ADN_Equipement_Data::CategoryInfo& category,
                                                         ADN_Units_Data::ComposanteInfos&   comp )
{
    item.setText( eColumnTarget, comp.ptrComposante_.GetData()->strName_.GetData().c_str() );
    item.setText( eColumnNbrComp, QString::number( comp.nNb_.GetData() ) );

    // retrieve corresponding dotation category
    ADN_Composantes_Data::T_CategoryInfos_Vector& categories = comp.ptrComposante_.GetData()->resources_.categories_;
    ADN_Composantes_Data::IT_CategoryInfos_Vector itCategory = categories.begin();
    for( ; itCategory != categories.end(); ++itCategory )
        if( (*itCategory)->ptrCategory_.GetData() == &category )
            break;
    if( itCategory == categories.end() )
        return;
    item.setText( eColumnContenance     , QString::number( (*itCategory)->rNbr_.GetData(), 'f', 2 ) );
    item.setText( eColumnNormalizedConso, QString::number( (*itCategory)->rNormalizedConsumption_.GetData(), 'f', 2 ) );
    compTotal_.rNbr_                   = (*itCategory)->rNbr_.GetData();
    compTotal_.rNormalizedConsumption_ = (*itCategory)->rNormalizedConsumption_.GetData();

    // consumption
    ADN_Composantes_Data::T_ConsumptionItem_Vector& consumptions = comp.ptrComposante_.GetData()->consumptions_.vConsumptions_;
    for( ADN_Composantes_Data::IT_ConsumptionItem_Vector itConso = consumptions.begin(); itConso != consumptions.end(); ++itConso )
    {
        ADN_Composantes_Data::ConsumptionItem& conso = **itConso;
        if( conso.ptrCategory_.GetData() == &category )
        {
            if( conso.nConsumptionType_ != eMoving && conso.nConsumptionType_ != eEngineStopped )
                continue;

            double rAutonomy = std::numeric_limits< double >::max();
            if( conso.nQuantityUsedPerHour_.GetData() > 0 )
                rAutonomy = (*itCategory)->rNbr_.GetData() / conso.nQuantityUsedPerHour_.GetData();

            if( conso.nConsumptionType_ == eMoving )
            {
                item.SetValueGreaterThan( eColumnMoveAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
                item.setText( eColumnMoveConso, QString::number( conso.nQuantityUsedPerHour_.GetData(), 'f', 2 ) );
                compTotal_.rMoveAutonomy_            = rAutonomy;
                compTotal_.rMoveQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
            }
            else if( conso.nConsumptionType_ == eEngineStopped )
            {
                item.SetValueGreaterThan( eColumnEngineStoppedAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
                item.setText( eColumnEngineStoppedConso, QString::number( conso.nQuantityUsedPerHour_.GetData(), 'f', 2 ) );
                compTotal_.rEngineStoppedAutonomy_            = rAutonomy;
                compTotal_.rEngineStoppedQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
            }
            else if( conso.nConsumptionType_ == eEngineStarted )
            {
                item.SetValueGreaterThan( eColumnEngineStartedAutonomy, rAutonomy, 2., 3., ADN_Rich_ListViewItem::eUnitHour );
                item.setText( eColumnEngineStartedConso, QString::number( conso.nQuantityUsedPerHour_.GetData(), 'f', 2 ) );
                compTotal_.rEngineStartedAutonomy_            = rAutonomy;
                compTotal_.rEngineStartedQuantityUsedPerHour_ = conso.nQuantityUsedPerHour_.GetData();
            }
        }
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
