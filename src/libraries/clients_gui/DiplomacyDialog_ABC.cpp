// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::DiplomacyDialog_ABC */

#include "clients_gui_pch.h"
#include "DiplomacyDialog_ABC.h"
#include "moc_DiplomacyDialog_ABC.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

Q_DECLARE_METATYPE( const Karma* )

#define KarmaRole ( Qt::UserRole + 1 )
#define ValueSetRole ( Qt::UserRole + 2 )

namespace
{
    QColor GetColor( const Karma& karma )
    {
        if( karma == Karma::friend_ )
            return QColor( 200, 200, 255 );
        if( karma == Karma::enemy_ )
            return QColor( 255, 200, 200 );
        return QColor( 200, 255, 200 );
    }

    class DiplomacyItemDelegate : public QItemDelegate
    {
    public:
        DiplomacyItemDelegate()
        {
            // NOTHING
        }

        ~DiplomacyItemDelegate()
        {
            // NOTHING
        }

        virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const
        {
            if( index.row() == index.column() )
                return 0;
            QComboBox* editor = new QComboBox( parent );
            editor->addItem( Karma::friend_ .GetName(), QVariant::fromValue( static_cast< const Karma* >( &Karma::friend_ ) ) );
            editor->addItem( Karma::enemy_  .GetName(), QVariant::fromValue( static_cast< const Karma* >( &Karma::enemy_ ) ) );
            editor->addItem( Karma::neutral_.GetName(), QVariant::fromValue( static_cast< const Karma* >( &Karma::neutral_ ) ) );
            return editor;
        }

        virtual void setEditorData( QWidget *editor, const QModelIndex &index) const
        {
            QComboBox* comboBox = static_cast< QComboBox* >( editor );
            const Karma* karma = index.model()->data( index, KarmaRole ).value< const Karma* >();
            comboBox->setCurrentIndex( comboBox->findData( QVariant::fromValue( karma ) ) );
        }

        virtual void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
        {
            QComboBox* comboBox = static_cast< QComboBox* >( editor );
            const Karma* karma = comboBox->itemData( comboBox->currentIndex() ).value< const Karma* >();
            model->setData( index, karma->GetName(),  Qt::DisplayRole);
            model->setData( index, QVariant::fromValue( karma ), KarmaRole );
            model->setData( index, GetColor( *karma ), Qt::BackgroundColorRole );
            model->setData( index, true, ValueSetRole );
            QModelIndex opposite = model->index( index.column(), index.row() );
            if( opposite.isValid() && opposite.data( ValueSetRole ).toBool() == false )
            {
                model->setData( opposite, karma->GetName(),  Qt::DisplayRole);
                model->setData( opposite, QVariant::fromValue( karma ), KarmaRole );
                model->setData( opposite, GetColor( *karma ), Qt::BackgroundColorRole );
            }
        }

