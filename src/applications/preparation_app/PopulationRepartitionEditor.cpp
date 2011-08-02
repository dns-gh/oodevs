// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PopulationRepartitionEditor.h"
#include "moc_PopulationRepartitionEditor.cpp"
#include "clients_kernel/Controller.h"
#include "preparation/PopulationRepartition.h"

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor constructor
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
PopulationRepartitionEditor::PopulationRepartitionEditor( QDialog*& self, QWidget* parent, kernel::Controller& controller )
    : QDialog( parent, 0, true )
    , self_      ( self )
    , controller_( controller )
{
    setCaption( tr( "Repartition Editor" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );
    Q3GroupBox* vBox = new Q3GroupBox( 2, Qt::Horizontal, this );
    pMainLayout->addWidget( vBox );
    {
        new QLabel( tr( "Male (%)" ), vBox );
        male_ = new QLineEdit( vBox );
        male_->setValidator( new QIntValidator( 0, 100, male_ ) );
        male_->setFixedWidth( 40 );
        connect( male_, SIGNAL( textChanged( const QString& ) ), SLOT( OnValueChanged() ) );
    }
    {
        new QLabel( tr( "Female (%)" ), vBox );
        female_ = new QLineEdit( vBox );
        female_->setValidator( new QIntValidator( 0, 100, female_ ) );
        female_->setFixedWidth( 40 );
        connect( female_, SIGNAL( textChanged( const QString& ) ), SLOT( OnValueChanged() ) );
    }
    {
        new QLabel( tr( "Children (%)" ), vBox );
        children_ = new QLineEdit( vBox );
        children_->setValidator( new QIntValidator( 0, 100, children_ ) );
        children_->setFixedWidth( 40 );
        connect( children_, SIGNAL( textChanged( const QString& ) ), SLOT( OnValueChanged() ) );
    }
    warning_ = new QLabel( this );
    pMainLayout->addWidget( warning_ );

    Q3HBox* pbuttonBox = new Q3HBox( this );
    okBtn_ = new QPushButton( tr( "Ok" ), pbuttonBox );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), pbuttonBox );
    okBtn_->setDefault( true );
    okBtn_->setMaximumWidth( 60 );
    cancelBtn->setMaximumWidth( 60 );

    pMainLayout->addWidget( pbuttonBox, 0, Qt::AlignRight );

    connect( okBtn_   , SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );

    show();
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor destructor
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
PopulationRepartitionEditor::~PopulationRepartitionEditor()
{
    self_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::SetValue
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void PopulationRepartitionEditor::SetValue( PopulationRepartition*& value )
{
    value_ = &value;
    male_->setText( QString::number( value->male_ ) );
    female_->setText( QString::number( value->female_ ) );
    children_->setText( QString::number( value->children_ ) );
    OnValueChanged();
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::GetValue
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
PopulationRepartition* PopulationRepartitionEditor::GetValue()
{
    return *value_;
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::OnValueChanged
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void PopulationRepartitionEditor::OnValueChanged()
{
    bool valid = ( male_->text().toInt() + female_->text().toInt() + children_->text().toInt() == 100 );
    okBtn_->setEnabled( valid );
    warning_->setText( valid ? QString() : tr( "<font color=\"#ff0000\">Total must be equal to 100</font>" ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::OnAccept
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void PopulationRepartitionEditor::OnAccept()
{
    ( *value_ )->male_ = male_->text().toUInt();
    ( *value_ )->female_ = female_->text().toUInt();
    ( *value_ )->children_ = children_->text().toUInt();
    controller_.Update( **value_ );
    accept();
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::OnReject
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void PopulationRepartitionEditor::OnReject()
{
    reject();
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::sizeHint
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
QSize PopulationRepartitionEditor::sizeHint() const
{
    return QSize( 250, 150 );
}
