// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ChangeSuperiorDialog.h"
#include "moc_ChangeSuperiorDialog.cpp"
#include "ChangeLogisticLinksWidget.h"
#include "RichGroupBox.h"
#include "RichPushButton.h"
#include "SubObjectName.h"
#include "TaskerWidget.h"
#include "tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "preparation/TacticalHierarchies.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog constructor
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
ChangeSuperiorDialog::ChangeSuperiorDialog( kernel::Controllers& controllers,
                                            const gui::EntitySymbols& symbols,
                                            QWidget* parent /* = 0 */ )
    : QDialog( parent )
    , controllers_( controllers )
{
    SubObjectName subObject( "ChangeSuperiorDialog" );
    setMinimumWidth( 300 );

    // Content
    currentEntity_ = new gui::TaskerWidget( "current-entity", controllers, symbols, tr( "Moved entity" ), true, false );
    tacticalSuperior_ = new gui::TaskerWidget( "tactical-superior", controllers, symbols, tr( "Tactical superior" ), false, false );
    tacticalSuperior_->setVisible( false );
    knowledgeGroup_ = new gui::TaskerWidget( "knowledge-group", controllers, symbols, tr( "Knowledge group" ), false, false );
    knowledgeGroup_->setVisible( false );
    logisticSuperior_ = new gui::RichGroupBox( "logistic-superior", tr( "Logistic links" ) );
    logisticSuperior_->setVisible( false );
    connect( tacticalSuperior_, SIGNAL( Clicked( bool ) ), SLOT( OnContentChanged() ) );
    connect( tacticalSuperior_, SIGNAL( TaskerChanged( const kernel::Entity_ABC* ) ), SLOT( OnContentChanged() ) );
    connect( knowledgeGroup_, SIGNAL( Clicked( bool ) ), SLOT( OnContentChanged() ) );
    connect( knowledgeGroup_, SIGNAL( TaskerChanged( const kernel::Entity_ABC* ) ), SLOT( OnContentChanged() ) );
    connect( logisticSuperior_, SIGNAL( clicked( bool ) ), SLOT( OnContentChanged() ) );

    QVBoxLayout* logisticLayout = new QVBoxLayout( logisticSuperior_ );
    logisticLayout->setMargin( 0 );
    logisticLayout->setSpacing( 0 );
    logisticWidget_ = new ChangeLogisticLinksWidget( 0, controllers );
    logisticLayout->addWidget( logisticWidget_ );

    // Buttons
    okButton_ = new RichPushButton( "ok", tr( "Ok" ), this );
    okButton_->setDefault( true );
    auto cancelButton = new RichPushButton( "cancel", tr( "Cancel" ), this );
    connect( okButton_, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    // Layouts
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch( 1 );
    buttonLayout->addWidget( okButton_ );
    buttonLayout->addWidget( cancelButton );

    QVBoxLayout* main = new QVBoxLayout( this );
    main->addWidget( currentEntity_ );
    main->addWidget( tacticalSuperior_ );
    main->addWidget( knowledgeGroup_ );
    main->addWidget( logisticSuperior_ );
    main->addLayout( buttonLayout );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog destructor
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
ChangeSuperiorDialog::~ChangeSuperiorDialog()
{
    controllers_.Unregister( *this );
}

namespace
{
    template< typename T >
    void SetVisibleAndCheckable( T* widget, int flags, int mandatoryFlag, int optionalFlag )
    {
        bool optional = ( flags & optionalFlag ) != 0;
        widget->setVisible( ( flags & mandatoryFlag ) != 0 || optional );
        widget->setCheckable( optional );
        if( optional )
            widget->setChecked( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::Show
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::Show( kernel::Entity_ABC& entity,
                                 const QString& title,
                                 int options /* = eTacticalSuperior */ )
{
    setWindowTitle( title );
    if( currentEntity_->GetTasker() )
        logisticWidget_->AddItem( *currentEntity_->GetTasker() );
    currentEntity_->SetTasker( &entity );
    // visibility
    SetVisibleAndCheckable( tacticalSuperior_, options, eTacticalSuperior, eOptionalTacticalSuperior );
    SetVisibleAndCheckable( knowledgeGroup_, options, eKnowledgeGroup, eOptionalKnowledgeGroup );
    SetVisibleAndCheckable( logisticSuperior_, options, eLogisticSuperior, eOptionalLogisticSuperior );
    logisticWidget_->SetCurrentVisible( logisticCurrentExtractor_ );
    // initialisation
    tacticalSuperior_->SetTasker( 0 );
    knowledgeGroup_->SetTasker( 0 );   
    logisticWidget_->RemoveItem( entity );
    if( logisticNominalFunctors_ )
        logisticWidget_->SetNominalSuperior( logisticNominalFunctors_->extract_( entity ) );
    if( logisticCurrentExtractor_ )
        logisticWidget_->SetCurrentSuperior( ( *logisticCurrentExtractor_ )( entity ) );
    show();
    OnContentChanged(); // OnContentChanged use widget's visibility, so we need to call show before
}

namespace
{
    template< typename T >
    bool IsSuperiorActivated( const T& widget )
    {
        return widget.isVisible() && ( !widget.isCheckable() || widget.isChecked() );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::OnTacticalSelectionChanged
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::OnTacticalSelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements )
{
    if( currentEntity_->GetTasker() &&
        isVisible() &&
        IsSuperiorActivated( *tacticalSuperior_ ) )
        tacticalSuperior_->SetTasker( elements.size() > 0 ? elements.at( 0 ) : 0 );
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::OnKnowledgeGroupSelectionChanged
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::OnKnowledgeGroupSelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements )
{
    if( currentEntity_->GetTasker() &&
        isVisible() &&
        IsSuperiorActivated( *knowledgeGroup_ ) )
        knowledgeGroup_->SetTasker( elements.size() > 0 ? elements.at( 0 ) : 0 );
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::NotifyDeleted
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( isVisible() && ( !currentEntity_->GetTasker() || &entity == currentEntity_->GetTasker() ) )
        hide();
}

namespace
{
    template< typename T >
    void ApplyChanges( kernel::Entity_ABC& entity,
                       const T& widget,
                       const ChangeSuperiorDialog::T_OptionalFunctors& functors )
    {
        if( IsSuperiorActivated( widget ) )
        {
            if( !functors )
                throw MASA_EXCEPTION( "No functor defined to change superior" );
            functors->doChange_( entity, *widget.GetTasker() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::accept
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::accept()
{
    if( !currentEntity_->GetTasker() )
        throw MASA_EXCEPTION( "Invalid entity while trying to change superior" );
    kernel::Entity_ABC& entity = const_cast< kernel::Entity_ABC& >( *currentEntity_->GetTasker() );
    ApplyChanges( entity, *tacticalSuperior_, tacticalSuperiorFunctors_ );
    ApplyChanges( entity, *knowledgeGroup_, knowledgeGroupFunctors_ );
    if( IsSuperiorActivated( *logisticSuperior_ ) )
    {
        if( !logisticNominalFunctors_ )
            throw MASA_EXCEPTION( "No functor defined to change superior" );
        logisticNominalFunctors_->doChange_( entity,
                                             logisticWidget_->GetNominalSuperior(),
                                             logisticWidget_->GetCurrentSuperior() );
    }
    Clear();
    QDialog::accept();
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::reject
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::reject()
{
    Clear();
    QDialog::reject();
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::Clear
// Created: ABR 2014-06-03
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::Clear()
{
    if( auto tasker = currentEntity_->GetTasker() )
        logisticWidget_->AddItem( *tasker );
    currentEntity_->SetTasker( 0 );
}

namespace
{
    bool IsValidSuperior( const kernel::Entity_ABC& entity,
                          gui::TaskerWidget& tasker,
                          const boost::optional< ChangeSuperiorDialog::ChangeSuperiorFunctors >& functors )
    {

        bool canChange = !IsSuperiorActivated( tasker );
        auto superior = tasker.GetTasker();
        canChange |= superior && functors->canChange_( entity, *superior );
        tasker.EnableStaticWarning( !canChange, Qt::red );
        return canChange;
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::OnContentChanged
// Created: ABR 2014-05-28
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::OnContentChanged()
{
    const bool validTactical = IsValidSuperior( *currentEntity_->GetTasker(), *tacticalSuperior_, tacticalSuperiorFunctors_ );
    const bool validKnowledgeGroup = IsValidSuperior( *currentEntity_->GetTasker(), *knowledgeGroup_, knowledgeGroupFunctors_ );
    okButton_->setEnabled( currentEntity_->GetTasker() && validTactical && validKnowledgeGroup );
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::SetTacticalSuperiorFunctors
// Created: ABR 2014-05-28
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::SetTacticalSuperiorFunctors( const T_CanChangeFunctor& canChange,
                                                        const T_DoChangeFunctor& doChange )
{
    if( !tacticalSuperiorFunctors_ )
        tacticalSuperiorFunctors_ = ChangeSuperiorFunctors( canChange, doChange );
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::SetKnowledgeGroupFunctors
// Created: ABR 2014-05-28
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::SetKnowledgeGroupFunctors( const T_CanChangeFunctor& canChange,
                                                      const T_DoChangeFunctor& doChange )
{
    if( !knowledgeGroupFunctors_ )
        knowledgeGroupFunctors_ = ChangeSuperiorFunctors( canChange, doChange );
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::SetLogisticCurrentExtractor
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::SetLogisticCurrentExtractor( const T_LogisticSuperiorExtractor& currentExtractor )
{
    if( !logisticCurrentExtractor_ )
        logisticCurrentExtractor_ = currentExtractor;
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::SetLogisticNominalFunctors
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::SetLogisticNominalFunctors( const T_LogisticSuperiorExtractor& nominalExtractor,
                                                       const T_DoChangeLogisticFunctor& doChange )
{
    if( !logisticNominalFunctors_ )
        logisticNominalFunctors_ = ChangeLogisticFunctors( nominalExtractor, doChange );
}
