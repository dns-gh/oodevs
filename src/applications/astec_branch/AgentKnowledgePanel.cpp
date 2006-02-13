// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentKnowledgePanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 18:35 $
// $Revision: 8 $
// $Workfile: AgentKnowledgePanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "AgentKnowledgePanel.h"
#include "moc_AgentKnowledgePanel.cpp"

#include "App.h"
#include "MainWindow.h"
#include "Tools.h"
#include "Agent.h"
#include "AgentManager.h"
#include "Team.h"
#include "Gtia.h"
#include "AgentKnowledge.h"
#include "ActionContext.h"
#include "Display.h"
#include "DisplayGroup.h"
#include "DisplayItem.h"
#include "OptionalValue.h"
#include "Model.h"

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel constructor
/** @param  pParent 
*/
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
AgentKnowledgePanel::AgentKnowledgePanel( QWidget* pParent )
    : InfoPanel_ABC     ( pParent )
    , pPopupMenu_           ( new QPopupMenu( this ) )
    , pSelectedKnowledge_   ( 0 )
    , pGtia_                ( 0 )
    , display_              ( 0 )
{
    pKnowledgeListView_ = new QListView( this );
    pKnowledgeListView_->addColumn( tr( "Agents connus" ) );
    pKnowledgeListView_->setResizeMode( QListView::LastColumn );

    pOwnTeamCheckBox_ = new QCheckBox( tr( "Afficher propre camp" ), this );
    pOwnTeamCheckBox_->setChecked( true );

    display_ = new Display( this );
    display_->AddGroup( "Détails" )
                .AddItem( "Id:" )
                .AddItem( "Agent associé:" )
                .AddItem( "Position:" )
                .AddItem( "Direction:" )
                .AddItem( "Vitesse:" )
                .AddItem( "Etat ops.:" )
                .AddItem( "Niveau de perception:" )
                .AddItem( "Niveau max de perception:" )
                .AddItem( "Camp:" )
                .AddItem( "Niveau:" )
                .AddItem( "Arme:" )
                .AddItem( "Spécialisation:" )
                .AddItem( "Qualification:" )
                .AddItem( "Catégorie:" )
                .AddItem( "Mobilité:" )
                .AddItem( "Capacité mission:" )
                .AddItem( "Rendu:" )
                .AddItem( "Fait prisonnier:" )
                .AddItem( "Réfugiés pris en compte:" )
                .AddItem( "PC:" )
                .AddItem( "Pertinence:"  );

    pPerceptionListView_ = new QListView( this );
    pPerceptionListView_->addColumn( tr( "Agent" ) );
    pPerceptionListView_->addColumn( tr( "Niveau perception" ) );
    pPerceptionListView_->setResizeMode( QListView::LastColumn );
    pPerceptionListView_->setAllColumnsShowFocus( true );

    connect( &App::GetApp(), SIGNAL( AgentKnowledgeCreated( Gtia&, AgentKnowledge& ) ),
             this,                 SLOT(    OnKnowledgeCreated( Gtia&, AgentKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( AgentKnowledgeUpdated( Gtia&, AgentKnowledge& ) ),
             this,                 SLOT(    OnKnowledgeUpdated( Gtia&, AgentKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( AgentKnowledgeDeleted( Gtia&, AgentKnowledge& ) ),
             this,                 SLOT(    OnKnowledgeDeleted( Gtia&, AgentKnowledge& ) ) );

    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ) );
    connect( this, SIGNAL( ElementSelected( SelectedElement& ) ),         &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ) );
    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),             &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );

    connect( pOwnTeamCheckBox_,   SIGNAL( clicked() ),                          this, SLOT( ToggleDisplayOwnTeam() ) ); 
    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );
    connect( pKnowledgeListView_, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel destructor
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
AgentKnowledgePanel::~AgentKnowledgePanel()
{
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnUpdate()
{
    /*if( GetSelectedGtia( selectedItem_ ) == 0 )
        OnClearSelection();
    else*/
        UpdateList();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnClearSelection()
{
    pGtia_ = 0;
    pKnowledgeListView_->clear();
    display_->Clear();
    pSelectedKnowledge_ = 0;
    UpdateSelected();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::UpdateList
// Created: APE 2004-06-10
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::UpdateList()
{
    Gtia*           pGtia      = GetSelectedGtia( selectedItem_ );
    AgentKnowledge* pKnowledge = selectedItem_.pAgentKnowledge_;

    if( pGtia == 0 )
        return;

    if( pGtia_ != pGtia )
    {
        pKnowledgeListView_->clear();
        const Gtia::T_AgentKnowledgeMap& knowledges = pGtia->GetAgentKnowledges();
        for( Gtia::CIT_AgentKnowledgeMap it = knowledges.begin(); it != knowledges.end(); ++it )
        {
            // We only display knowledges concerning our own team if the appropriate check box is checked.
            if( pOwnTeamCheckBox_->isChecked() || pGtia->GetTeam().GetID() != (*it).second->GetRealAgent().GetTeam().GetID() )
                new MT_ValuedListViewItem<AgentKnowledge*>( (*it).second, pKnowledgeListView_, (*it).second->GetRealAgent().GetName().c_str() );
        }
        pGtia_ = pGtia;
    }

    // Try to select the appropriate knowledge.
    if( pKnowledge != 0 )
    {
        QListViewItem* pItem = pKnowledgeListView_->firstChild();
        while( pItem != 0 )
        {
            MT_ValuedListViewItem<AgentKnowledge*>* pCastItem = (MT_ValuedListViewItem<AgentKnowledge*>*)pItem;
            if( pCastItem->GetValue() == pKnowledge )
            {
                pKnowledgeListView_->setSelected( pItem, true );
                return;
            }
            pItem = pItem->nextSibling();
        }
    }

    if( pKnowledgeListView_->childCount() > 0 )
        pKnowledgeListView_->setSelected( pKnowledgeListView_->firstChild(), true );
    else
    {
        pSelectedKnowledge_ = 0;
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
// Name: AgentKnowledgePanel::UpdateSelected
/** @param  pKnowledge 
*/
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::UpdateSelected()
{
    display_->Clear();

    pPerceptionListView_->clear();

    if( pSelectedKnowledge_ == 0 )
        return;

    AgentKnowledge& k = *pSelectedKnowledge_;
    display_->Group( "Détails" )
                .Display( "Id:", Display::Id(  k.GetID() ) )
                .Display( "Agent associé:", Display::Id( k.GetRealAgent().GetID() ) )
                .Display( "Position:", IfSet( k.strPosition_, k.strPosition_ ) )
                .Display( "Direction:", IfSet( k.nDirection_ ) )
                .Display( "Vitesse:", IfSet( k.nSpeed_ ) )
                .Display( "Etat ops.:", IfSet( k.nEtatOps_ ) )
                .Display( "Niveau de perception:", IfSet( k.nCurrentPerceptionLevel_, Tools::ToString( k.nCurrentPerceptionLevel_ ) ) )
                .Display( "Niveau max de perception:", IfSet( k.nCurrentPerceptionLevel_, Tools::ToString( k.nMaxPerceptionLevel_ ) ) )
                .Display( "Camp:", IfSet( k.nTeam_, App::GetApp().GetModel().GetTeam( k.nTeam_).GetName() ) )
                .Display( "Niveau:", IfSet( k.nLevel_, ENT_Tr::ConvertFromNatureLevel( k.nLevel_ ) ) )
                .Display( "Arme:", IfSet( k.nWeapon_, ENT_Tr::ConvertFromUnitNatureWeapon( k.nWeapon_ ) ) )
                .Display( "Spécialisation:", IfSet( k.nSpecialization_, ENT_Tr::ConvertFromUnitNatureSpecialization( k.nSpecialization_ ) ) )
                .Display( "Qualification:", IfSet( k.nQualifier_, ENT_Tr::ConvertFromUnitNatureQualifier( k.nQualifier_ ) ) )
                .Display( "Catégorie:", IfSet( k.nCategory_, ENT_Tr::ConvertFromUnitNatureCategory( k.nCategory_ ) ) )
                .Display( "Mobilité:", IfSet( k.nMobility_, ENT_Tr::ConvertFromUnitNatureMobility( k.nMobility_ ) ) )
                .Display( "Capacité mission:", IfSet( k.nCapacity_, ENT_Tr::ConvertFromUnitCapaciteMission( k.nCapacity_ ) ) )
                .Display( "Rendu:", IfSet( k.bSurrendered_, Display::YesNo( k.bSurrendered_ ) ) )
                .Display( "Fait prisonnier:", IfSet( k.bPrisonner_, Display::YesNo( k.bPrisonner_ ) ) )
                .Display( "Réfugiés pris en compte:", IfSet( k.bRefugies_, Display::YesNo( k.bRefugies_ ) ) )
                .Display( "PC:", IfSet( k.bIsPC_, Display::YesNo( k.bIsPC_ ) ) )
                .Display( "Pertinence:", IfSet( k.nRelevance_ ) );

    if( k.automatePerceptionMap_.IsSet() )
    {
        for( AgentKnowledge::CIT_AutomatePerceptionMap it = k.automatePerceptionMap_.Data().begin(); it != k.automatePerceptionMap_.Data().end(); ++it )
            new QListViewItem( pPerceptionListView_, it->first->GetName().c_str(), Tools::ToString( it->second )  );
    }
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnSelectionChanged
/** @param  pItem 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnSelectionChanged( QListViewItem* pItem )
{
    if( pItem != 0 )
    {
        MT_ValuedListViewItem<AgentKnowledge*>* pCastItem = (MT_ValuedListViewItem<AgentKnowledge*>*)pItem;
        pSelectedKnowledge_ = pCastItem->GetValue();
        SelectedElement selectedElement( *pSelectedKnowledge_ );
        emit ElementSelected( selectedElement );
    }
    else
        pSelectedKnowledge_ = 0;

    UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnRequestCenter
// Created: APE 2004-06-10
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnRequestCenter()
{
    QListViewItem* pItem = pKnowledgeListView_->selectedItem();
    if( pItem == 0 )
        return;

    MT_ValuedListViewItem<AgentKnowledge*>* pCastItem = (MT_ValuedListViewItem<AgentKnowledge*>*)pItem;
    emit CenterOnPoint( pCastItem->GetValue()->GetPosition() );
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnKnowledgeCreated
/** @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnKnowledgeCreated( Gtia& gtia, AgentKnowledge& knowledge )
{
    if( &gtia != pGtia_ )
        return;

    // We only display knowledges concerning our own team if the appropriate check box is checked.
    if( pOwnTeamCheckBox_->isChecked() || gtia.GetTeam().GetID() != knowledge.GetRealAgent().GetTeam().GetID() )
        new MT_ValuedListViewItem<AgentKnowledge*>( &knowledge, pKnowledgeListView_, knowledge.GetRealAgent().GetName().c_str() );
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnKnowledgeUpdated
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnKnowledgeUpdated( Gtia& /*gtia*/, AgentKnowledge& knowledge )
{
    if( &knowledge != pSelectedKnowledge_ )
        return;
    UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnKnowledgeDeleted
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnKnowledgeDeleted( Gtia& gtia, AgentKnowledge& knowledge )
{
    if( &gtia != pGtia_ )
        return;

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem<AgentKnowledge*>* pCastItem = (MT_ValuedListViewItem<AgentKnowledge*>*)pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            if ( pSelectedKnowledge_ == &knowledge )
                pSelectedKnowledge_ = 0;
            delete pCastItem;
            break;
        }

        pItem = pItem->nextSibling();
    }
    UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnContextMenuRequested
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos )
{
    if( pItem == 0 ) 
        return;

    pPopupMenu_->clear();

    MT_ValuedListViewItem<AgentKnowledge*>* pCastItem = (MT_ValuedListViewItem<AgentKnowledge*>*)pItem;
    SelectedElement selectedElement( *(pCastItem->GetValue()) );
    emit NewPopupMenu( *pPopupMenu_, ActionContext( selectedElement ) );

    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::ToggleDisplayOwnTeam
// Created: APE 2004-05-27
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::ToggleDisplayOwnTeam()
{
    this->UpdateList();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::GetSelectedGtia
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
Gtia* AgentKnowledgePanel::GetSelectedGtia( const SelectedElement& /*item*/ )
{
    if( selectedItem_.pAgent_ )
        return & selectedItem_.pAgent_->GetGtia();
    else if( selectedItem_.pGtia_ )
        return selectedItem_.pGtia_;
    else if( selectedItem_.pAgentKnowledge_ )
        return & selectedItem_.pAgentKnowledge_->GetOwner();
    return 0;
}
