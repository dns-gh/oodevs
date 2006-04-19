// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectPrototype.h"
#include "moc_ObjectPrototype.cpp"

#include "DisplayBuilder.h"
#include "Controllers.h"
#include "Model.h"
#include "AgentsModel.h"
#include "Team.h"
#include "ObjectType.h"
#include "ObjectTypes.h"
#include "GroupDisplayer.h"
#include "Iterator.h"
#include "ObjectPrototypeAttributes_ABC.h"
#include "CampPrototype.h"
#include "CrossingSitePrototype.h"
#include "ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( QWidget* parent, Controllers& controllers, const Model& model )
    : controllers_( controllers )
    , model_( model )
    , activeAttributes_( 0 )
{
    display_ = new DisplayBuilder( parent );
    display_->AddGroup( "Informations" )
                .AddTextField( "Nom:" );

    new QLabel( qApp->tr( "Camp:" ), &display_->Group( "Informations" ) ); // $$$$ SBO 2006-04-19: LabelDisplayer somehow
    teams_ = new ValuedComboBox< const Team* >( &display_->Group( "Informations" ) );

    new QLabel( qApp->tr( "Type:" ), &display_->Group( "Informations" ) ); // $$$$ SBO 2006-04-19: LabelDisplayer somehow
    objectTypes_ = new ValuedComboBox< const ObjectType* >( &display_->Group( "Informations" ) );

    Resolver< ObjectType >& resolver = model_.objectTypes_;
    Iterator< const ObjectType& > it( resolver.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const ObjectType& element = it.NextElement();
        objectTypes_->AddItem( element.GetName().c_str(), &element );
    }

    campAttributes_ = new CampPrototype( *display_, controllers, model_.agents_ );
    campAttributes_->Hide();

    crossingSiteAttributes_ = new CrossingSitePrototype( *display_ );
    crossingSiteAttributes_->Hide();

    //.AddPosition( "Position:" )
    controllers.Register( *this );

    connect( objectTypes_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged( int ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPrototype destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::~ObjectPrototype()
{
    delete campAttributes_;
    delete crossingSiteAttributes_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::CheckValidity
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
bool ObjectPrototype::CheckValidity() const
{
    return teams_->GetValue() 
        && objectTypes_->GetValue() 
        && ( !activeAttributes_ || activeAttributes_->CheckValidity() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::Serialize
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::Serialize( ASN_MsgObjectMagicAction& msg ) const
{
    ASN1T_MagicActionCreateObject& creation = *msg.GetAsnMsg().action.u.create_object;

    creation.camp = teams_->GetValue()->GetId();
    //creation.localisation = ;
//    creation.name = ;
    creation.type = (ASN1T_EnumObjectType)objectTypes_->GetValue()->id_;

    if( activeAttributes_ )
        activeAttributes_->Serialize( creation );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::NotifyCreated( const Team& team )
{
    if( teams_->GetItemIndex( &team ) != -1 )
        return;
    teams_->AddItem( team.GetName().c_str(), &team );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPrototype::NotifyUpdated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::NotifyUpdated( const Team& /*team*/ )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPrototype::NotifyDeleted
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::NotifyDeleted( const Team& team )
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
//        case EnumObjectType::itineraire_logistique:
//            attributes_.reset( new ObjectPrototypeLogisticRouteAttributes( controllers_.controller_ ) );
//            break;
//        case EnumObjectType::nuage_nbc:
//        case EnumObjectType::zone_nbc:
//            attributes_.reset( new ObjectPrototypeNBCAttributes( controllers_.controller_, model_.objectTypes_ ) );
//            break;
//        case EnumObjectType::rota:
//            attributes_.reset( new ObjectPrototypeRotaAttributes( controllers_.controller_, model_.objectTypes_ ) );
//            break;
        case EnumObjectType::site_franchissement:
            activeAttributes_ = crossingSiteAttributes_;
            break;
        default:
            activeAttributes_ = 0;
            break;
    }
    if( previous && previous != activeAttributes_ )
        previous->Hide();

    if( activeAttributes_ && previous != activeAttributes_ )
        activeAttributes_->Show();
}
