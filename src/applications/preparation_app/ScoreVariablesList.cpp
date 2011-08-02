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
#include "clients_kernel/SimpleLocationDrawer.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Circle.h"
#include "clients_gui/UtmParser.h"
#include "indicators/DataTypeFactory.h"
#include "indicators/Element_ABC.h"
#include "indicators/Variable.h"
#include "indicators/Variables.h"
#include "preparation/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreVariablesList::ScoreVariablesList( QWidget* parent, gui::ItemFactory_ABC& factory, kernel::Controllers& controllers, gui::ParametersLayer& layer,
                                        const StaticModel& staticModel, const kernel::GlTools_ABC& tools )
    : Q3VBox   ( parent )
    , factory_( factory )
    , tools_  ( tools )
    , wizard_ ( new ScoreVariableCreationWizard( this, controllers, layer, staticModel, tools_ ) )
    , list_   ( new gui::ListDisplayer< ScoreVariablesList >( this, *this, factory ) )
    , parser_ ( new gui::UtmParser( controllers, staticModel.coordinateConverter_ ) )
{
    setMargin( 5 );
    {
        list_->AddColumn( tr( "Name" ) );
        list_->AddColumn( tr( "Type" ) );
        list_->AddColumn( tr( "Value" ) );
        connect( list_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnPaste() ) );
    }
    {
        Q3HBox* box = new Q3HBox( this );
        QPushButton* add = new QPushButton( tr( "Add" ), box );
        QPushButton* del = new QPushButton( tr( "Del" ), box );
        QPushButton* paste = new QPushButton( tr( "Paste" ), box );
        connect( add, SIGNAL( clicked() ), SLOT( OnAdd() ) );
        connect( del, SIGNAL( clicked() ), SLOT( OnDelete() ) );
        connect( paste, SIGNAL( clicked() ), SLOT( OnPaste() ) );
    }
    connect( wizard_, SIGNAL( VariableCreated( const indicators::Element_ABC& ) ), SLOT( AddVariable( const indicators::Element_ABC& ) ) );
    connect( wizard_, SIGNAL( Closed() ), SIGNAL( EndEdit() ) );
    connect( list_, SIGNAL( selectionChanged() ), SLOT( OnItemClick() ) );
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
    location_.reset();
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
    for( Q3ListViewItemIterator it( list_ ); it.current(); ++it )
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
    if( Q3ListViewItem* item = list_->selectedItem() )
    {
        list_->removeItem( item );
        emit Updated();
        location_.reset();
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::OnPaste
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreVariablesList::OnPaste()
{
    if( Q3ListViewItem* item = list_->selectedItem() )
        emit Insert( item->text( 0 ) );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::Draw
// Created: FPO 2011-07-19
// -----------------------------------------------------------------------------
void ScoreVariablesList::Draw( kernel::Viewport_ABC& viewport )
{
    wizard_->Draw( viewport );
    if( location_.get() && isVisible() )
    {
        kernel::SimpleLocationDrawer drawer( tools_ );
        location_->Accept( drawer );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::OnItemClick
// Created: FPO 2011-07-20
// -----------------------------------------------------------------------------
void ScoreVariablesList::OnItemClick()
{
    location_.reset();
    if( Q3ListViewItem* item = list_->selectedItem() )
    {
        if( item->text( 1 ) == "zone" )
        {
            QString temp( item->text( 2 ) );
            if( item->text( 2 ).left( 7 ) == "polygon" )
                location_.reset( new kernel::Polygon() );
            else if( item->text( 2 ).left( 6 ) == "circle" )
                location_.reset( new kernel::Circle() );

            if( location_.get() )
            {
                temp = temp.mid( temp.find( "(" ) + 1, temp.length() - ( temp.find( "(" ) + 2 ) );
                QStringList tempList = QStringList::split( ",", temp );
                QStringList hint;
                geometry::Point2f point;
                for( QStringList::iterator it = tempList.begin(); it != tempList.end(); it++ )
                {
                    if( parser_->Parse( *it, point, hint ) )
                        location_->AddPoint( point );
                }
            }
        }
    }
}
