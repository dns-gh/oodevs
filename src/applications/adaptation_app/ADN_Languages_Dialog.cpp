// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Languages_Dialog.h"
#include "moc_ADN_Languages_Dialog.cpp"
#include "ADN_Languages_Data.h"
#include "ADN_Languages_MasterDialog.h"
#include "ADN_Gui_Tools.h"
#include "clients_kernel/Language.h"
#include "clients_gui/resources.h"

namespace
{
    QToolButton* CreateArrowButton( const QString& objectName, const QString& text, Qt::ArrowType type, const QObject* receiver, const char* slot )
    {
        QToolButton* button = new gui::RichWidget< QToolButton >( objectName );
        button->setToolTip( text );
        button->setArrowType( type );
        QObject::connect( button, SIGNAL( clicked() ), receiver, slot );
        return button;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Dialog constructor
// Created: ABR 2013-09-27
// -----------------------------------------------------------------------------
ADN_Languages_Dialog::ADN_Languages_Dialog( ADN_Languages_Data& data )
    : gui::RichWidget< QDialog >( "language-dialog", 0 )
    , data_( data )
    , masterDialog_( new ADN_Languages_MasterDialog( data ) )
{
    connect( masterDialog_.get(), SIGNAL( MasterChanged() ), this, SIGNAL( MasterChanged() ) );
    gui::ObjectNameManager::getInstance()->AddSubLevel( "language-dialog" );

    setFixedWidth( 500 );
    setMinimumHeight( 400 );
    resize( 500, 400 );
    setCaption( tr( "Languages" ) );

    QGroupBox* masterGroupBox = new QGroupBox( tr( "Master" ) );
    QHBoxLayout* masterLayout = new QHBoxLayout( masterGroupBox );
    master_ = new gui::RichWidget< QLabel >( "master-label" );
    masterLayout->addWidget( master_, 1, Qt::AlignCenter );

    QGroupBox* translationsGroupBox = new QGroupBox( tr( "Translations" ) );
    QGridLayout* translationsLayout = new QGridLayout( translationsGroupBox );

    availables_ = new gui::RichWidget< QTreeWidget >( "available-languages" );
    actives_ = new gui::RichWidget< QTreeWidget >( "active-languages" );
    availables_->setHeaderLabels( QStringList() << tr( "Unused" ) << "Code" );
    availables_->setRootIsDecorated( false );
    availables_->setColumnHidden( 1, true );
    actives_->setHeaderLabels( QStringList() << tr( "Used" ) << "Code" );
    actives_->setRootIsDecorated( false );
    actives_->setColumnHidden( 1, true );
    connect( availables_, SIGNAL( itemSelectionChanged() ), this, SLOT( OnSelectionChanged() ) );
    connect( actives_, SIGNAL( itemSelectionChanged() ), this, SLOT( OnSelectionChanged() ) );

    up_ = CreateArrowButton( "up", tr( "Move up"), Qt::UpArrow, this, SLOT( OnUp() ) );
    down_ = CreateArrowButton( "down", tr( "Move down"), Qt::DownArrow, this, SLOT( OnDown() ) );
    add_ = CreateArrowButton( "add", tr( "Add language" ), Qt::RightArrow, this, SLOT( OnAdd() ) );
    remove_ = CreateArrowButton( "remove", tr( "Remove language" ), Qt::LeftArrow, this, SLOT( OnRemove() ) );
    translationsLayout->addWidget( availables_, 0, 0, 4, 1 );
    translationsLayout->addWidget( add_, 1, 1 );
    translationsLayout->addWidget( remove_, 2, 1 );
    translationsLayout->addWidget( actives_, 0, 2, 4, 1 );
    translationsLayout->addWidget( up_, 1, 3 );
    translationsLayout->addWidget( down_, 2, 3 );

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    QPushButton* cancelButton = ADN_Gui_Tools::CreatePushButton( "cancel", tr( "Cancel" ), qApp->style()->standardIcon( QStyle::SP_DialogCancelButton ), this, SLOT( reject() ) );
    QPushButton* okButton = ADN_Gui_Tools::CreatePushButton( "ok", tr( "Ok" ), qApp->style()->standardIcon( QStyle::SP_DialogOkButton ), this, SLOT( accept() ) );
    buttonsLayout->addWidget( okButton );
    buttonsLayout->addStretch( 1 );
    buttonsLayout->addWidget( cancelButton );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( masterGroupBox );
    mainLayout->addWidget( translationsGroupBox, 1 );
    mainLayout->addLayout( buttonsLayout );

    gui::ObjectNameManager::getInstance()->RemoveSubLevel();
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Dialog destructor
// Created: ABR 2013-09-27
// -----------------------------------------------------------------------------
ADN_Languages_Dialog::~ADN_Languages_Dialog()
{
    // NOTHING
}

namespace
{
    void InsertItem( const boost::shared_ptr< kernel::Language >& language, QTreeWidget* widget )
    {
        QTreeWidgetItem* item = new QTreeWidgetItem( QStringList() << language->GetName().c_str() << language->GetCode().c_str() );
        widget->insertTopLevelItem( widget->topLevelItemCount(), item );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Dialog::OnShow
// Created: ABR 2013-09-27
// -----------------------------------------------------------------------------
int ADN_Languages_Dialog::exec()
{
    if( data_.IsMasterEmpty() && masterDialog_->exec() == QDialog::Rejected )
        return QDialog::Rejected;
    availables_->clear();
    actives_->clear();

    const kernel::Languages::T_Languages& languages = data_.GetAllLanguages().GetLanguages();
    for( auto itAll = languages.begin(); itAll != languages.end(); ++itAll )
        if( data_.IsMaster( ( *itAll )->GetCode() ) )
            master_->setText( ( *itAll )->GetName().c_str() );
        else if( !data_.HasActiveLanguage( ( *itAll )->GetCode() ) )
            InsertItem( *itAll, availables_ );

    const kernel::Languages::T_Languages& activeLanguages = data_.GetActiveLanguages();
    for( auto itActive = activeLanguages.begin(); itActive != activeLanguages.end(); ++itActive )
        InsertItem( *itActive, actives_ );

    availables_->sortItems( 1, Qt::AscendingOrder );
    OnSelectionChanged();
    return QDialog::exec();
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Dialog::accept
// Created: ABR 2013-09-27
// -----------------------------------------------------------------------------
void ADN_Languages_Dialog::accept()
{
    data_.PurgeActiveLanguages();
    for( int i = 0; i < actives_->topLevelItemCount(); ++i )
        data_.AddActiveLanguage( actives_->topLevelItem( i )->text( 1 ).toStdString() );
    emit LanguagesEdited();
    ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
    QDialog::accept();
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Dialog::OnSelectionChanged
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
void ADN_Languages_Dialog::OnSelectionChanged()
{
    QTreeWidgetItem* availableItem = availables_->currentItem();
    QTreeWidgetItem* activeItem = actives_->currentItem();
    int activeIndex = activeItem ? actives_->indexOfTopLevelItem( activeItem ) : -1;
    add_->setEnabled( availableItem != 0 );
    remove_->setEnabled( activeItem != 0 );
    up_->setEnabled( activeItem && activeIndex != 0 );
    down_->setEnabled( activeItem && activeIndex != actives_->topLevelItemCount() - 1 );
}

namespace
{
    void MoveCurrentItem( QTreeWidget* widget, int indexModifier )
    {
        QTreeWidgetItem* item = widget->currentItem();
        const int index = widget->indexOfTopLevelItem( item );
        if( index + indexModifier < 0 || index + indexModifier >= widget->topLevelItemCount() )
            return;
        widget->takeTopLevelItem( index );
        widget->insertTopLevelItem( index + indexModifier, item );
        widget->setCurrentItem( item );
    }
    void SwapItem( QTreeWidget* source, QTreeWidget* dest )
    {
        QTreeWidgetItem* item = source->currentItem();
        source->takeTopLevelItem( source->indexOfTopLevelItem( item ) );
        dest->insertTopLevelItem( dest->topLevelItemCount(), item );
        dest->setCurrentItem( item );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Dialog::OnUp
// Created: ABR 2013-09-27
// -----------------------------------------------------------------------------
void ADN_Languages_Dialog::OnUp()
{
    MoveCurrentItem( actives_, -1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Dialog::OnDown
// Created: ABR 2013-09-27
// -----------------------------------------------------------------------------
void ADN_Languages_Dialog::OnDown()
{
    MoveCurrentItem( actives_, 1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Dialog::OnAdd
// Created: ABR 2013-09-27
// -----------------------------------------------------------------------------
void ADN_Languages_Dialog::OnAdd()
{
    SwapItem( availables_, actives_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Dialog::OnRemove
// Created: ABR 2013-09-27
// -----------------------------------------------------------------------------
void ADN_Languages_Dialog::OnRemove()
{
    SwapItem( actives_, availables_ );
    availables_->sortItems( 1, Qt::AscendingOrder );
}
