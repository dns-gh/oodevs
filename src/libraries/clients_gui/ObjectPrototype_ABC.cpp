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

#include "DisplayBuilder.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Iterator.h"
#include "clients_kernel/Location_ABC.h"

#include "LocationCreator.h"
#include "ParametersLayer.h"
#include "Tools.h"
#include "RichLabel.h"
#include "GroupDisplayer.h"
#include "ObjectPrototypeAttributes_ABC.h"
#include "CampPrototype_ABC.h"
#include "CrossingSitePrototype_ABC.h"
#include "LogisticRoutePrototype_ABC.h"
#include "NBCPrototype_ABC.h"
#include "RotaPrototype_ABC.h"

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype_ABC::ObjectPrototype_ABC( QWidget* parent, Controllers& controllers, const Resolver< ObjectType >& resolver, ParametersLayer& layer )
    : QGroupBox( 2, Qt::Horizontal, tr( "Informations" ), parent )
    , controllers_( controllers )
    , resolver_( resolver )
    , location_( 0 )
    , activeAttributes_( 0 )
    , campAttributes_( 0 )
    , crossingSiteAttributes_( 0 )
    , logisticRouteAttributes_( 0 )
    , nbcAttributes_( 0 )
    , rotaAttributes_( 0 )
{
    new QLabel( tr( "Nom:" ), this );
    name_ = new QLineEdit( this );

    new QLabel( tr( "Camp:" ), this );
    teams_ = new ValuedComboBox< const Team_ABC* >( this );

    new QLabel( tr( "Type:" ), this );
    objectTypes_ = new ValuedComboBox< const ObjectType* >( this );

    position_ = new RichLabel( tr( "Position:" ), this );
    locationLabel_ = new QLabel( tr( "---" ), this );
    locationLabel_->setMinimumWidth( 100 );
    locationLabel_->setAlignment( Qt::AlignCenter );
    locationLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    locationCreator_ = new LocationCreator( position_, "Nouvel objet", layer, *this );

    // $$$$ AGE 2006-08-11: L'initialisation du reste est delayée... C'est pas terrible

    controllers.Register( *this );

    connect( objectTypes_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::NotifyUpdated
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::NotifyUpdated( const ModelLoaded& )
{
    FillObjectTypes();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::showEvent
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::showEvent( QShowEvent* e )
{
    FillObjectTypes();
    controllers_.Register( *locationCreator_ );
    QGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::FillObjectTypes
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::FillObjectTypes()
{
    objectTypes_->Clear();
    Iterator< const ObjectType& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const ObjectType& element = it.NextElement();
        objectTypes_->AddItem( element.GetName().c_str(), &element );
    }
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::hideEvent
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::hideEvent( QHideEvent* )
{
    if( locationCreator_ )
        controllers_.Remove( *locationCreator_ );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype_ABC::~ObjectPrototype_ABC()
{
    delete location_;
    delete campAttributes_;
    delete crossingSiteAttributes_;
    delete logisticRouteAttributes_;
    delete nbcAttributes_;
    delete rotaAttributes_;
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

    if( ! location_ )
    {
        position_->Warn( 3000 );
        return false;
    }
    return ( !activeAttributes_ || activeAttributes_->CheckValidity() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::Clean()
{
    if( activeAttributes_ )
        activeAttributes_->Clean();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::NotifyCreated( const Team_ABC& team )
{
    if( teams_->GetItemIndex( &team ) != -1 )
        return;
    teams_->AddItem( team.GetName().c_str(), &team );
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
void ObjectPrototype_ABC::OnTypeChanged( int )
{
    const ObjectType* type = objectTypes_->GetValue();
    if( !type )
    {
        activeAttributes_ = 0;
        return;
    }
    ObjectPrototypeAttributes_ABC* previous = activeAttributes_;

    switch( type->id_ )
    {
        case EnumObjectType::camp_prisonniers:
        case EnumObjectType::camp_refugies:
            activeAttributes_ = campAttributes_;
            break;
        case EnumObjectType::itineraire_logistique:
            activeAttributes_ = logisticRouteAttributes_;
            break;
        case EnumObjectType::nuage_nbc:
        case EnumObjectType::zone_nbc:
            activeAttributes_ = nbcAttributes_;
            break;
        case EnumObjectType::rota:
            activeAttributes_ = rotaAttributes_;
            break;
        case EnumObjectType::site_franchissement:
            activeAttributes_ = crossingSiteAttributes_;
            break;
        default:
            activeAttributes_ = 0;
            break;
    }
    if( previous && previous != activeAttributes_ )
        previous->hide();

    if( activeAttributes_ && previous != activeAttributes_ )
        activeAttributes_->show();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::Handle
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::Handle( Location_ABC& location )
{
    delete location_;
    location_ = &location;
    if( location.IsValid() )
        locationLabel_->setText( location.GetName().c_str() );
    else
        locationLabel_->setText( tr( "---" ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype_ABC::Draw
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectPrototype_ABC::Draw( const GlTools_ABC& tools ) const
{
    if( isVisible() && location_ )
        location_->Draw( tools );
}

