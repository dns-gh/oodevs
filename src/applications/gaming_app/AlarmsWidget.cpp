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
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: AlarmsWidget constructor
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
AlarmsWidget::AlarmsWidget( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation )
    : QDialog     ( parent, "AlarmsWidget" )
    , controllers_( controllers )
    , simulation_ ( simulation )
    , remove_     ( 0 )
{
    setCaption( tr( "Alarms" ) );
    Q3GridLayout* pLayout = new Q3GridLayout( this, 2, 1 );
    pLayout->setMargin( 10 );
    pLayout->setSpacing( 10 );

    list_ = new Q3ListView( this );
    list_->addColumn( tr( "Trigger date" ) );
    list_->addColumn( tr( "Message" ) );
    list_->setSelectionMode( Q3ListView::Single );
    pLayout->addWidget( list_, 0, 0 );

    connect( list_, SIGNAL( selectionChanged( Q3ListViewItem* ) ),                  SLOT( OnSelectionChange( Q3ListViewItem* ) ) );
    connect( list_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( ShowEditor( Q3ListViewItem* ) ) );

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
    const QDateTime date = simulation.GetDateTime();
    Q3ListViewItem* item = list_->firstChild();
    while( item )
    {
        if( IsAfter( item, date ) )
            item = Trigger( item );
        else
            item = item->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::IsAfter
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
bool AlarmsWidget::IsAfter( Q3ListViewItem* item, const QDateTime& date )
{
    if( !item
     || item->text( 0 ).isEmpty() )
        return false;

    QDateTime myTime = QDateTime::fromString( item->text( 0 ), Qt::ISODate );
    return date >= myTime;
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::Trigger
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
Q3ListViewItem* AlarmsWidget::Trigger( Q3ListViewItem* item )
{
    messageBox_->setText( item->text( 1 ) );
    messageBox_->show();
    Q3ListViewItem* next = item->nextSibling();
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
    Q3ListViewItem* item = new Q3ListViewItem( list_ );
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
void AlarmsWidget::OnSelectionChange( Q3ListViewItem* item )
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
            setCaption( tools::translate( "AlarmsWidget", "Alarm parameters" ) );
            Q3GridLayout* pLayout = new Q3GridLayout( this, 2, 3 );
            pLayout->setMargin( 10 );
            pLayout->setSpacing( 10 );

            time_ = new Q3DateTimeEdit( this );
            pLayout->addMultiCellWidget( time_, 0, 0, 0, 1 );
            text_ = new QLineEdit( this );
            pLayout->addWidget( text_, 0, 2 );

            Q3HBox* buttonBox = new Q3HBox( this );
            QPushButton* ok = new QPushButton( tr( "Ok" ), buttonBox );
            connect( ok, SIGNAL( clicked() ), SLOT( accept() ) );
            QPushButton* cancel = new QPushButton( tr( "Cancel" ), buttonBox );
            connect( cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

            pLayout->addMultiCellWidget( buttonBox, 1, 1, 0, 2 );

            Show( 0 );
        }
        void Show( Q3ListViewItem* item )
        {
            item_ = item;
            if( item_ && ! item_->text( 0 ).isEmpty() )
                time_->setDateTime( QDateTime::fromString( item_->text( 0 ), Qt::ISODate ) );
            else
                time_->setDateTime( simulation_.GetDateTime() );
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
        AlarmEditor& operator=( const AlarmEditor& );
        const Simulation& simulation_;
        Q3ListViewItem* item_;
        Q3DateTimeEdit* time_;
        QLineEdit*     text_;
    };
}

// -----------------------------------------------------------------------------
// Name: AlarmsWidget::ShowEditor
// Created: AGE 2007-05-07
// -----------------------------------------------------------------------------
void AlarmsWidget::ShowEditor( Q3ListViewItem* item )
{
    static AlarmEditor* editor = new AlarmEditor( this, simulation_ );
    editor->Show( item );
}
