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
#include "ActionController.h"
#include "Model.h"
#include "CoordinateConverter.h"
#include "AgentsModel.h"
#include "Team.h"
#include "ObjectType.h"
#include "ObjectTypes.h"
#include "GroupDisplayer.h"
#include "Iterator.h"
#include "ObjectPrototypeAttributes_ABC.h"
#include "CampPrototype.h"
#include "CrossingSitePrototype.h"
#include "LogisticRoutePrototype.h"
#include "NBCPrototype.h"
#include "RotaPrototype.h"
#include "ASN_Messages.h"
#include "LocationCreator.h"
#include "ParametersLayer.h"
#include "Tools.h"
#include "RichLabel.h"

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( QWidget* parent, Controllers& controllers, const Model& model, ParametersLayer& layer )
    : QGroupBox( 2, Qt::Horizontal, tr( "Informations" ), parent )
    , controllers_( controllers )
    , model_( model )
    , umtCoords_( 0 )
    , activeAttributes_( 0 )
{
    new QLabel( tr( "Nom:" ), this );
    name_ = new QLineEdit( this );

    new QLabel( tr( "Camp:" ), this );
    teams_ = new ValuedComboBox< const Team* >( this );

    new QLabel( tr( "Type:" ), this );
    objectTypes_ = new ValuedComboBox< const ObjectType* >( this );

    Resolver< ObjectType >& resolver = model_.objectTypes_;
    Iterator< const ObjectType& > it( resolver.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const ObjectType& element = it.NextElement();
        objectTypes_->AddItem( element.GetName().c_str(), &element );
    }

    location_ = new RichLabel( tr( "Position:" ), this );
    locationLabel_ = new QLabel( tr( "---" ), this );
    locationLabel_->setMinimumWidth( 100 );
    locationLabel_->setAlignment( Qt::AlignCenter );
    locationLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    locationCreator_ = new LocationCreator( this, "Nouvel objet", layer, *this );
    locationCreator_->AddLocationType( tr( "point" ), EnumTypeLocalisation::point );
    locationCreator_->AddLocationType( tr( "ligne" ), EnumTypeLocalisation::line );
    locationCreator_->AddLocationType( tr( "polygone" ), EnumTypeLocalisation::polygon );
    locationCreator_->AddLocationType( tr( "cercle" ), EnumTypeLocalisation::circle );
    controllers.actions_.Register( *locationCreator_ );

    campAttributes_          = new CampPrototype( parent, controllers, model_.agents_ ); campAttributes_->hide();
    crossingSiteAttributes_  = new CrossingSitePrototype( parent );                      crossingSiteAttributes_->hide();
    logisticRouteAttributes_ = new LogisticRoutePrototype( parent );                     logisticRouteAttributes_->hide();
    nbcAttributes_           = new NBCPrototype( parent, model_.objectTypes_ );          nbcAttributes_->hide();
    rotaAttributes_          = new RotaPrototype( parent, model_.objectTypes_ );         rotaAttributes_->hide();

    
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

    if( !locationPoints_.size() )
    {
        location_->Warn( 3000 );
        return false;
    }
    return ( !activeAttributes_ || activeAttributes_->CheckValidity() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::Serialize
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::Serialize( ASN_MsgObjectMagicAction& msg )
{
    ASN1T_MagicActionCreateObject& creation = *msg.GetAsnMsg().action.u.create_object;

    if( !name_->text().isEmpty() )
    {
        creation.m.nomPresent = 1;
        creation.nom = name_->text().ascii();
    }
    creation.camp = teams_->GetValue()->GetId();
    creation.type = (ASN1T_EnumObjectType)objectTypes_->GetValue()->id_;

    creation.localisation.type = locationCreator_->GetCurrentType();
    unsigned nbrPoints = locationPoints_.size();

    creation.localisation.vecteur_point.n = nbrPoints;
    if( nbrPoints )
    {
        umtCoords_ = new ASN1T_CoordUTM[ nbrPoints ];
        creation.localisation.vecteur_point.elem = umtCoords_;

        for( unsigned i = 0; i < nbrPoints; ++i )
        {
            const std::string coord = model_.coordinateConverter_.ConvertToMgrs( locationPoints_[i] );
            creation.localisation.vecteur_point.elem[i] = coord.c_str();
        }
    }

    if( activeAttributes_ )
    {
        creation.m.attributs_specifiquesPresent = 1;
        activeAttributes_->Serialize( creation );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectPrototype::Clean()
{
    delete[] umtCoords_;
    if( activeAttributes_ )
        activeAttributes_->Clean();
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
void ObjectPrototype::Handle( const T_PointVector& points )
{
    locationType_ = locationCreator_->GetCurrentType();
    locationPoints_ = points;
    if(    ( locationType_ == EnumTypeLocalisation::point   && locationPoints_.size() == 1 )
        || ( locationType_ == EnumTypeLocalisation::line    && locationPoints_.size() >= 2 )
        || ( locationType_ == EnumTypeLocalisation::circle  && locationPoints_.size() == 2 )
        || ( locationType_ == EnumTypeLocalisation::polygon && locationPoints_.size() > 2 ) )
        locationLabel_->setText( Tools::ToString( locationType_ ) );
    else
        locationLabel_->setText( tr( "---" ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::Draw
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectPrototype::Draw( const GlTools_ABC& tools ) const
{
    locationCreator_->Draw( locationPoints_, locationType_, tools );
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
