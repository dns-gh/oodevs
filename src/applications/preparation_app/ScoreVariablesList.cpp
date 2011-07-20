// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreVariablesList.h"
#include "moc_ScoreVariablesList.cpp"
#include "ScoreVariableCreationWizard.h"
#include "indicators/DataTypeFactory.h"
#include "indicators/Element_ABC.h"
#include "indicators/Variable.h"
#include "indicators/Variables.h"

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreVariablesList::ScoreVariablesList( QWidget* parent, gui::ItemFactory_ABC& factory, kernel::Controllers& controllers, gui::ParametersLayer& layer,
                                        const StaticModel& staticModel, const kernel::GlTools_ABC& tools )
    : QVBox( parent )
    , factory_( factory )
    , wizard_( new ScoreVariableCreationWizard( this, controllers, layer, staticModel, tools ) )
    , list_( new gui::ListDisplayer< ScoreVariablesList >( this, *this, factory ) )
{
    setMargin( 5 );
    {
        list_->AddColumn( tr( "Name" ) );
        list_->AddColumn( tr( "Type" ) );
        list_->AddColumn( tr( "Value" ) );
        connect( list_, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), SLOT( OnPaste() ) );
    }
    {
        QHBox* box = new QHBox( this );
        QButton* add = new QPushButton( tr( "Add" ), box );
        QButton* del = new QPushButton( tr( "Del" ), box );
        QButton* paste = new QPushButton( tr( "Paste" ), box );
        connect( add, SIGNAL( clicked() ), SLOT( OnAdd() ) );
        connect( del, SIGNAL( clicked() ), SLOT( OnDelete() ) );
        connect( paste, SIGNAL( clicked() ), SLOT( OnPaste() ) );
    }
    connect( wizard_, SIGNAL( VariableCreated( const indicators::Element_ABC& ) ), SLOT( AddVariable( const indicators::Element_ABC& ) ) );
    connect( wizard_, SIGNAL( Closed() ), SIGNAL( EndEdit() ) );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList destructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreVariablesList::~ScoreVariablesList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::StartEdit
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void ScoreVariablesList::StartEdit( const indicators::Variables& variables )
{
    list_->clear();
    tools::Iterator< const indicators::Element_ABC& > it = variables.CreateIterator();
    while( it.HasMoreElements() )
        AddVariable( it.NextElement() );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::AddVariable
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void ScoreVariablesList::AddVariable( const indicators::Element_ABC& variable )
{
    gui::ValuedListItem* item = factory_.CreateItem( list_ );
    Display( variable, item );
    emit Updated();
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::GetValue
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
indicators::Variables ScoreVariablesList::GetValue() const
{
    indicators::Variables result;
    indicators::DataTypeFactory types;
    for( QListViewItemIterator it( list_ ); it.current(); ++it )
        if( const gui::ValuedListItem* item = static_cast< const gui::ValuedListItem* >( *it ) )
        {
            const QString name  = item->text( 0 ).remove( 0, 1 );
            const QString type  = item->text( 1 );
            const QString value = item->text( 2 );
            boost::shared_ptr< indicators::Element_ABC > variable( new indicators::Variable( name.ascii(), types.Instanciate( type.ascii() ), value.ascii() ) );
            result.Register( name.ascii(), variable );
        }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::Display
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void ScoreVariablesList::Display( const indicators::Element_ABC& variable, gui::ValuedListItem* item )
{
    // $$$$ SBO 2009-04-24: TODO use a displayer
    item->setText( 0, variable.GetInput().c_str() );
    item->setText( 1, variable.GetType().ToString().c_str() );
    item->setText( 2, variable.GetValue().c_str() );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::OnAdd
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreVariablesList::OnAdd()
{
    emit StartEdit();
    wizard_->Create();
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::OnDelete
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreVariablesList::OnDelete()
{
    if( QListViewItem* item = list_->selectedItem() )
    {
        list_->removeItem( item );
        emit Updated();
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::OnPaste
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreVariablesList::OnPaste()
{
    if( QListViewItem* item = list_->selectedItem() )
        emit Insert( item->text( 0 ) );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::Draw
// Created: FPO 2011-07-19
// -----------------------------------------------------------------------------
void ScoreVariablesList::Draw( kernel::Viewport_ABC& viewport )
{
    wizard_->Draw( viewport );
}
