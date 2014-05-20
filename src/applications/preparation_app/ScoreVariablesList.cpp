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

#include "clients_gui/RichWidget.h"
#include "clients_gui/SimpleLocationDrawer.h"
#include "clients_gui/MgrsParser.h"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Circle.h"
#include "clients_kernel/Tools.h"
#include "indicators/DataTypeFactory.h"
#include "indicators/Element_ABC.h"
#include "indicators/Variable.h"
#include "indicators/Variables.h"
#include "preparation/StaticModel.h"

#include <boost/smart_ptr/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreVariablesList::ScoreVariablesList( kernel::Controllers& controllers, const StaticModel& staticModel,
                                        gui::GlTools_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder )
    : tools_( tools )
    , wizard_( new ScoreVariableCreationWizard( this, controllers, tools, builder ) )
    , list_( new gui::RichWidget< QTreeWidget >( "scoreList", this ) )
    , parser_( new gui::MgrsParser( controllers,
                                   [&]( const std::string& mgrs ) { return staticModel.coordinateConverter_.ConvertToXY( mgrs ); },
                                   [&]( const geometry::Point2f& position ) { return staticModel.coordinateConverter_.ConvertToMgrs( position ); } ) )
{
    gui::SubObjectName subObject( "ScoreVariablesList" );
    setMargin( 5 );
    {
        list_->setColumnCount( 3 );
        QStringList headers;
        headers << tools::translate( "ScoreVariablesList", "Name" ) << tools::translate( "ScoreVariablesList", "Type" ) << tools::translate( "ScoreVariablesList", "Value" );
        list_->setHeaderLabels( headers );
        list_->setRootIsDecorated( false );
        connect( list_, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), SLOT( OnPaste() ) );
    }
    {
        Q3HBox* box = new Q3HBox( this );
        gui::RichPushButton* add = new gui::RichPushButton( "add", tools::translate( "ScoreVariablesList", "Add" ), box );
        gui::RichPushButton* del = new gui::RichPushButton( "del", tools::translate( "ScoreVariablesList", "Del" ), box );
        gui::RichPushButton* paste = new gui::RichPushButton( "paste",  tools::translate( "ScoreVariablesList", "Paste" ), box );
        connect( add, SIGNAL( clicked() ), SLOT( OnAdd() ) );
        connect( del, SIGNAL( clicked() ), SLOT( OnDelete() ) );
        connect( paste, SIGNAL( clicked() ), SLOT( OnPaste() ) );
    }
    connect( wizard_, SIGNAL( VariableCreated( const indicators::Element_ABC& ) ), SLOT( AddVariable( const indicators::Element_ABC& ) ) );
    connect( wizard_, SIGNAL( Closed() ), SIGNAL( EndEdit() ) );
    connect( list_, SIGNAL( itemSelectionChanged() ), SLOT( OnItemClick() ) );

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
    list_->model()->removeRows( 0, list_->model()->rowCount() );
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
    QStringList list;
    list << variable.GetInput().c_str() << variable.GetType().ToString().c_str() << variable.GetValue().c_str();
    QTreeWidgetItem* item = new QTreeWidgetItem( list );
    list_->addTopLevelItem( item );
    emit Updated();
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::OnCloseDialogs
// Created: LDC 2013-09-05
// -----------------------------------------------------------------------------
void ScoreVariablesList::OnCloseDialogs()
{
    wizard_->hide();
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::GetValue
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
indicators::Variables ScoreVariablesList::GetValue() const
{
    indicators::Variables result;
    indicators::DataTypeFactory types;
    for( int i = 0; i < list_->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = list_->topLevelItem( i );
        const QString name  = item->text( 0 ).remove( 0, 1 );
        const QString type  = item->text( 1 );
        const QString value = item->text( 2 );
        boost::shared_ptr< indicators::Element_ABC > variable = boost::make_shared< indicators::Variable >( name.toStdString(), types.Instanciate( type.toStdString() ), value.toStdString() );
        result.Register( name.toStdString(), variable );
    }
    return result;
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
    if( QTreeWidgetItem* item = list_->currentItem() )
    {
        delete item;
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
    if( QTreeWidgetItem* item = list_->currentItem() )
        emit Insert( item->text( 0 ) );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesList::Draw
// Created: FPO 2011-07-19
// -----------------------------------------------------------------------------
void ScoreVariablesList::Draw( gui::Viewport_ABC& viewport )
{
    wizard_->Draw( viewport );
    if( location_.get() && isVisible() )
    {
        gui::SimpleLocationDrawer drawer( tools_ );
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
    if( QTreeWidgetItem* item = list_->currentItem() )
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
                for( QStringList::iterator it = tempList.begin(); it != tempList.end(); ++it )
                {
                    if( parser_->Parse( QStringList() << *it, point, hint, false ) )
                        location_->AddPoint( point );
                }
            }
        }
    }
}
