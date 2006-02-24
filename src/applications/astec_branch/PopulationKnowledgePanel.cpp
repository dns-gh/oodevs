// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationKnowledgePanel.h"

#include "DisplayBuilder.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "ListDisplayer.h"

#include "Controller.h"
#include "ActionController.h"

#include "KnowledgeGroup.h"
#include "PopulationKnowledges.h"
#include "PopulationKnowledge.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel constructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
PopulationKnowledgePanel::PopulationKnowledgePanel( InfoPanel* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC( pParent, "C. popu" )
    , selected_( 0 )
    , subSelected_( 0 )
    , selectedPart_( 0 )
{
    knowledgeList_ = new ListDisplayer< PopulationKnowledgePanel >( this, *this );
    knowledgeList_->AddColumn( "Populations connues" );

    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Détails" )
                .AddLabel( "Id:" )
                .AddLabel( "Population associée:" )
                .AddLabel( "Camp:" );

    display_->AddGroup( "Concentration" )
                .AddLabel( "Id:" )
                .AddLabel( "Concentration associée:" )
                .AddLabel( "Position:" )
                .AddLabel( "Humains vivants:" )
                .AddLabel( "Humains morts:" )
                .AddLabel( "Attitude:" )
                .AddLabel( "Percue:" )
                .AddLabel( "Pertinence:" );

    display_->AddGroup( "Flux" )
                .AddLabel( "Id:" )
                .AddLabel( "Flux associée:" )
                .AddLabel( "Direction:" )
                .AddLabel( "Vitesse:" )
                .AddLabel( "Humains vivants:" )
                .AddLabel( "Humains morts:" )
                .AddLabel( "Attitude:" )
                .AddLabel( "Percue:" )
                .AddLabel( "Portions connues:" );

    perceptionList_ = new ListDisplayer< PopulationKnowledgePanel >( this, *this );
    perceptionList_->AddColumn( "Agent" );
    perceptionList_->AddColumn( "Niveau perception" );

    controller.Register( *this );
    actionController.Register( *this );
}

//    pFlowPartBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Portions de flux" ), this );
//    pFlowPartTable_ = new QTable( 0, 2, pFlowPartBox_ );
//    pFlowPartTable_->horizontalHeader()->setLabel( 0, tr( "#" ) );
//    pFlowPartTable_->horizontalHeader()->setLabel( 1, tr( "Pertinence" ) );
//    pFlowPartTable_->setColumnWidth   ( 0, 20 );
//    pFlowPartTable_->setColumnReadOnly( 0, true );
//    pFlowPartTable_->setColumnReadOnly( 1, true );
//    pFlowPartTable_->setLeftMargin( 0 );
//    pFlowPartTable_->setShowGrid( false );
//    pFlowPartTable_->setMaximumHeight( 100 );
//    pFlowPartBox_->hide();

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel destructor
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
PopulationKnowledgePanel::~PopulationKnowledgePanel()
{
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::NotifyUpdated( const PopulationKnowledges& element )
{
    if( ! isVisible() || selected_ != &element )
        return;

    knowledgeList_->Display( element.CreateIterator(), knowledgeList_->firstChild() );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::Display( const PopulationKnowledge& knowlege, Displayer_ABC& displayer )
{
//    knowlege.DisplayInList( displayer );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::NotifyUpdated( const PopulationKnowledge& element )
{
    if( ! isVisible() || subSelected_ != &element )
        return;
//    element.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::NotifyUpdated( const PopulationFlowKnowledge& element )
{
//    if( ! isVisible() || selectedPart_ != &element )
        return;
//    element.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::NotifyUpdated( const PopulationConcentrationKnowledge& element )
{
//    if( ! isVisible() || selectedPart_ != &element )
        return;
    element.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::Select( const KnowledgeGroup* element )
{
    const PopulationKnowledges* k = element ? element->Retrieve< PopulationKnowledges >() : 0;
    if( ! k || k != selected_ )
    {
        selected_ = k;
        if( selected_ ) {
            Show();
            NotifyUpdated( *selected_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::UpdateList
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
//void PopulationKnowledgePanel::UpdateList()
//{
//    Gtia* pGtia = GetSelectedGtia();
//
//    if( pGtia == 0 )
//        return;
//
//    if( pGtia_ != pGtia )
//    {
//        MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* pParentItem = 0;
//
//        pKnowledgeListView_->clear();
//        const Gtia::T_PopulationKnowledgeMap& knowledges = pGtia->GetPopulationKnowledges();
//        for( Gtia::CIT_PopulationKnowledgeMap it = knowledges.begin(); it != knowledges.end(); ++it )
//        {
//            // We only display knowledges concerning our own team if the appropriate check box is checked.
//            if( pOwnTeamCheckBox_->isChecked() || pGtia->GetTeam().GetID() != it->second->GetPopulation().GetTeam().GetID() )
//            {
//                pParentItem = new MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >( it->second, pKnowledgeListView_, it->second->GetPopulation().GetName().c_str() );
//                const PopulationKnowledge::T_ConcentrationKnowledgeMap& concentrationKnowledges = it->second->GetConcentrations();
//                for( PopulationKnowledge::CIT_ConcentrationKnowledgeMap itC = concentrationKnowledges.begin(); itC != concentrationKnowledges.end(); ++itC )
//                {
//                    new MT_ValuedListViewItem< PopulationConcentrationKnowledge*, eConcentration >( itC->second, pParentItem, tr( "Concentration - " ) + QString::number( itC->second->GetID() ) );
//                    if( itC->second == pSelectedConcentrationKnowledge_ )
//                    {
//                        disconnect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
//                        pKnowledgeListView_->setSelected( pKnowledgeListView_->lastItem(), true );
//                        connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
//                    }
//                }
//                const PopulationKnowledge::T_FlowKnowledgeMap& flowKnowledges = it->second->GetFlows();
//                for( PopulationKnowledge::CIT_FlowKnowledgeMap itF = flowKnowledges.begin(); itF != flowKnowledges.end(); ++itF )
//                {
//                    new MT_ValuedListViewItem< PopulationFlowKnowledge*, eFlow >( itF->second, pParentItem, tr( "Flux - " ) + QString::number( itF->second->GetID() ) );
//                    if( itF->second == pSelectedFlowKnowledge_ )
//                    {
//                        disconnect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
//                        pKnowledgeListView_->setSelected( pKnowledgeListView_->lastItem(), true );
//                        connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
//                    }
//                }
//            }
//        }
//        pGtia_ = pGtia;
//    }
//
//    if( pKnowledgeListView_->childCount() == 0 )
//    {
//        pSelectedKnowledge_              = 0;
//        pSelectedConcentrationKnowledge_ = 0;
//        pSelectedFlowKnowledge_          = 0;
//        UpdateSelected();
//    }
//}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::UpdateSelected
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
//void PopulationKnowledgePanel::UpdateSelected()
//{
//    display_->Clear();
//    display_->Group( "Concentration" ).hide();
//    display_->Group( "Flux" ).hide();
//
//    pFlowPartBox_->hide   ();
//
//    if( pSelectedKnowledge_ == 0 )
//        return;
//    PopulationKnowledge& k = *pSelectedKnowledge_;
//
//    display_->Group( "Détails" )
//                .Display( "Id:", Display::Id( k.GetID() ) )
//                .Display( "Population associée:", Display::Id( k.GetPopulation().GetID() ) )
//                .Display( "Camp:", k.GetTeam() ? k.GetTeam()->GetName() : "" );
//
//    if( pSelectedConcentrationKnowledge_ != 0 )
//    {
//        PopulationConcentrationKnowledge& ck = *pSelectedConcentrationKnowledge_;
//        std::string strPos;
//        App::GetApp().GetWorld().SimToMosMgrsCoord( ck.GetPosition(), strPos );
//        display_->Group( "Concentration" )
//                    .Display( "Id:", Display::Id( ck.GetID() ) )
//                    .Display( "Concentration associée:", Display::Id( ck.GetConcentration() ? ck.GetConcentration()->GetID() : 0 ) )
//                    .Display( "Position:", strPos )
//                    .Display( "Humains vivants:", IfSet( ck.nNbrAliveHumans_ ) )
//                    .Display( "Humains morts:", IfSet( ck.nNbrDeadHumans_ ) )
//                    .Display( "Attitude:", IfSet( ck.eAttitude_, ENT_Tr::ConvertFromPopulationAttitude( ck.eAttitude_ ) ) )
//                    .Display( "Percue:", IfSet( ck.bIsPerceived_, Display::YesNo( ck.bIsPerceived_ ) ) )
//                    .Display( "Pertinence:", ck.rRelevance_ );
//    }
//    else if( pSelectedFlowKnowledge_ != 0 )
//    {
//        PopulationFlowKnowledge& fk = *pSelectedFlowKnowledge_;
//
//        display_->Group( "Flux" )
//                .Display( "Id:", Display::Id( fk.GetID() ) )
//                .Display( "Flux associée:", Display::Id( fk.GetFlow() ? fk.GetFlow()->GetID() : 0 ) )
//                .Display( "Direction:", IfSet( fk.rDirection_ ) )
//                .Display( "Vitesse:", IfSet( fk.rSpeed_ ) )
//                .Display( "Humains vivants:", IfSet( fk.nNbrAliveHumans_ ) )
//                .Display( "Humains morts:", IfSet( fk.nNbrDeadHumans_ ) )
//                .Display( "Attitude:", IfSet( fk.eAttitude_, ENT_Tr::ConvertFromPopulationAttitude( fk.eAttitude_ ) ) )
//                .Display( "Percue:", IfSet( fk.bIsPerceived_, Display::YesNo( fk.bIsPerceived_ ) ) )
//                .Display( "Portions connues:", IfSet( fk.flowParts_, QString::number( fk.flowParts_.Data().size() ) ) );
//
//        if( fk.flowParts_.IsSet() && ! fk.flowParts_.Data().empty() )
//        {
//            const PopulationFlowKnowledge::T_FlowParts& parts = fk.flowParts_.Data();
//            pFlowPartTable_->setNumRows( parts.size() );
//            uint nNbr = 0;
//            for( PopulationFlowKnowledge::CIT_FlowParts it = parts.begin(); it != parts.end(); ++it, ++nNbr )
//            {
//                // try to recycle previous cells
//                if( pFlowPartTable_->item( nNbr, 0 ) )
//                {
//                    pFlowPartTable_->item( nNbr, 0 )->setText( QString::number( nNbr ) );
//                    pFlowPartTable_->item( nNbr, 1 )->setText( QString::number( it->rRelevance_ ) );
//                }
//                else
//                {
//                    QTableItem* pItem = new QTableItem( pFlowPartTable_, QTableItem::EditType::Never );
//                    pItem->setText( QString::number( nNbr ) );
//                    pFlowPartTable_->setItem( nNbr, 0, pItem );
//                    pItem = new QTableItem( pFlowPartTable_, QTableItem::EditType::Never );
//                    pItem->setText( QString::number( it->rRelevance_ ) );
//                    pFlowPartTable_->setItem( nNbr, 1, pItem );
//                }
//            }
//            pFlowPartBox_->show();
//        }
//    }
//}