        virtual void updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /*index*/ ) const
        {
            editor->setGeometry( option.rect );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC constructor
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
DiplomacyDialog_ABC::DiplomacyDialog_ABC( QWidget* parent, Controllers& controllers, const Profile_ABC& profile )
    : ModalDialog( parent, "ChangeDiplomacyDialog" )
    , controllers_( controllers )
    , profile_    ( profile )
{
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    setCaption( tools::translate( "gui::DiplomacyDialog_ABC", "Diplomacy" ) );
    setMaximumSize( 1024, 768);

    // Table
    table_ = new QTableWidget( this );
    table_->setSelectionMode( QAbstractItemView::NoSelection );
    table_->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    table_->verticalHeader()->setResizeMode( QHeaderView::Stretch );
    table_->setItemDelegate( new DiplomacyItemDelegate() );

    // Buttons
    QPushButton* okBtn     = new QPushButton( tools::translate( "gui::DiplomacyDialog_ABC", "Ok" ), this );
    QPushButton* cancelBtn = new QPushButton( tools::translate( "gui::DiplomacyDialog_ABC", "Cancel" ), this );
    okBtn->setDefault( true );
    okBtn->setMaximumWidth( 100 );
    cancelBtn->setMaximumWidth( 100 );

    // Layouts
    Q3VBoxLayout* pMainLayout = new Q3VBoxLayout( this, 5, 5 );
    pMainLayout->addWidget( table_ );
    Q3HBoxLayout* pButtonLayout = new Q3HBoxLayout( pMainLayout, 5 );
    pButtonLayout->setAlignment( Qt::AlignCenter );
    pButtonLayout->addWidget( okBtn );
    pButtonLayout->addWidget( cancelBtn );

    // Connections
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( okBtn    , SIGNAL( clicked() ), SLOT( Validate() ) );

    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC destructor
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
DiplomacyDialog_ABC::~DiplomacyDialog_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::Validate
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::Validate()
{
    for( unsigned int i = 0; i < teams_.size(); ++i )
        for( unsigned int j = 0; j < teams_.size(); ++j )
        {
            if( i == j )
                continue;
            Karma karma = Karma::neutral_;
            QTableWidgetItem* item = table_->item( i, j );
            if( item && item->data( KarmaRole ).isValid() )
                karma = *item->data( KarmaRole ).value< const Karma* >();
            SetDiplomacy( *teams_[ i ], *teams_[ j ], karma );
        }

    hide();
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::Reject
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::Reject()
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::NotifyCreated
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::NotifyCreated( const Team_ABC& team )
{
    teams_.push_back( &team );
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::NotifyDeleted
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::NotifyDeleted( const Team_ABC& team )
{
    T_Teams::iterator it = std::find( teams_.begin(), teams_.end(), &team );
    if( it != teams_.end() )
        teams_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::NotifyContextMenu
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::NotifyContextMenu( const Team_ABC& team, ContextMenu& menu )
{
    if( profile_.CanDoMagic( team ) )
        menu.InsertItem( "Command", tools::translate( "gui::DiplomacyDialog_ABC", "Diplomacy" ), this, SLOT( show() ) );
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::showEvent
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::showEvent( QShowEvent* )
{
    UpdateTable();
    for( unsigned int i = 0; i < teams_.size(); ++i )
        for( unsigned int j = 0; j < teams_.size(); ++j )
        {
            if( i == j )
            {
                if( !table_->item( i, j ) )
                    table_->setItem( i, j, new QTableWidgetItem() );
            }
            else
            {
                const Karma& diplomacy = teams_[ i ]->Get< Diplomacies_ABC >().GetDiplomacy( *teams_[ j ] );
                const Karma* karma = &Karma::neutral_;
                if( diplomacy == Karma::friend_ )
                    karma = &Karma::friend_;
                if( diplomacy == Karma::enemy_ )
                    karma = &Karma::enemy_;
                QTableWidgetItem* item = table_->item( i, j );
                if( !item )
                {
                    item = new QTableWidgetItem();
                    table_->setItem( i, j, item );
                    item->setData( ValueSetRole, false );
                    item->setTextAlignment( Qt::AlignCenter );
                }
                item->setText( karma->GetName() );
                item->setData( KarmaRole, QVariant::fromValue( karma ) );
                item->setData( Qt::BackgroundColorRole, GetColor( *karma ) );
            }
        }

    table_->adjustSize();
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::UpdateTable
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::UpdateTable()
{
    table_->setColumnCount( static_cast< int >( teams_.size() ) );
    table_->setRowCount( static_cast< int >( teams_.size() ) );
    int maxName = 0;
    QStringList list;
    for( unsigned i = 0; i < teams_.size(); ++i )
    {
        const QString name = teams_.at( i )->GetName();
        list << name;
        maxName = std::max< int >( name.size(), maxName );
    }
    table_->setVerticalHeaderLabels( list );
    table_->setHorizontalHeaderLabels( list );
    int newWidth = static_cast< int >( teams_.size() * maxName * 10 );
    int newHeight = static_cast< int >( teams_.size() * 40 );
    table_->setMinimumSize( newWidth < maximumWidth() - 20 ? newWidth : maximumWidth() - 20, newHeight < maximumHeight() - 80 ? newHeight : maximumHeight() - 80 );
}
