// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreList.h"
#include "moc_ScoreList.cpp"
#include "ScoreEditor.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/SubObjectName.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWidget.h"
#include "indicators/Primitives.h"
#include "preparation/Score_ABC.h"
#include "preparation/ScoresModel.h"
#include "tools/ExerciseConfig.h"

Q_DECLARE_METATYPE( const Score_ABC* )

// -----------------------------------------------------------------------------
// Name: ScoreList constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreList::ScoreList( kernel::Controllers& controllers, ScoresModel& model, const tools::ExerciseConfig& config,
                      const StaticModel& staticModel, gui::GLView_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder, const QString& objectName )
    : controllers_( controllers )
    , model_      ( model )
    , scores_     ( new gui::RichWidget< QTreeWidget >( "scores", this ) )
    , editor_     ( new ScoreEditor( this, controllers, model, staticModel, tools, builder ) )
    , config_     ( config )
{
    setObjectName( objectName );
    gui::SubObjectName subObject( objectName );
    scores_->setHeaderLabels( QStringList( tr( "Name" ) ) );
    scores_->setRootIsDecorated( false );

    //edit delete box
    Q3HBox* box = new Q3HBox( this );
    gui::RichPushButton* editButton = new gui::RichPushButton( "edit", tr( "Edit..." ), box );
    gui::RichPushButton* deleteButton = new gui::RichPushButton( "delete", tr( "Delete" ), box );
    connect( editButton, SIGNAL( clicked() ), SLOT( OnEdit() ) );
    connect( deleteButton, SIGNAL( clicked() ), SLOT( OnDelete() ) );

    //generate box
    generatorBox_ = new Q3HBox( this );
    gui::RichPushButton* defaultIndicators = new gui::RichPushButton( "defaultIndicators", tr( "Generate default indicators" ), generatorBox_ );
    connect( defaultIndicators, SIGNAL( clicked() ), SLOT( OnGenerate() ) );
    generatorBox_->hide();

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->setAlignment( Qt::AlignRight );
    mainLayout->addWidget( scores_ );
    mainLayout->addWidget( box );
    mainLayout->addWidget( generatorBox_ );

    connect( scores_, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), SLOT( OnEdit() ) );
    connect( editor_, SIGNAL( Show() ), SIGNAL( Hide() ) );
    connect( editor_, SIGNAL( Hide() ), SIGNAL( Show() ) );
    controllers_.Register( *this );

}

// -----------------------------------------------------------------------------
// Name: ScoreList destructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreList::~ScoreList()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::OnEdit
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::OnEdit()
{
    if( Score_ABC* score = FindSelected() )
        editor_->StartEdit( *score );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::OnDelete
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::OnDelete()
{
    // $$$$ SBO 2009-04-20: prompt confirmation
    if( const Score_ABC* score = FindSelected() )
        emit ScoreDeleted( *score );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::OnGenerate
// Created: JSR 2011-02-08
// -----------------------------------------------------------------------------
void ScoreList::OnGenerate()
{
    if( config_.GetOptionalPhysicalChildFile( "scores" ).Exists() )
        model_.GenerateScoresFromTemplate( config_.GetPhyLoader() );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::NotifyCreated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::NotifyCreated( const Score_ABC& element )
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText( 0, element.GetName() );
    item->setData( 0, Qt::UserRole + 1, QVariant::fromValue( &element ) );
    scores_->addTopLevelItem( item );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::NotifyUpdated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::NotifyUpdated( const Score_ABC& element )
{
    for( int i = 0; i < scores_->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = scores_->topLevelItem( i );
        if( item->data( 0, Qt::UserRole + 1 ).value< const Score_ABC* >() == &element )
            item->setText( 0, element.GetName() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreList::NotifyDeleted
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::NotifyDeleted( const Score_ABC& element )
{
    for( int i = 0; i < scores_->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = scores_->topLevelItem( i );
        if( item->data( 0, Qt::UserRole + 1 ).value< const Score_ABC* >() == &element )
        {
            delete item;
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreList::FindSelected
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
Score_ABC* ScoreList::FindSelected() const
{
    if( QTreeWidgetItem* selected = scores_->currentItem() )
        return const_cast< Score_ABC* >( selected->data( 0, Qt::UserRole + 1 ).value< const Score_ABC* >() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ScoreList::Load
// Created: ABR 2011-05-27
// -----------------------------------------------------------------------------
void ScoreList::Load()
{
    if( config_.GetOptionalPhysicalChildFile( "scores" ).Exists() )
        generatorBox_->show();
    else
        generatorBox_->hide();
}

// -----------------------------------------------------------------------------
// Name: ScoreList::Draw
// Created: FPO 2011-07-19
// -----------------------------------------------------------------------------
void ScoreList::Draw( gui::Viewport_ABC& viewport )
{
    editor_->Draw( viewport );
}
