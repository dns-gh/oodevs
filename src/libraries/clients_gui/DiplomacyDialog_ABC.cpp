// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DiplomacyDialog_ABC.h"
#include "moc_DiplomacyDialog_ABC.cpp"
#include "RichWidget.h"
#include "RichPushButton.h"
#include "RichWidget.h"
#include "SubObjectName.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace gui;

Q_DECLARE_METATYPE( const Karma* )

namespace
{
    enum E_Roles
    {
        KarmaRole = Qt::UserRole,
        ValueSetRole = Qt::UserRole + 1
    };
}

namespace
{
    const int rowHeight = 40;
    const int textMargin = 27;
    const int maxWidth = 1024;
    const int maxHeight = 768;

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

        virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index ) const
        {
            if( index.row() == index.column() )
                return 0;
            RichWidget< QComboBox >* editor = new RichWidget< QComboBox >( "editorCombo", parent );
            editor->addItem( Karma::friend_ .GetName(), QVariant::fromValue( static_cast< const Karma* >( &Karma::friend_ ) ) );
            editor->addItem( Karma::enemy_  .GetName(), QVariant::fromValue( static_cast< const Karma* >( &Karma::enemy_ ) ) );
            editor->addItem( Karma::neutral_.GetName(), QVariant::fromValue( static_cast< const Karma* >( &Karma::neutral_ ) ) );
            return editor;
        }

        virtual void setEditorData( QWidget *editor, const QModelIndex &index ) const
        {
            RichWidget< QComboBox >* comboBox = static_cast< RichWidget< QComboBox >* >( "comboBox", editor );
            const Karma* karma = index.model()->data( index, KarmaRole ).value< const Karma* >();
            comboBox->setCurrentIndex( comboBox->findData( QVariant::fromValue( karma ) ) );
        }

        virtual void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
        {
            RichWidget< QComboBox >* comboBox = static_cast< RichWidget< QComboBox >* >( "comboBox", editor );
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
    , minCellWidth_( 0 )
{
    SubObjectName subObject( "DiplomacyDialogABC" );
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    setCaption( tools::translate( "gui::DiplomacyDialog_ABC", "Diplomacy" ) );
    setMaximumSize( maxWidth, maxHeight );

    // Table
    table_ = new RichWidget< QTableWidget >( "table", this );
    table_->setSelectionMode( QAbstractItemView::NoSelection );
    table_->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    table_->verticalHeader()->setResizeMode( QHeaderView::Stretch );
    table_->setItemDelegate( new DiplomacyItemDelegate() );
    table_->setEditTriggers( QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked | QAbstractItemView::CurrentChanged );

    QFontMetrics fm( table_->fontMetrics() );
    minCellWidth_ = std::max( fm.width( Karma::friend_.GetName() ), minCellWidth_ );
    minCellWidth_ = std::max( fm.width( Karma::enemy_.GetName() ), minCellWidth_ );
    minCellWidth_ = std::max( fm.width( Karma::neutral_.GetName() ), minCellWidth_ );
    minCellWidth_ = std::max( fm.width( Karma::unknown_.GetName() ), minCellWidth_ );

    // Buttons
    RichPushButton* okBtn     = new RichPushButton( "ok", tools::translate( "gui::DiplomacyDialog_ABC", "Ok" ), this );
    RichPushButton* cancelBtn = new RichPushButton( "cancel", tools::translate( "gui::DiplomacyDialog_ABC", "Cancel" ), this );
    okBtn->setDefault( true );
    okBtn->setMaximumWidth( 100 );
    cancelBtn->setMaximumWidth( 100 );

    // Layouts
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->addWidget( table_ );
    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
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
    const int nTeams = static_cast< int >( teams_.size() );
    for( int i = 0; i < nTeams; ++i )
        for( int j = 0; j < nTeams; ++j )
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
    const int nTeams = static_cast< int >( teams_.size() );
    for( int i = 0; i < nTeams; ++i )
        for( int j = 0; j < nTeams; ++j )
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
    adjustSize();
}

// -----------------------------------------------------------------------------
// Name: DiplomacyDialog_ABC::UpdateTable
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void DiplomacyDialog_ABC::UpdateTable()
{
    const int nTeams = static_cast< int >( teams_.size() );
    table_->setColumnCount( nTeams );
    table_->setRowCount( nTeams );
    int maxNameWidth = 0;
    QStringList list;
    for( std::size_t i = 0; i < teams_.size(); ++i )
    {
        const QString name = teams_.at( i )->GetName();
        list << name;
        maxNameWidth = std::max( table_->fontMetrics().width( name ), maxNameWidth );
    }
    table_->setVerticalHeaderLabels( list );
    table_->setHorizontalHeaderLabels( list );
    int cellWidth = std::max( maxNameWidth, minCellWidth_ ) + textMargin;
    int newWidth = std::min( ( nTeams + 1 ) * cellWidth, maxWidth );
    int newHeight = std::min( ( nTeams + 1 ) * rowHeight, maxHeight );
    table_->setMinimumSize( newWidth, newHeight );
}
