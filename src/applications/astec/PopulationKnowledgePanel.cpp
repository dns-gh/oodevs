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
#include "moc_PopulationKnowledgePanel.cpp"

#include "App.h"
#include "World.h"
#include "MainWindow.h"
#include "Tools.h"
#include "Agent.h"
#include "AgentManager.h"
#include "Team.h"
#include "Gtia.h"
#include "Population.h"
#include "PopulationKnowledge.h"
#include "PopulationConcentration.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlow.h"
#include "PopulationFlowKnowledge.h"
#include "ActionContext.h"
#include "Display.h"
#include "DisplayGroup.h"
#include "DisplayItem.h"

#include <qtable.h>

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel constructor
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
PopulationKnowledgePanel::PopulationKnowledgePanel( QWidget* pParent )
    : InfoPanel_ABC                ( pParent )
    , pPopupMenu_                      ( new QPopupMenu( this ) )
    , pSelectedKnowledge_              ( 0 )
    , pSelectedConcentrationKnowledge_ ( 0 )
    , pSelectedFlowKnowledge_          ( 0 )
    , pGtia_                           ( 0 )
{
    pKnowledgeListView_ = new QListView( this );
    pKnowledgeListView_->addColumn( tr( "Populations connues" ) );
    pKnowledgeListView_->setResizeMode( QListView::LastColumn );

    pOwnTeamCheckBox_ = new QCheckBox( tr( "Afficher propre camp" ), this );
    pOwnTeamCheckBox_->setChecked( true );

    display_ = new Display( this );
    display_->AddGroup( "Détails" )
                .AddItem( "Id:" )
                .AddItem( "Population associée:" )
                .AddItem( "Camp:" )
                .AddItem( "Domination:" );
            
    display_->AddGroup( "Concentration" )
                .AddItem( "Id:" )
                .AddItem( "Concentration associée:" )
                .AddItem( "Position:" )
                .AddItem( "Humains vivants:" )
                .AddItem( "Humains morts:" )
                .AddItem( "Attitude:" )
                .AddItem( "Percue:" )
                .AddItem( "Pertinence:" );

    display_->AddGroup( "Flux" )
                .AddItem( "Id:" )
                .AddItem( "Flux associée:" )
                .AddItem( "Direction:" )
                .AddItem( "Vitesse:" )
                .AddItem( "Humains vivants:" )
                .AddItem( "Humains morts:" )
                .AddItem( "Attitude:" )
                .AddItem( "Percue:" )
                .AddItem( "Portions connues:" );

    pFlowPartBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Portions de flux" ), this );
    pFlowPartTable_ = new QTable( 0, 2, pFlowPartBox_ );
    pFlowPartTable_->horizontalHeader()->setLabel( 0, tr( "#" ) );
    pFlowPartTable_->horizontalHeader()->setLabel( 1, tr( "Pertinence" ) );
    pFlowPartTable_->setColumnWidth   ( 0, 20 );
    pFlowPartTable_->setColumnReadOnly( 0, true );
    pFlowPartTable_->setColumnReadOnly( 1, true );
    pFlowPartTable_->setLeftMargin( 0 );
    pFlowPartTable_->setShowGrid( false );
    pFlowPartTable_->setMaximumHeight( 100 );

    pFlowPartBox_->hide();

    // Perception Agents
    pPerceptionListView_ = new QListView( this );
    pPerceptionListView_->addColumn( tr( "Agent" ) );
    pPerceptionListView_->addColumn( tr( "Niveau perception" ) );
    pPerceptionListView_->setResizeMode( QListView::LastColumn );
    pPerceptionListView_->setAllColumnsShowFocus( true );

    // populations
    connect( &App::GetApp(), SIGNAL( PopulationKnowledgeCreated( Gtia&, PopulationKnowledge& ) ),
             this,                 SLOT( OnKnowledgeCreated( Gtia&, PopulationKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationKnowledgeUpdated( Gtia&, PopulationKnowledge& ) ),
             this,                 SLOT( OnKnowledgeUpdated( Gtia&, PopulationKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationKnowledgeDeleted( Gtia&, PopulationKnowledge& ) ),
             this,                 SLOT( OnKnowledgeDeleted( Gtia&, PopulationKnowledge& ) ) );
    // concentrations
    connect( &App::GetApp(), SIGNAL( PopulationConcentrationKnowledgeCreated( Gtia&, PopulationConcentrationKnowledge& ) ),
             this,                 SLOT( OnConcentrationKnowledgeCreated( Gtia&, PopulationConcentrationKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationConcentrationKnowledgeUpdated( Gtia&, PopulationConcentrationKnowledge& ) ),
             this,                 SLOT( OnConcentrationKnowledgeUpdated( Gtia&, PopulationConcentrationKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationConcentrationKnowledgeDeleted( Gtia&, PopulationConcentrationKnowledge& ) ),
             this,                 SLOT( OnConcentrationKnowledgeDeleted( Gtia&, PopulationConcentrationKnowledge& ) ) );
    // flow
    connect( &App::GetApp(), SIGNAL( PopulationFlowKnowledgeCreated( Gtia&, PopulationFlowKnowledge& ) ),
             this,                 SLOT( OnFlowKnowledgeCreated( Gtia&, PopulationFlowKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationFlowKnowledgeUpdated( Gtia&, PopulationFlowKnowledge& ) ),
             this,                 SLOT( OnFlowKnowledgeUpdated( Gtia&, PopulationFlowKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationFlowKnowledgeDeleted( Gtia&, PopulationFlowKnowledge& ) ),
             this,                 SLOT( OnFlowKnowledgeDeleted( Gtia&, PopulationFlowKnowledge& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ) );
    connect( this, SIGNAL( ElementSelected( SelectedElement& ) )              , &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ) );
    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) )                  , &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );

    connect( pOwnTeamCheckBox_,   SIGNAL( clicked() )                         , this, SLOT( ToggleDisplayOwnTeam() ) ); 
    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );
    connect( pKnowledgeListView_, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
}


// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel destructor
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
PopulationKnowledgePanel::~PopulationKnowledgePanel()
{
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnUpdate()
{
    if( selectedItem_ == SelectedElement() || GetSelectedGtia() == 0 )
        OnClearSelection();
    else
        UpdateList();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnClearSelection
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnClearSelection()
{
    pGtia_ = 0;
    pKnowledgeListView_->clear();
    display_->Clear();
    pSelectedKnowledge_              = 0;
    pSelectedConcentrationKnowledge_ = 0;
    pSelectedFlowKnowledge_          = 0;
    UpdateSelected();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::UpdateList
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::UpdateList()
{
    Gtia* pGtia = GetSelectedGtia();

    if( pGtia == 0 )
        return;

    if( pGtia_ != pGtia )
    {
        MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* pParentItem = 0;

        pKnowledgeListView_->clear();
        const Gtia::T_PopulationKnowledgeMap& knowledges = pGtia->GetPopulationKnowledges();
        for( Gtia::CIT_PopulationKnowledgeMap it = knowledges.begin(); it != knowledges.end(); ++it )
        {
            // We only display knowledges concerning our own team if the appropriate check box is checked.
            if( pOwnTeamCheckBox_->isChecked() || pGtia->GetTeam().GetID() != it->second->GetPopulation().GetTeam().GetID() )
            {
                pParentItem = new MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >( it->second, pKnowledgeListView_, it->second->GetPopulation().GetName().c_str() );
                const PopulationKnowledge::T_ConcentrationKnowledgeMap& concentrationKnowledges = it->second->GetConcentrations();
                for( PopulationKnowledge::CIT_ConcentrationKnowledgeMap itC = concentrationKnowledges.begin(); itC != concentrationKnowledges.end(); ++itC )
                {
                    new MT_ValuedListViewItem< PopulationConcentrationKnowledge*, eConcentration >( itC->second, pParentItem, tr( "Concentration - " ) + QString::number( itC->second->GetID() ) );
                    if( itC->second == pSelectedConcentrationKnowledge_ )
                    {
                        disconnect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
                        pKnowledgeListView_->setSelected( pKnowledgeListView_->lastItem(), true );
                        connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
                    }
                }
                const PopulationKnowledge::T_FlowKnowledgeMap& flowKnowledges = it->second->GetFlows();
                for( PopulationKnowledge::CIT_FlowKnowledgeMap itF = flowKnowledges.begin(); itF != flowKnowledges.end(); ++itF )
                {
                    new MT_ValuedListViewItem< PopulationFlowKnowledge*, eFlow >( itF->second, pParentItem, tr( "Flux - " ) + QString::number( itF->second->GetID() ) );
                    if( itF->second == pSelectedFlowKnowledge_ )
                    {
                        disconnect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
                        pKnowledgeListView_->setSelected( pKnowledgeListView_->lastItem(), true );
                        connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
                    }
                }
            }
        }
        pGtia_ = pGtia;
    }

    if( pKnowledgeListView_->childCount() == 0 )
    {
        pSelectedKnowledge_              = 0;
        pSelectedConcentrationKnowledge_ = 0;
        pSelectedFlowKnowledge_          = 0;
        UpdateSelected();
    }
}

namespace
{
    // $$$$ AGE 2006-02-09: HIé
    template< typename T >
    QString IfSet( const T& value, const QString& message )
    {
        return value.IsSet() ? message : "";
    }
    template< typename T >
    QString IfSet( const T& value, const std::string& message )
    {
        return value.IsSet() ? message.c_str() : "";
    }
    template< typename T >
    QString IfSet( const T& value )
    {
        return value.IsSet() ? QString::number( value ) : "";
    }
}


// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::UpdateSelected
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::UpdateSelected()
{
    display_->Clear();
    display_->Group( "Concentration" ).hide();
    display_->Group( "Flux" ).hide();

    pFlowPartBox_->hide   ();

    if( pSelectedKnowledge_ == 0 )
        return;
    PopulationKnowledge& k = *pSelectedKnowledge_;

    display_->Group( "Détails" )
                .Display( "Id:", Display::Id( k.GetID() ) )
                .Display( "Population associée:", Display::Id( k.GetPopulation().GetID() ) )
                .Display( "Camp:", k.GetTeam() ? k.GetTeam()->GetName() : "" )
                .Display( "Domination:", IfSet( k.nDomination_, QString::number( k.nDomination_ ) + "%" ) ) ;

    if( pSelectedConcentrationKnowledge_ != 0 )
    {
        PopulationConcentrationKnowledge& ck = *pSelectedConcentrationKnowledge_;
        std::string strPos;
        App::GetApp().GetWorld().SimToMosMgrsCoord( ck.GetPosition(), strPos );
        display_->Group( "Concentration" )
                    .Display( "Id:", Display::Id( ck.GetID() ) )
                    .Display( "Concentration associée:", Display::Id( ck.GetConcentration() ? ck.GetConcentration()->GetID() : 0 ) )
                    .Display( "Position:", strPos )
                    .Display( "Humains vivants:", IfSet( ck.nNbrAliveHumans_ ) )
                    .Display( "Humains morts:", IfSet( ck.nNbrDeadHumans_ ) )
                    .Display( "Attitude:", IfSet( ck.eAttitude_, ENT_Tr::ConvertFromPopulationAttitude( ck.eAttitude_ ) ) )
                    .Display( "Percue:", IfSet( ck.bIsPerceived_, Display::YesNo( ck.bIsPerceived_ ) ) )
                    .Display( "Pertinence:", ck.rRelevance_ );
    }
    else if( pSelectedFlowKnowledge_ != 0 )
    {
        PopulationFlowKnowledge& fk = *pSelectedFlowKnowledge_;

        display_->Group( "Flux" )
                .Display( "Id:", Display::Id( fk.GetID() ) )
                .Display( "Flux associée:", Display::Id( fk.GetFlow() ? fk.GetFlow()->GetID() : 0 ) )
                .Display( "Direction:", IfSet( fk.rDirection_ ) )
                .Display( "Vitesse:", IfSet( fk.rSpeed_ ) )
                .Display( "Humains vivants:", IfSet( fk.nNbrAliveHumans_ ) )
                .Display( "Humains morts:", IfSet( fk.nNbrDeadHumans_ ) )
                .Display( "Attitude:", IfSet( fk.eAttitude_, ENT_Tr::ConvertFromPopulationAttitude( fk.eAttitude_ ) ) )
                .Display( "Percue:", IfSet( fk.bIsPerceived_, Display::YesNo( fk.bIsPerceived_ ) ) )
                .Display( "Portions connues:", IfSet( fk.flowParts_, QString::number( fk.flowParts_.Data().size() ) ) );

        if( fk.flowParts_.IsSet() && ! fk.flowParts_.Data().empty() )
        {
            const PopulationFlowKnowledge::T_FlowParts& parts = fk.flowParts_.Data();
            pFlowPartTable_->setNumRows( parts.size() );
            uint nNbr = 0;
            for( PopulationFlowKnowledge::CIT_FlowParts it = parts.begin(); it != parts.end(); ++it, ++nNbr )
            {
                // try to recycle previous cells
                if( pFlowPartTable_->item( nNbr, 0 ) )
                {
                    pFlowPartTable_->item( nNbr, 0 )->setText( QString::number( nNbr ) );
                    pFlowPartTable_->item( nNbr, 1 )->setText( QString::number( it->rRelevance_ ) );
                }
                else
                {
                    QTableItem* pItem = new QTableItem( pFlowPartTable_, QTableItem::EditType::Never );
                    pItem->setText( QString::number( nNbr ) );
                    pFlowPartTable_->setItem( nNbr, 0, pItem );
                    pItem = new QTableItem( pFlowPartTable_, QTableItem::EditType::Never );
                    pItem->setText( QString::number( it->rRelevance_ ) );
                    pFlowPartTable_->setItem( nNbr, 1, pItem );
                }
            }
            pFlowPartBox_->show();
        }
    }
}


// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnSelectionChanged
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnSelectionChanged( QListViewItem* pItem )
{
    if( pItem != 0 )
    {
        if( pItem->rtti() == ePopulation )
        {
            pSelectedKnowledge_ = ( ( MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* )pItem )->GetValue();
            pSelectedConcentrationKnowledge_ = 0;
            pSelectedFlowKnowledge_ = 0;
        }
        else if( pItem->rtti() == eConcentration )
        {
            MT_ValuedListViewItem< PopulationConcentrationKnowledge*, eConcentration >* pCastConcentration;
            pCastConcentration = ( MT_ValuedListViewItem< PopulationConcentrationKnowledge*, eConcentration >* )pItem;
            pSelectedConcentrationKnowledge_ = pCastConcentration->GetValue();
            QListViewItem* pParent = pCastConcentration->parent();
            pSelectedKnowledge_ = ( ( MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* )pParent )->GetValue();
            pSelectedFlowKnowledge_ = 0;
        }
        else if( pItem->rtti() == eFlow )
        {
            MT_ValuedListViewItem< PopulationFlowKnowledge*, eFlow >* pCastFlow;
            pCastFlow = ( MT_ValuedListViewItem< PopulationFlowKnowledge*, eFlow >* )pItem;
            pSelectedFlowKnowledge_ = pCastFlow->GetValue();
            QListViewItem* pParent = pCastFlow->parent();
            pSelectedKnowledge_ = ( ( MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* )pParent )->GetValue();            
            pSelectedConcentrationKnowledge_ = 0;
        }
        else
        {
            assert( false );
        }
        SelectedElement selectedElement( *pSelectedKnowledge_ );
        emit ElementSelected( selectedElement );
    }
    else
    {
        pSelectedKnowledge_              = 0;
        pSelectedConcentrationKnowledge_ = 0;
        pSelectedFlowKnowledge_          = 0;
    }

    UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnRequestCenter
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnRequestCenter()
{
    QListViewItem* pItem = pKnowledgeListView_->selectedItem();
    if( pItem == 0 )
        return;
}


// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnKnowledgeCreated
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnKnowledgeCreated( Gtia& gtia, PopulationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    // We only display knowledges concerning our own team if the appropriate check box is checked.
    if( pOwnTeamCheckBox_->isChecked() || gtia.GetTeam().GetID() != knowledge.GetPopulation().GetTeam().GetID() )
        new MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >( &knowledge, pKnowledgeListView_, knowledge.GetPopulation().GetName().c_str() );
}


// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnKnowledgeUpdated
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnKnowledgeUpdated( Gtia& gtia, PopulationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;
    if( &knowledge != pSelectedKnowledge_ )
       return;
    // force list update
    UpdateList();
}


// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnKnowledgeDeleted
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnKnowledgeDeleted( Gtia& gtia, PopulationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* pCastItem = ( MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* )pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            delete pCastItem;
            return;
        }

        pItem = pItem->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnConcentrationKnowledgeCreated
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnConcentrationKnowledgeCreated( Gtia& gtia, PopulationConcentrationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    const PopulationKnowledge* pParentKnowledge = knowledge.GetPopulationKnowledge();
    assert( pParentKnowledge );

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* pCastItem = ( MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* )pItem;
        if( pCastItem->GetValue() == pParentKnowledge )
        {
            // We only display knowledges concerning our own team if the appropriate check box is checked.
            if( pOwnTeamCheckBox_->isChecked() || gtia.GetTeam().GetID() != pParentKnowledge->GetPopulation().GetTeam().GetID() )
                new MT_ValuedListViewItem< PopulationConcentrationKnowledge*, eConcentration >( &knowledge, pCastItem, QString( knowledge.GetConcentration()->GetName().c_str() ) + QString( " - " ) + QString::number( knowledge.GetID() ) );
            return;
        }
        pItem = pItem->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnConcentrationKnowledgeUpdated
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnConcentrationKnowledgeUpdated( Gtia& gtia, PopulationConcentrationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;
    if( pSelectedConcentrationKnowledge_ == &knowledge )
        UpdateSelected();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnConcentrationKnowledgeDeleted
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnConcentrationKnowledgeDeleted( Gtia& gtia, PopulationConcentrationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    const PopulationKnowledge* pParentKnowledge = knowledge.GetPopulationKnowledge();
    assert( pParentKnowledge );

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* pCastItem = ( MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* )pItem;
        if( pCastItem->GetValue() == pParentKnowledge )
        {
            QListViewItem* pChild = pCastItem->firstChild();
            while( pChild != 0 )
            {
                MT_ValuedListViewItem< PopulationConcentrationKnowledge*, eConcentration >* pCastChild = ( MT_ValuedListViewItem< PopulationConcentrationKnowledge*, eConcentration >* )pChild;
                if( pCastChild->GetValue() == &knowledge )
                {
                    delete pCastChild;
                    return;
                }
                pChild = pChild->nextSibling();
            }
        }
        pItem = pItem->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnFlowKnowledgeCreated
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnFlowKnowledgeCreated( Gtia& gtia, PopulationFlowKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    const PopulationKnowledge* pParentKnowledge = knowledge.GetPopulationKnowledge();
    assert( pParentKnowledge );

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* pCastItem = ( MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* )pItem;
        if( pCastItem->GetValue() == pParentKnowledge )
        {
            // We only display knowledges concerning our own team if the appropriate check box is checked.
            if( pOwnTeamCheckBox_->isChecked() || gtia.GetTeam().GetID() != pParentKnowledge->GetPopulation().GetTeam().GetID() )
                new MT_ValuedListViewItem< PopulationFlowKnowledge*, eFlow >( &knowledge, pCastItem, QString( knowledge.GetFlow()->GetName().c_str() ) + QString( " - " ) + QString::number( knowledge.GetID() ) );
            return;
        }
        pItem = pItem->nextSibling();
    }
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnFlowKnowledgeUpdated
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnFlowKnowledgeUpdated( Gtia& gtia, PopulationFlowKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;
    if( pSelectedFlowKnowledge_ == &knowledge )
        UpdateSelected();
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnFlowKnowledgeDeleted
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnFlowKnowledgeDeleted( Gtia& gtia, PopulationFlowKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    const PopulationKnowledge* pParentKnowledge = knowledge.GetPopulationKnowledge();
    assert( pParentKnowledge );

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* pCastItem = ( MT_ValuedListViewItem< PopulationKnowledge*, ePopulation >* )pItem;
        if( pCastItem->GetValue() == pParentKnowledge )
        {
            QListViewItem* pChild = pCastItem->firstChild();
            while( pChild != 0 )
            {
                MT_ValuedListViewItem< PopulationFlowKnowledge*, eFlow >* pCastChild = ( MT_ValuedListViewItem< PopulationFlowKnowledge*, eFlow >* )pChild;
                if( pCastChild->GetValue() == &knowledge )
                {
                    delete pCastChild;
                    return;
                }
                pChild = pChild->nextSibling();
            }
        }
        pItem = pItem->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnContextMenuRequested
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos )
{
    if( pItem == 0 ) 
        return;

    pPopupMenu_->clear();

    MT_ValuedListViewItem< PopulationKnowledge* >* pCastItem = ( MT_ValuedListViewItem< PopulationKnowledge* >* )pItem;
    SelectedElement selectedElement( *( pCastItem->GetValue() ) );
    emit NewPopupMenu( *pPopupMenu_, ActionContext( selectedElement ) );

    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::ToggleDisplayOwnTeam
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::ToggleDisplayOwnTeam()
{
    UpdateList();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::GetSelectedGtia
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
Gtia* PopulationKnowledgePanel::GetSelectedGtia()
{
    if( selectedItem_.pAgent_ )
        return & selectedItem_.pAgent_->GetGtia();
    else if( selectedItem_.pGtia_ )
        return selectedItem_.pGtia_;
    else if( selectedItem_.pPopulationKnowledge_ )
        return selectedItem_.pPopulationKnowledge_->GetGtia();
    return 0;
}
