// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_PopulationKnowledgePanel.h"
#include "moc_MOS_PopulationKnowledgePanel.cpp"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_MainWindow.h"
#include "MOS_Tools.h"
#include "MOS_Agent.h"
#include "MOS_AgentManager.h"
#include "MOS_Team.h"
#include "MOS_Gtia.h"
#include "MOS_Population.h"
#include "MOS_PopulationKnowledge.h"
#include "MOS_PopulationConcentration.h"
#include "MOS_PopulationConcentrationKnowledge.h"
#include "MOS_PopulationFlow.h"
#include "MOS_PopulationFlowKnowledge.h"
#include "MOS_ActionContext.h"
#include "MOS_Display.h"
#include "MOS_DisplayGroup.h"
#include "MOS_DisplayItem.h"

#include <qtable.h>

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel constructor
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
MOS_PopulationKnowledgePanel::MOS_PopulationKnowledgePanel( QWidget* pParent )
    : MOS_InfoPanel_ABC                ( pParent )
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

    display_ = new MOS_Display( this );
    display_->AddGroup( "Détails" )
                .AddItem( "Id:" )
                .AddItem( "Population associée:" )
                .AddItem( "Camp:" );

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
    connect( &MOS_App::GetApp(), SIGNAL( PopulationKnowledgeCreated( MOS_Gtia&, MOS_PopulationKnowledge& ) ),
             this,                 SLOT( OnKnowledgeCreated( MOS_Gtia&, MOS_PopulationKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationKnowledgeUpdated( MOS_Gtia&, MOS_PopulationKnowledge& ) ),
             this,                 SLOT( OnKnowledgeUpdated( MOS_Gtia&, MOS_PopulationKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationKnowledgeDeleted( MOS_Gtia&, MOS_PopulationKnowledge& ) ),
             this,                 SLOT( OnKnowledgeDeleted( MOS_Gtia&, MOS_PopulationKnowledge& ) ) );
    // concentrations
    connect( &MOS_App::GetApp(), SIGNAL( PopulationConcentrationKnowledgeCreated( MOS_Gtia&, MOS_PopulationConcentrationKnowledge& ) ),
             this,                 SLOT( OnConcentrationKnowledgeCreated( MOS_Gtia&, MOS_PopulationConcentrationKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationConcentrationKnowledgeUpdated( MOS_Gtia&, MOS_PopulationConcentrationKnowledge& ) ),
             this,                 SLOT( OnConcentrationKnowledgeUpdated( MOS_Gtia&, MOS_PopulationConcentrationKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationConcentrationKnowledgeDeleted( MOS_Gtia&, MOS_PopulationConcentrationKnowledge& ) ),
             this,                 SLOT( OnConcentrationKnowledgeDeleted( MOS_Gtia&, MOS_PopulationConcentrationKnowledge& ) ) );
    // flow
    connect( &MOS_App::GetApp(), SIGNAL( PopulationFlowKnowledgeCreated( MOS_Gtia&, MOS_PopulationFlowKnowledge& ) ),
             this,                 SLOT( OnFlowKnowledgeCreated( MOS_Gtia&, MOS_PopulationFlowKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationFlowKnowledgeUpdated( MOS_Gtia&, MOS_PopulationFlowKnowledge& ) ),
             this,                 SLOT( OnFlowKnowledgeUpdated( MOS_Gtia&, MOS_PopulationFlowKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationFlowKnowledgeDeleted( MOS_Gtia&, MOS_PopulationFlowKnowledge& ) ),
             this,                 SLOT( OnFlowKnowledgeDeleted( MOS_Gtia&, MOS_PopulationFlowKnowledge& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );
    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) )              , &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );
    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) )                  , &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );

    connect( pOwnTeamCheckBox_,   SIGNAL( clicked() )                         , this, SLOT( ToggleDisplayOwnTeam() ) ); 
    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );
    connect( pKnowledgeListView_, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel destructor
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
MOS_PopulationKnowledgePanel::~MOS_PopulationKnowledgePanel()
{
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnUpdate()
{
    if( selectedItem_ == MOS_SelectedElement() || GetSelectedGtia() == 0 )
        OnClearSelection();
    else
        UpdateList();
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::OnClearSelection
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnClearSelection()
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
// Name: MOS_PopulationKnowledgePanel::UpdateList
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::UpdateList()
{
    MOS_Gtia* pGtia = GetSelectedGtia();

    if( pGtia == 0 )
        return;

    if( pGtia_ != pGtia )
    {
        MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* pParentItem = 0;

        pKnowledgeListView_->clear();
        const MOS_Gtia::T_PopulationKnowledgeMap& knowledges = pGtia->GetPopulationKnowledges();
        for( MOS_Gtia::CIT_PopulationKnowledgeMap it = knowledges.begin(); it != knowledges.end(); ++it )
        {
            // We only display knowledges concerning our own team if the appropriate check box is checked.
            if( pOwnTeamCheckBox_->isChecked() || pGtia->GetTeam().GetID() != it->second->GetPopulation().GetTeam().GetID() )
            {
                pParentItem = new MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >( it->second, pKnowledgeListView_, it->second->GetPopulation().GetName().c_str() );
                const MOS_PopulationKnowledge::T_ConcentrationKnowledgeMap& concentrationKnowledges = it->second->GetConcentrations();
                for( MOS_PopulationKnowledge::CIT_ConcentrationKnowledgeMap itC = concentrationKnowledges.begin(); itC != concentrationKnowledges.end(); ++itC )
                {
                    new MT_ValuedListViewItem< MOS_PopulationConcentrationKnowledge*, eConcentration >( itC->second, pParentItem, tr( "Concentration - " ) + QString::number( itC->second->GetID() ) );
                    if( itC->second == pSelectedConcentrationKnowledge_ )
                    {
                        disconnect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
                        pKnowledgeListView_->setSelected( pKnowledgeListView_->lastItem(), true );
                        connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
                    }
                }
                const MOS_PopulationKnowledge::T_FlowKnowledgeMap& flowKnowledges = it->second->GetFlows();
                for( MOS_PopulationKnowledge::CIT_FlowKnowledgeMap itF = flowKnowledges.begin(); itF != flowKnowledges.end(); ++itF )
                {
                    new MT_ValuedListViewItem< MOS_PopulationFlowKnowledge*, eFlow >( itF->second, pParentItem, tr( "Flux - " ) + QString::number( itF->second->GetID() ) );
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
// Name: MOS_PopulationKnowledgePanel::UpdateSelected
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::UpdateSelected()
{
    display_->Clear();
    display_->Group( "Concentration" ).hide();
    display_->Group( "Flux" ).hide();

    pFlowPartBox_->hide   ();

    if( pSelectedKnowledge_ == 0 )
        return;
    MOS_PopulationKnowledge& k = *pSelectedKnowledge_;

    display_->Group( "Détails" )
                .Display( "Id:", MOS_Display::Id( k.GetID() ) )
                .Display( "Population associée:", MOS_Display::Id( k.GetPopulation().GetID() ) )
                .Display( "Camp:", k.GetTeam() ? k.GetTeam()->GetName() : "" );

    if( pSelectedConcentrationKnowledge_ != 0 )
    {
        MOS_PopulationConcentrationKnowledge& ck = *pSelectedConcentrationKnowledge_;
        std::string strPos;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( ck.GetPosition(), strPos );
        display_->Group( "Concentration" )
                    .Display( "Id:", MOS_Display::Id( ck.GetID() ) )
                    .Display( "Concentration associée:", MOS_Display::Id( ck.GetConcentration() ? ck.GetConcentration()->GetID() : 0 ) )
                    .Display( "Position:", strPos )
                    .Display( "Humains vivants:", IfSet( ck.nNbrAliveHumans_ ) )
                    .Display( "Humains morts:", IfSet( ck.nNbrDeadHumans_ ) )
                    .Display( "Attitude:", IfSet( ck.eAttitude_, ENT_Tr::ConvertFromPopulationAttitude( ck.eAttitude_ ) ) )
                    .Display( "Percue:", IfSet( ck.bIsPerceived_, MOS_Display::YesNo( ck.bIsPerceived_ ) ) )
                    .Display( "Pertinence:", ck.rRelevance_ );
    }
    else if( pSelectedFlowKnowledge_ != 0 )
    {
        MOS_PopulationFlowKnowledge& fk = *pSelectedFlowKnowledge_;

        display_->Group( "Flux" )
                .Display( "Id:", MOS_Display::Id( fk.GetID() ) )
                .Display( "Flux associée:", MOS_Display::Id( fk.GetFlow() ? fk.GetFlow()->GetID() : 0 ) )
                .Display( "Direction:", IfSet( fk.rDirection_ ) )
                .Display( "Vitesse:", IfSet( fk.rSpeed_ ) )
                .Display( "Humains vivants:", IfSet( fk.nNbrAliveHumans_ ) )
                .Display( "Humains morts:", IfSet( fk.nNbrDeadHumans_ ) )
                .Display( "Attitude:", IfSet( fk.eAttitude_, ENT_Tr::ConvertFromPopulationAttitude( fk.eAttitude_ ) ) )
                .Display( "Percue:", IfSet( fk.bIsPerceived_, MOS_Display::YesNo( fk.bIsPerceived_ ) ) )
                .Display( "Portions connues:", IfSet( fk.flowParts_, QString::number( fk.flowParts_.Data().size() ) ) );

        if( fk.flowParts_.IsSet() && ! fk.flowParts_.Data().empty() )
        {
            const MOS_PopulationFlowKnowledge::T_FlowParts& parts = fk.flowParts_.Data();
            pFlowPartTable_->setNumRows( parts.size() );
            uint nNbr = 0;
            for( MOS_PopulationFlowKnowledge::CIT_FlowParts it = parts.begin(); it != parts.end(); ++it, ++nNbr )
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
// Name: MOS_PopulationKnowledgePanel::OnSelectionChanged
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnSelectionChanged( QListViewItem* pItem )
{
    if( pItem != 0 )
    {
        if( pItem->rtti() == ePopulation )
        {
            pSelectedKnowledge_ = ( ( MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* )pItem )->GetValue();
            pSelectedConcentrationKnowledge_ = 0;
            pSelectedFlowKnowledge_ = 0;
        }
        else if( pItem->rtti() == eConcentration )
        {
            MT_ValuedListViewItem< MOS_PopulationConcentrationKnowledge*, eConcentration >* pCastConcentration;
            pCastConcentration = ( MT_ValuedListViewItem< MOS_PopulationConcentrationKnowledge*, eConcentration >* )pItem;
            pSelectedConcentrationKnowledge_ = pCastConcentration->GetValue();
            QListViewItem* pParent = pCastConcentration->parent();
            pSelectedKnowledge_ = ( ( MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* )pParent )->GetValue();
            pSelectedFlowKnowledge_ = 0;
        }
        else if( pItem->rtti() == eFlow )
        {
            MT_ValuedListViewItem< MOS_PopulationFlowKnowledge*, eFlow >* pCastFlow;
            pCastFlow = ( MT_ValuedListViewItem< MOS_PopulationFlowKnowledge*, eFlow >* )pItem;
            pSelectedFlowKnowledge_ = pCastFlow->GetValue();
            QListViewItem* pParent = pCastFlow->parent();
            pSelectedKnowledge_ = ( ( MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* )pParent )->GetValue();            
            pSelectedConcentrationKnowledge_ = 0;
        }
        else
        {
            assert( false );
        }
        MOS_SelectedElement selectedElement( *pSelectedKnowledge_ );
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
// Name: MOS_PopulationKnowledgePanel::OnRequestCenter
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnRequestCenter()
{
    QListViewItem* pItem = pKnowledgeListView_->selectedItem();
    if( pItem == 0 )
        return;
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::OnKnowledgeCreated
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnKnowledgeCreated( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    // We only display knowledges concerning our own team if the appropriate check box is checked.
    if( pOwnTeamCheckBox_->isChecked() || gtia.GetTeam().GetID() != knowledge.GetPopulation().GetTeam().GetID() )
        new MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >( &knowledge, pKnowledgeListView_, knowledge.GetPopulation().GetName().c_str() );
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::OnKnowledgeUpdated
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnKnowledgeUpdated( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;
    if( &knowledge != pSelectedKnowledge_ )
       return;
    // force list update
    UpdateList();
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::OnKnowledgeDeleted
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* pCastItem = ( MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* )pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            delete pCastItem;
            return;
        }

        pItem = pItem->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::OnConcentrationKnowledgeCreated
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnConcentrationKnowledgeCreated( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    const MOS_PopulationKnowledge* pParentKnowledge = knowledge.GetPopulationKnowledge();
    assert( pParentKnowledge );

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* pCastItem = ( MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* )pItem;
        if( pCastItem->GetValue() == pParentKnowledge )
        {
            // We only display knowledges concerning our own team if the appropriate check box is checked.
            if( pOwnTeamCheckBox_->isChecked() || gtia.GetTeam().GetID() != pParentKnowledge->GetPopulation().GetTeam().GetID() )
                new MT_ValuedListViewItem< MOS_PopulationConcentrationKnowledge*, eConcentration >( &knowledge, pCastItem, QString( knowledge.GetConcentration()->GetName().c_str() ) + QString( " - " ) + QString::number( knowledge.GetID() ) );
            return;
        }
        pItem = pItem->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::OnConcentrationKnowledgeUpdated
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnConcentrationKnowledgeUpdated( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;
    if( pSelectedConcentrationKnowledge_ == &knowledge )
        UpdateSelected();
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::OnConcentrationKnowledgeDeleted
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnConcentrationKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    const MOS_PopulationKnowledge* pParentKnowledge = knowledge.GetPopulationKnowledge();
    assert( pParentKnowledge );

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* pCastItem = ( MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* )pItem;
        if( pCastItem->GetValue() == pParentKnowledge )
        {
            QListViewItem* pChild = pCastItem->firstChild();
            while( pChild != 0 )
            {
                MT_ValuedListViewItem< MOS_PopulationConcentrationKnowledge*, eConcentration >* pCastChild = ( MT_ValuedListViewItem< MOS_PopulationConcentrationKnowledge*, eConcentration >* )pChild;
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
// Name: MOS_PopulationKnowledgePanel::OnFlowKnowledgeCreated
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnFlowKnowledgeCreated( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    const MOS_PopulationKnowledge* pParentKnowledge = knowledge.GetPopulationKnowledge();
    assert( pParentKnowledge );

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* pCastItem = ( MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* )pItem;
        if( pCastItem->GetValue() == pParentKnowledge )
        {
            // We only display knowledges concerning our own team if the appropriate check box is checked.
            if( pOwnTeamCheckBox_->isChecked() || gtia.GetTeam().GetID() != pParentKnowledge->GetPopulation().GetTeam().GetID() )
                new MT_ValuedListViewItem< MOS_PopulationFlowKnowledge*, eFlow >( &knowledge, pCastItem, QString( knowledge.GetFlow()->GetName().c_str() ) + QString( " - " ) + QString::number( knowledge.GetID() ) );
            return;
        }
        pItem = pItem->nextSibling();
    }
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::OnFlowKnowledgeUpdated
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnFlowKnowledgeUpdated( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;
    if( pSelectedFlowKnowledge_ == &knowledge )
        UpdateSelected();
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::OnFlowKnowledgeDeleted
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnFlowKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge )
{
    if( &gtia != GetSelectedGtia() )
        return;

    const MOS_PopulationKnowledge* pParentKnowledge = knowledge.GetPopulationKnowledge();
    assert( pParentKnowledge );

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* pCastItem = ( MT_ValuedListViewItem< MOS_PopulationKnowledge*, ePopulation >* )pItem;
        if( pCastItem->GetValue() == pParentKnowledge )
        {
            QListViewItem* pChild = pCastItem->firstChild();
            while( pChild != 0 )
            {
                MT_ValuedListViewItem< MOS_PopulationFlowKnowledge*, eFlow >* pCastChild = ( MT_ValuedListViewItem< MOS_PopulationFlowKnowledge*, eFlow >* )pChild;
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
// Name: MOS_PopulationKnowledgePanel::OnContextMenuRequested
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos )
{
    if( pItem == 0 ) 
        return;

    pPopupMenu_->clear();

    MT_ValuedListViewItem< MOS_PopulationKnowledge* >* pCastItem = ( MT_ValuedListViewItem< MOS_PopulationKnowledge* >* )pItem;
    MOS_SelectedElement selectedElement( *( pCastItem->GetValue() ) );
    emit NewPopupMenu( *pPopupMenu_, MOS_ActionContext( selectedElement ) );

    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::ToggleDisplayOwnTeam
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledgePanel::ToggleDisplayOwnTeam()
{
    UpdateList();
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledgePanel::GetSelectedGtia
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
MOS_Gtia* MOS_PopulationKnowledgePanel::GetSelectedGtia()
{
    if( selectedItem_.pAgent_ )
        return & selectedItem_.pAgent_->GetGtia();
    else if( selectedItem_.pGtia_ )
        return selectedItem_.pGtia_;
    else if( selectedItem_.pPopulationKnowledge_ )
        return selectedItem_.pPopulationKnowledge_->GetGtia();
    return 0;
}
