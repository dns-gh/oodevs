// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectPrototype_ABC.h"
#include "moc_ObjectPrototype_ABC.cpp"

#include "FileDialog.h"
#include "GlTools_ABC.h"
#include "LoadableLineEdit.h"
#include "LocationCreator.h"
#include "ObjectAttributePrototypeContainer.h"
#include "ObjectAttributePrototypeFactory_ABC.h"
#include "ObjectPrototypeShapeFileLoader.h"
#include "ParametersLayer.h"
#include "RichLabel.h"
#include "Tools.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Point.h"
#include "tools/Iterator.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype_ABC::ObjectPrototype_ABC( QWidget* parent, Controllers& controllers,
                                         const kernel::CoordinateConverter_ABC& coordinateConverter,
                                         const tools::Resolver_ABC< ObjectType, std::string >& resolver, const Team_ABC& noSideTeam,
                                         ParametersLayer& layer, std::auto_ptr< ObjectAttributePrototypeFactory_ABC > factory )
    : QWidget( parent )
    , coordinateConverter_( coordinateConverter )
    , controllers_( controllers )
    , resolver_  ( resolver )
    , location_  ( 0 )
{
    QBoxLayout* layout = new QBoxLayout( this, QBoxLayout::TopToBottom, 0, 5 );

    // Information box
    {
        Q3GroupBox* infoBox = new Q3GroupBox( 2, Qt::Horizontal, tr( "Information" ), parent );
        layout->setAlignment( Qt::AlignTop );
        layout->addWidget( infoBox );

        new QLabel( tr( "Name:" ), infoBox );
        name_ = new LoadableLineEdit( infoBox, "NAME" );

        new QLabel( tr( "Side:" ), infoBox );
        teams_ = new ValuedComboBox< const Team_ABC* >( infoBox );
        teams_->AddItem( noSideTeam.GetName(), &noSideTeam );

        new QLabel( tr( "Type:" ), infoBox );
        objectTypes_ = new ValuedComboBox< const ObjectType* >( infoBox );
        objectTypes_->setSorting( true );
        connect( objectTypes_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged() ) );

        position_ = new RichLabel( tr( "Location:" ), infoBox );
        locationLabel_ = new QLabel( tr( "---" ), infoBox );
        locationLabel_->setMinimumWidth( 100 );
        locationLabel_->setAlignment( Qt::AlignCenter );
        locationLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );

        locationCreator_ = new LocationCreator( position_, tr( "New object" ), layer, *this );

        new QWidget( infoBox );
        loadFromFileButton_ = new QPushButton(  tr( "Load from file" ), infoBox );
        loadFromFileButton_->setToggleButton( true );
        connect( loadFromFileButton_, SIGNAL( toggled( bool ) ), this, SLOT( LoadFromFile( bool ) ) );
        new QWidget( infoBox );
        loadFromFilePathLabel_ = new QLabel( infoBox );
    }

    // Description box
    {
        descriptionBox_ = new QGroupBox( tr( "Description" ), parent );
        descriptionBox_->setVisible( false );
        descriptionLabel_ = new QLabel( descriptionBox_ );
        descriptionLabel_->setWordWrap( true );
        QBoxLayout* boxLayout = new QBoxLayout( QBoxLayout::TopToBottom, descriptionBox_ );
        boxLayout->addWidget( descriptionLabel_ );
        layout->addWidget( descriptionBox_ );
    }

    // Attribute box
    {
        Q3GroupBox* attributBox = new Q3GroupBox( 1, Qt::Horizontal, tr( "Attributes" ), parent );
        layout->addWidget( attributBox );
        attributes_.reset( new ObjectAttributePrototypeContainer( resolver, factory, attributBox ) );
        // $$$$ AGE 2006-08-11: L'initialisation du reste est delayée... C'est pas terrible
    }

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype_ABC::~ObjectPrototype_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::NotifyUpdated
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::NotifyUpdated( const ModelLoaded& )
{
    attributes_->NotifyUpdated();
    attributes_->Hide();
    FillObjectTypes();
    OnTypeChanged();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::NotifyUpdated
// Created: SBO 2010-05-19
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    objectTypes_->Clear();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::showEvent
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::showEvent( QShowEvent* e )
{
    OnTypeChanged();
    controllers_.Register( *locationCreator_ );
    QWidget::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::hideEvent
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::hideEvent( QHideEvent* )
{
    if( locationCreator_ )
        controllers_.Unregister( *locationCreator_ );
    ResetLocation();
    attributes_->Hide();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::FillObjectTypes
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::FillObjectTypes()
{
    objectTypes_->Clear();
    tools::Iterator< const ObjectType& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const ObjectType& element = it.NextElement();
        const std::string& type = element.GetType();
        if( type == "urban block" || type == "population moving" )
            continue;
        objectTypes_->AddItem( element.GetName(), &element );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::CheckValidity
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
bool ObjectPrototype_ABC::CheckValidity() const
{
    if( !teams_->count() || !teams_->GetValue() )
        return false;
    if( !objectTypes_->count() || !objectTypes_->GetValue() )
        return false;

    if( !location_ && !loader_.get() )
    {
        position_->Warn( 3000 );
        return false;
    }
    return attributes_->CheckValidity( *( teams_->GetValue() ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::Clean()
{
    name_->clear();
    ResetLocation();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::Commit
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::Commit()
{
    if( loader_.get() )
    {
        loader_->StartLoad();
        while( loader_->LoadNext() )
            if( CheckValidity() )
                DoCommit( *( teams_->GetValue() ) );
        name_->clear();
        ResetLocation();
    }
    else
    {
        if( CheckValidity() )
        {
            DoCommit( *( teams_->GetValue() ) );
            name_->clear();
            ResetLocation();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::DoCommit
// Created: BCI 2011-05-10
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::DoCommit( const kernel::Team_ABC& team )
{
    attributes_->Commit( team );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::NotifyCreated( const Team_ABC& team )
{
    if( teams_->GetItemIndex( &team ) != -1 )
        return;
    teams_->AddItem( team.GetName(), &team );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::NotifyUpdated
// Created: SBO 2008-03-28
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::NotifyUpdated( const kernel::Team_ABC& team )
{
    const int index = teams_->GetItemIndex( &team );
    if( index != -1 )
        teams_->changeItem( team.GetName(), index );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::NotifyDeleted
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::NotifyDeleted( const Team_ABC& team )
{
    teams_->RemoveItem( &team );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::OnTypeChanged
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::OnTypeChanged()
{
    LoadFromFile( false );
    if( objectTypes_->Count() != 0 )
    {
        const ObjectType* type = objectTypes_->GetValue();
        if( !type )
            return;
        locationCreator_->Allow( type->CanBePoint(), type->CanBeLine(), type->CanBePolygon(), type->CanBeCircle(), type->CanBeRectangle() );
        if( location_ )
            ResetLocation();
        descriptionBox_->setVisible( !type->GetDescription().empty() );
        descriptionLabel_->setText( type->GetDescription().c_str() );
        attributes_->Select( *type );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::Handle
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::Handle( Location_ABC& location )
{
    ResetLocation();
    location_ = &location;
    if( location.IsValid() )
        locationLabel_->setText( location.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::ResetLocation
// Created: SBO 2008-04-17
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::ResetLocation()
{
    delete location_;
    location_ = 0;
    locationLabel_->setText( tr( "---" ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::Draw
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::Draw( const GlTools_ABC& tools ) const
{
    if( location_ )
        Draw( *location_, geometry::Rectangle2f(), tools );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::Draw
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& /*viewport*/, const GlTools_ABC& tools ) const
{
    if( isVisible() )
        if( const kernel::ObjectType* type = objectTypes_->GetValue() )
        {
            const std::string locationType = location.GetTypeName();
            tools.DrawTacticalGraphics( type->GetSymbol( locationType ), location, true, dynamic_cast< const kernel::Point* >( &location ) != nullptr );
        }
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::LoadFromFile
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::LoadFromFile( bool mustLoadFromFile )
{
    loader_.reset();
    loadFromFilePathLabel_->setText( QString::null );
    if( mustLoadFromFile )
    {
        const ObjectType* type = objectTypes_->Count() != 0 ? objectTypes_->GetValue() : 0;
        if( type )
        {

            tools::Path filename = gui::FileDialog::getOpenFileName( this, tr( "Choose a file" ), "", "Shapefile (*.shp)" );
            if( !filename.IsEmpty() )
            {
                try
                {
                    loader_.reset( new ObjectPrototypeShapeFileLoader( coordinateConverter_, this, filename, *type ) );
                    loadFromFilePathLabel_->setText( QString::fromStdWString( filename.ToUnicode() ) );
                }
                catch( const ObjectPrototypeLoader_ABC::LoadCancelledException& )
                {
                    //NOTHING
                }
                catch( const std::exception& e )
                {
                    QMessageBox::warning( this, tr( "Cannot load %1" ).arg( filename.ToUTF8().c_str() ), tools::GetExceptionMsg( e ).c_str(), QMessageBox::Ok, QMessageBox::NoButton );
                }
            }
        }
    }

    loadFromFileButton_->setOn( loader_.get() != 0 );
    name_->SetLoader( loader_.get() );
    attributes_->SetLoader( loader_.get() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::GetCurrentName
// Created: BCI 2011-05-10
// -----------------------------------------------------------------------------
QString ObjectPrototype_ABC::GetCurrentName() const
{
    return name_->text();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::GetCurrentLocation
// Created: BCI 2011-05-10
// -----------------------------------------------------------------------------
const kernel::Location_ABC& ObjectPrototype_ABC::GetCurrentLocation() const
{
    if( loader_.get() )
        return loader_->GetCurrentLocation();
    return *location_;
}
