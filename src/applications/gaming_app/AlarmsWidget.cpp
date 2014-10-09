// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AlarmsWidget.h"
#include "moc_AlarmsWidget.cpp"
#include "gaming/Simulation.h"
#include "gaming/SimulationController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: AlarmsWidget constructor
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
AlarmsWidget::AlarmsWidget( QWidget* parent, kernel::Controllers& controllers, const SimulationController& simulationController )
    : QDialog( parent, "AlarmsWidget" )
    , controllers_( controllers )
    , simulationController_( simulationController )
{
    setCaption( tr( "Alarms" ) );
    Q3GridLayout* pLayout = new Q3GridLayout( this, 2, 1 );
    pLayout->setMargin( 10 );
    pLayout->setSpacing( 10 );

    list_ = new QTreeWidget( this );
    list_->setColumnCount( 2 );
    list_->setRootIsDecorated( false );
    list_->header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    list_->setAllColumnsShowFocus( true );
    QStringList headers;
    headers << tr( "Trigger date" ) << tr( "Message" );
    list_->setHeaderLabels( headers );
    pLayout->addWidget( list_, 0, 0 );

    connect( list_, SIGNAL( itemSelectionChanged() ), SLOT( OnSelectionChange() ) );
    connect( list_, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), SLOT( ShowEditorSlot( QTreeWidgetItem* ) ) );

    Q3HBox* buttons = new Q3HBox( this );
    {
        QPushButton* create = new QPushButton( tr( "Create new alarm" ), buttons );
        connect( create, SIGNAL( clicked() ), SLOT( OnCreate() ) );

        remove_ = new QPushButton( tr( "Remove alarm" ), buttons );
        remove_->setDisabled( true );
        connect( remove_, SIGNAL( clicked() ), SLOT( OnDelete() ) );
    }
    pLayout->addWidget( buttons, 1, 0 );

    messageBox_ = new QMessageBox( tr( "Alarm triggered" ), "", QMessageBox::Information, QMessageBox::Ok | QMessageBox::Default, Qt::NoButton, Qt::NoButton );
    messageBox_->hide();

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget destructor
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
AlarmsWidget::~AlarmsWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::NotifyUpdated
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
void AlarmsWidget::NotifyUpdated( const Simulation& simulation )
{
    if( list_->topLevelItemCount() == 0 )
        return;
    const QDateTime date = simulation.GetDateTime();
    QTreeWidgetItem* item = list_->topLevelItem( 0 );
    while( item )
    {
        QTreeWidgetItem* nextItem = list_->itemBelow( item );
        if( IsAfter( item, date ) )
            Trigger( item );
        item = nextItem;
    }
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::IsAfter
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
bool AlarmsWidget::IsAfter( QTreeWidgetItem* item, const QDateTime& date )
{
    if( !item || item->text( 0 ).isEmpty() )
        return false;

    QDateTime myTime = QDateTime::fromString( item->text( 0 ), Qt::ISODate );
    return date >= myTime;
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::Trigger
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
void AlarmsWidget::Trigger( QTreeWidgetItem* item )
{
    messageBox_->setText( item->text( 1 ) );
    messageBox_->show();
    remove_->setDisabled( true );
    delete item;
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::OnCreate
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
void AlarmsWidget::OnCreate()
{
    QTreeWidgetItem* item = new QTreeWidgetItem( list_ );
    ShowEditor( item, true );
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::OnDelete
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
void AlarmsWidget::OnDelete()
{
    delete list_->currentItem();
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::OnSelectionChange
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
void AlarmsWidget::OnSelectionChange()
{
    remove_->setDisabled( list_->currentItem() == 0 );
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::ShowEditorSlot
// Created: JSR 2012-10-29
// -----------------------------------------------------------------------------
void AlarmsWidget::ShowEditorSlot( QTreeWidgetItem* item )
{
    ShowEditor( item, false );
}

namespace
{
    class AlarmEditor : public QDialog
    {
    public:
        AlarmEditor( QWidget* parent, const SimulationController& simulationController )
            : QDialog( parent )
            , simulationController_( simulationController )
            , item_( 0 )
        {
            setCaption( tools::translate( "AlarmsWidget", "Alarm parameters" ) );
            Q3GridLayout* pLayout = new Q3GridLayout( this, 2, 3 );
            pLayout->setMargin( 10 );
            pLayout->setSpacing( 10 );

            time_ = new QDateTimeEdit( this );
            pLayout->addMultiCellWidget( time_, 0, 0, 0, 1 );
            text_ = new QLineEdit( this );
            pLayout->addWidget( text_, 0, 2 );

            Q3HBox* buttonBox = new Q3HBox( this );
            QPushButton* ok = new QPushButton( tools::translate( "AlarmEditor", "Ok" ), buttonBox );
            connect( ok, SIGNAL( clicked() ), SLOT( accept() ) );
            QPushButton* cancel = new QPushButton( tools::translate( "AlarmEditor", "Cancel" ), buttonBox );
            connect( cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

            pLayout->addMultiCellWidget( buttonBox, 1, 1, 0, 2 );

            Show( 0 );
        }
        void Show( QTreeWidgetItem* item )
        {
            item_ = item;
            if( item_ && ! item_->text( 0 ).isEmpty() )
                time_->setDateTime( QDateTime::fromString( item_->text( 0 ), Qt::ISODate ) );
            else
                time_->setDateTime( simulationController_.GetDateTime() );
            if( item_ )
            {
                text_->setText( item_->text( 1 ) );
                exec();
            }
            else
                hide();
        }

        virtual void accept()
        {
            item_->setText( 0, time_->dateTime().toString( Qt::ISODate ) );
            item_->setText( 1, text_->text() );
            QDialog::accept();
        }
    private:
        const SimulationController& simulationController_;
        QTreeWidgetItem* item_;
        QDateTimeEdit* time_;
        QLineEdit* text_;
    };
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::ShowEditor
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
void AlarmsWidget::ShowEditor( QTreeWidgetItem* item, bool newAlarm )
{
    static AlarmEditor* editor = new AlarmEditor( this, simulationController_ );
    editor->Show( item );
    if( newAlarm && editor->result() == QDialog::Rejected )
        delete item;
}
