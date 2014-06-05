// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamHumanWoundList.h"
#include "moc_ParamHumanWoundList.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/MedicalPriorities.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/resources.h"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamHumanWoundList::ParamHumanWoundList( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
    , list_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList destructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamHumanWoundList::~ParamHumanWoundList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamHumanWoundList::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QGridLayout* layout = new QGridLayout( group_ );
    group_->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );

    list_ = new QTreeView( parent );
    list_->setRootIsDecorated( false );
    list_->setEditTriggers( 0 );
    model_.setColumnCount( 1 );
    list_->setModel( &model_ );
    list_->setHeaderHidden( true );
    list_->setAllColumnsShowFocus( true );
    list_->setContextMenuPolicy( Qt::CustomContextMenu );
    list_->header()->setResizeMode( 0, QHeaderView::ResizeToContents );

    ::gui::RichPushButton* upBtn = new ::gui::RichPushButton( "upBtn", MAKE_ICON( arrow_up ), QString::null, parent );
    upBtn->setFixedSize( 32, 32 );
    ::gui::RichPushButton* downBtn = new ::gui::RichPushButton( "downBtn", MAKE_ICON( arrow_down ), QString::null, parent );
    downBtn->setFixedSize( 32, 32 );

    layout->addWidget( list_, 0, 0, 2, 1 );
    layout->addWidget( upBtn, 0, 1 );
    layout->addWidget( downBtn, 1, 1 );

    connect( upBtn, SIGNAL( clicked() ), SLOT( OnUp() ) );
    connect( upBtn, SIGNAL( clicked() ), SLOT( Update() ) );
    connect( downBtn, SIGNAL( clicked() ), SLOT( OnDown() ) );
    connect( downBtn, SIGNAL( clicked() ), SLOT( Update() ) );
    connect( list_, SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( OnContextMenu( const QPoint& ) ) );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::unique_ptr< actions::parameters::MedicalPriorities > param( new actions::parameters::MedicalPriorities( parameter_ ) );
    if( IsChecked() )
        for( int row = 0; row < model_.rowCount(); ++row )
        {
            const std::string text = model_.item( row )->text().toStdString();
            param->AddMedicalPriority( ENT_Tr::ConvertToHumanWound( text, ENT_Tr::eToTr ) );
        }
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnUp
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnUp()
{
    const QModelIndex index = list_->selectionModel()->currentIndex();
    if( !index.isValid() || index.row() == 0 )
        return;
    QList< QStandardItem* > currentSelection = model_.takeRow( index.row() );
    model_.insertRow( index.row() - 1, currentSelection );
    list_->selectionModel()->clearSelection();
    list_->selectionModel()->setCurrentIndex( model_.index( index.row() - 1, 0 ), QItemSelectionModel::Select );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnDown
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnDown()
{
    const QModelIndex index = list_->selectionModel()->currentIndex();
    if( !index.isValid() || index.row() == model_.rowCount() )
        return;
    QList< QStandardItem* > currentSelection = model_.takeRow( index.row() );
    model_.insertRow( index.row() + 1, currentSelection );
    list_->selectionModel()->clearSelection();
    list_->selectionModel()->setCurrentIndex( model_.index( index.row() + 1, 0 ) , QItemSelectionModel::Select );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnAdd
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnAdd( int value )
{
    model_.appendRow( new QStandardItem( tools::ToString( E_HumanWound( value ) ) ) );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnRemove
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnRemove()
{
    const QModelIndex index = list_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    model_.removeRow( index.row() );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnContextMenu
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnContextMenu( const QPoint& point )
{
    //general context menu
    kernel::ContextMenu* menu = new kernel::ContextMenu( list_ );
    connect( menu, SIGNAL( aboutToHide() ), menu, SLOT( deleteLater() ) );
    kernel::ContextMenu* items = new kernel::ContextMenu( menu );
    for( unsigned int i = eHumanWound_BlesseUrgence1; i < int( eNbrHumanWound ); ++i )
    {
        const QString name = tools::ToString( ( E_HumanWound )i );
        if( model_.findItems( name ).isEmpty() )
        {
            int id = items->insertItem( name, this, SLOT( OnAdd( int ) ) );
            items->setItemParameter( id, i );
        }
    }
    if( items->count() )
        menu->insertItem( tr( "Add..." ), items );

    //context menu on selected item
    const QModelIndex index = list_->selectionModel()->currentIndex();
    if( index.isValid() )
    {
        QStandardItem* item = model_.itemFromIndex( index );
        if ( item )
            menu->insertItem( tr( "Remove" ), this, SLOT( OnRemove() ) );
    }
    menu->popup( list_->viewport()->mapToGlobal( point ) );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::InternalCheckValidity
// Created: LDC 2009-11-12
// -----------------------------------------------------------------------------
bool ParamHumanWoundList::InternalCheckValidity() const
{
    return list_ && model_.rowCount() != 0;
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamHumanWoundList::Visit( const actions::parameters::MedicalPriorities& param )
{
    ActivateOptionalIfNeeded( param );
    model_.clear();
    QString value = param.GetValue();
    QStringList values = value.split( QRegExp( ", " ) );
    for( auto it = values.begin(); it != values.end(); ++it )
        model_.appendRow( new QStandardItem( *it ) );
}
