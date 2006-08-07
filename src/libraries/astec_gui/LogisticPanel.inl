// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: LogisticPanel constructor
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
template< typename ConcretePanel, typename Consign >
LogisticPanel< ConcretePanel, Consign >::LogisticPanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory, const QString& tabName )
    : InfoPanel_ABC( pParent, tabName )
    , selected_( controllers )
{
    pConsignListView_ = new ListDisplayer< ConcretePanel >( this, (ConcretePanel&)*this, factory );
    pConsignListView_->AddColumn( "Demandes logistiques" );
    pConsignListView_->AddColumn( "" );

    pConsignHandledListView_ = new ListDisplayer< ConcretePanel >( this, (ConcretePanel&)*this, factory );
    pConsignHandledListView_->AddColumn( "Consignes en traitement" );
    pConsignHandledListView_->AddColumn( "" );

    logDisplay_ = new SubItemDisplayer( "Consigne :", factory );
    logDisplay_->AddChild( "Pion demandeur :" )
                .AddChild( "Pion traitant :" );
}

// -----------------------------------------------------------------------------
// Name: LogisticPanel destructor
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
template< typename ConcretePanel, typename Consign >
LogisticPanel< ConcretePanel, Consign >::~LogisticPanel()
{
    delete logDisplay_;
}

// -----------------------------------------------------------------------------
// Name: LogisticPanel::AddConsignColumn
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
template< typename ConcretePanel, typename Consign >
void LogisticPanel< ConcretePanel, Consign >::AddConsignColumn( const char* column )
{
    logDisplay_->AddChild( column );
}

// -----------------------------------------------------------------------------
// Name: LogisticPanel::showEvent
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
template< typename ConcretePanel, typename Consign >
void LogisticPanel< ConcretePanel, Consign >::showEvent( QShowEvent* )
{
    const Agent_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: LogisticPanel::NotifySelected
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
template< typename ConcretePanel, typename Consign >
void LogisticPanel< ConcretePanel, Consign >::NotifySelected( const Agent_ABC* agent )
{
    if( ! agent || agent != selected_ )
    {
        selected_ = agent;
        if( selected_ )
        {
            pConsignListView_->hide();
            pConsignHandledListView_->hide();
            NotifySelected( *selected_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPanel::NotifyUpdated
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
template< typename ConcretePanel, typename Consign >
void LogisticPanel< ConcretePanel, Consign >::NotifyUpdated( const LogisticConsigns& consigns )
{
    if( ! ShouldUpdate( consigns ) )
        return;

    DisplayRequested( consigns, pConsignListView_ );
    DisplayHandled( consigns, pConsignHandledListView_ );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticPanel::NotifyUpdated
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
template< typename ConcretePanel, typename Consign >
void LogisticPanel< ConcretePanel, Consign >::NotifyUpdated( const Consign& consign )
{
    ValuedListItem* item = FindItem( &consign, pConsignListView_->firstChild() );
    if( ! item )
        item = FindItem( &consign, pConsignHandledListView_->firstChild() );
    if( item )
        consign.Display( (*logDisplay_)( item ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticPanel::ShouldUpdate
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
template< typename ConcretePanel, typename Consign >
template< typename Extension >
bool LogisticPanel< ConcretePanel, Consign >::ShouldUpdate( const Extension& e )
{
    return IsVisible() && selected_ && selected_->Retrieve< Extension >() == &e ;
}

// -----------------------------------------------------------------------------
// Name: LogisticPanel::GetDisplayer
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
template< typename ConcretePanel, typename Consign >
Displayer_ABC& LogisticPanel< ConcretePanel, Consign >::GetDisplayer( ValuedListItem* item )
{
    return (*logDisplay_)( item );
}
