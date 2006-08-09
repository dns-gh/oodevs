// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ObjectPrototype.h"
#include "moc_ObjectPrototype.cpp"

#include "DisplayBuilder.h"
#include "astec_kernel/Controllers.h"
#include "astec_kernel/ActionController.h"
#include "astec_gaming/StaticModel.h"
#include "astec_kernel/CoordinateConverter_ABC.h"
#include "astec_gaming/AgentsModel.h"
#include "astec_kernel/Team_ABC.h"
#include "astec_kernel/ObjectType.h"
#include "astec_kernel/ObjectTypes.h"
#include "GroupDisplayer.h"
#include "astec_kernel/Iterator.h"
#include "ObjectPrototypeAttributes_ABC.h"
#include "CampPrototype.h"
#include "CrossingSitePrototype.h"
#include "LogisticRoutePrototype.h"
#include "NBCPrototype.h"
#include "RotaPrototype.h"
#include "astec_gaming/ASN_Messages.h"
#include "LocationCreator.h"
#include "ParametersLayer.h"
#include "Tools.h"
#include "RichLabel.h"
#include "astec_gaming/CoordinateConverter.h"
#include "astec_kernel/Location_ABC.h"

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( QWidget* parent, Controllers& controllers, const StaticModel& model, ParametersLayer& layer )
    : QGroupBox( 2, Qt::Horizontal, tr( "Informations" ), parent )
    , controllers_( controllers )
    , static_( model )
    , layer_( layer )
    , serializer_( model.coordinateConverter_, creation_.localisation )
    , activeAttributes_( 0 )
    , location_( 0 )
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

    locationCreator_ = new LocationCreator( position_, "Nouvel objet", layer_, *this );

    msg_.GetAsnMsg().action.t                 = T_MsgObjectMagicAction_action_create_object;
    msg_.GetAsnMsg().action.u.create_object   = &creation_;

    campAttributes_          = new CampPrototype( parent, controllers, creation_ );          campAttributes_->hide();
    crossingSiteAttributes_  = new CrossingSitePrototype( parent, creation_ );               crossingSiteAttributes_->hide();
    logisticRouteAttributes_ = new LogisticRoutePrototype( parent, creation_ );              logisticRouteAttributes_->hide();
    nbcAttributes_           = new NBCPrototype( parent, static_.objectTypes_, creation_ );  nbcAttributes_->hide();
    rotaAttributes_          = new RotaPrototype( parent, static_.objectTypes_, creation_ ); rotaAttributes_->hide();
    
    controllers.Register( *this );

    connect( objectTypes_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::NotifyUpdated
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ObjectPrototype::NotifyUpdated( const ModelLoaded& )
{
    FillObjectTypes();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::showEvent
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void ObjectPrototype::showEvent( QShowEvent* )
{
    FillObjectTypes();
    controllers_.Register( *locationCreator_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::FillObjectTypes
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ObjectPrototype::FillObjectTypes()
{
    objectTypes_->Clear();
    Resolver< ObjectType >& resolver = static_.objectTypes_; // $$$$ AGE 2006-05-03: evenement de chargement des données statiques
    Iterator< const ObjectType& > it( resolver.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const ObjectType& element = it.NextElement();
        objectTypes_->AddItem( element.GetName().c_str(), &element );
    }
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPrototype::hideEvent
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void ObjectPrototype::hideEvent( QHideEvent* )
{
    if( locationCreator_ )
        controllers_.Remove( *locationCreator_ );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPrototype destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::~ObjectPrototype()
{
    delete location_;
    delete campAttributes_;
    delete crossingSiteAttributes_;
    delete logisticRouteAttributes_;
    delete nbcAttributes_;
    delete rotaAttributes_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::CheckValidity
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
bool ObjectPrototype::CheckValidity() const
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
// Name: ObjectPrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::Commit( Publisher_ABC& publisher )
{
    msg_.GetAsnMsg().oid_objet = GetType().manager_.GetFreeIdentifier();

    if( !name_->text().isEmpty() )
    {
        creation_.m.nomPresent = 1;
        creation_.nom = name_->text().ascii();
    }
    creation_.camp = teams_->GetValue()->GetId();
    creation_.type = (ASN1T_EnumObjectType)objectTypes_->GetValue()->id_;
    if( location_ )
        serializer_.Serialize( *location_ );

    if( activeAttributes_ )
    {
        creation_.m.attributs_specifiquesPresent = 1;
        activeAttributes_->Commit();
    }
    else
        creation_.m.attributs_specifiquesPresent = 0;

    msg_.Send( publisher );
    Clean();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectPrototype::Clean()
{
    if( activeAttributes_ )
        activeAttributes_->Clean();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::NotifyCreated( const Team_ABC& team )
{
    if( teams_->GetItemIndex( &team ) != -1 )
        return;
    teams_->AddItem( team.GetName().c_str(), &team );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPrototype::NotifyDeleted
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::NotifyDeleted( const Team_ABC& team )
{
    teams_->RemoveItem( &team );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::OnTypeChanged
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::OnTypeChanged( int )
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
// Name: ObjectPrototype::Handle
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectPrototype::Handle( Location_ABC& location )
{
    delete location_;
    location_ = &location;
    if( location.IsValid() )
        locationLabel_->setText( location.GetName().c_str() );
    else
        locationLabel_->setText( tr( "---" ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::Draw
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectPrototype::Draw( const GlTools_ABC& tools ) const
{
    if( isVisible() && location_ )
        location_->Draw( tools );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::GetType
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
const ObjectType& ObjectPrototype::GetType() const
{
    const ObjectType* type = objectTypes_->GetValue();
    if( !type )
        throw std::runtime_error( "Object prototype has no type" );
    return *type;
}
