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
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: AlarmsWidget constructor
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
AlarmsWidget::AlarmsWidget( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation )
    : QDialog     ( parent )
    , controllers_( controllers )
    , simulation_ ( simulation )
    , remove_     ( 0 )
{
    setCaption( tr( "Alarms" ) );
    QGridLayout* pLayout = new QGridLayout( this, 2, 1 );
    pLayout->setMargin( 10 );
    pLayout->setSpacing( 10 );

    list_ = new QListView( this );
    list_->addColumn( tr( "Trigger day" ) );
    list_->addColumn( tr( "Trigger time" ) );
    list_->addColumn( tr( "Message" ) );
    list_->setSelectionMode( QListView::Single );
    pLayout->addWidget( list_, 0, 0 );

    connect( list_, SIGNAL( selectionChanged( QListViewItem* ) ),                  SLOT( OnSelectionChange( QListViewItem* ) ) );
    connect( list_, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), SLOT( ShowEditor( QListViewItem* ) ) );

    QHBox* buttons = new QHBox( this );
    {
        QPushButton* create = new QPushButton( tr( "Create new alarm" ), buttons );
        connect( create, SIGNAL( clicked() ), SLOT( OnCreate() ) );

        remove_ = new QPushButton( tr( "Remove alarm" ), buttons );
        remove_->setDisabled( true );
        connect( remove_, SIGNAL( clicked() ), SLOT( OnDelete() ) );
    }
    pLayout->addWidget( buttons, 1, 0 );

    messageBox_ = new QMessageBox( tr( "Alarm triggered" ), "", QMessageBox::Information, QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton, QMessageBox::NoButton );
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
    const int day    = simulation.GetDay();
    const QTime time = QTime::fromString( simulation.GetTimeAsString() );
    QListViewItem* item = list_->firstChild();
    while( item )
    {
        if( IsAfter( item, day, time ) )
            item = Trigger( item );
        else
            item = item->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::IsAfter
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
bool AlarmsWidget::IsAfter( QListViewItem* item, int simuDay, const QTime& simulationTime )
{
    if( !item
     || item->text( 0 ).isEmpty() 
     || item->text( 1 ).isEmpty() )
        return false;

    int day = item->text( 0 ).mid( 4 ).toInt();
    if( simuDay > day )
        return true;
    if( simuDay < day )
        return false;
    QTime time = QTime::fromString( item->text( 1 ) );
    return time <= simulationTime;
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::Trigger
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
QListViewItem* AlarmsWidget::Trigger( QListViewItem* item )
{
    messageBox_->setText( item->text( 1 ) );
    messageBox_->show();
    QListViewItem* next = item->nextSibling();
    remove_->setDisabled( true );
    delete item;
    return next;
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::OnCreate
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
void AlarmsWidget::OnCreate()
{
    QListViewItem* item = new QListViewItem( list_ );
    ShowEditor( item );
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::OnDelete
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
void AlarmsWidget::OnDelete()
{
    delete list_->selectedItem();
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::OnSelectionChange
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
void AlarmsWidget::OnSelectionChange( QListViewItem* item )
{
    remove_->setDisabled( item == 0 );
}

namespace
{
    class AlarmEditor : public QDialog
    {
    public:
        AlarmEditor( QWidget* parent, const Simulation& simulation )
            : QDialog    ( parent )
            , simulation_( simulation )
            , item_      ( 0 )
        {
            setCaption( tr("Alarm parameters") );
            QGridLayout* pLayout = new QGridLayout( this, 2, 3 );
            pLayout->setMargin( 10 );
            pLayout->setSpacing( 10 );

            day_ = new QSpinBox( 0, 10000, 1, this );
            pLayout->addWidget( day_, 0, 0 );
            time_ = new QTimeEdit( this );
            pLayout->addWidget( time_, 0, 1 );
            text_ = new QLineEdit( this );
            pLayout->addWidget( text_, 0, 2 );

            QHBox* buttonBox = new QHBox( this );
            QPushButton* ok = new QPushButton( tr( "Ok" ), buttonBox );
            connect( ok, SIGNAL( clicked() ), SLOT( accept() ) );
            QPushButton* cancel = new QPushButton( tr( "Cancel" ), buttonBox );
            connect( cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

            pLayout->addMultiCellWidget( buttonBox, 1, 1, 0, 2 );

            Show( 0 );
        }
        void Show( QListViewItem* item )
        {
            item_ = item;
            QString day  = item_ && ! item_->text( 0 ).isEmpty() ? item_->text( 0 ) : QString( "Day %1" ).arg( simulation_.GetDay() );
            QString time = item_ && ! item_->text( 1 ).isEmpty() ? item_->text( 1 ) : simulation_.GetTimeAsString();
            QString text = item_ && ! item_->text( 2 ).isEmpty() ? item_->text( 2 ) : "";
            day_ ->setValue( day.mid( 4 ).toInt() );
            time_->setTime( QTime::fromString( time ) );
            text_->setText( text );
            if( item_ )
                exec();
            else 
                hide();
        }

        virtual void accept()
        {
            item_->setText( 0, QString( "Day %1" ).arg( day_->value() ) );
            item_->setText( 1, time_->time().toString() );
            item_->setText( 2, text_->text() );
            QDialog::accept();
        }
    private:
        AlarmEditor& operator=( const AlarmEditor& );
        const Simulation& simulation_;
        QListViewItem* item_;
        QSpinBox*      day_;
        QTimeEdit*     time_;
        QLineEdit*     text_;
    };
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::ShowEditor
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
void AlarmsWidget::ShowEditor( QListViewItem* item )
{
    static AlarmEditor* editor = new AlarmEditor( this, simulation_ );
    editor->Show( item );
}
