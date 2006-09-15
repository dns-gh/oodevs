// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamObstacle.h"
#include "ParamLocation.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Resolver.h"

#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamObstacle constructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::ParamObstacle( QWidget* parent, ASN1T_MissionGenObject& asnObject, const QString& label, const ObjectTypes& objectTypes, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : QGroupBox( 2, Qt::Horizontal, label, parent )
    , asnObject_( asnObject )
{
    new QLabel( tr( "Type:" ), this );

    typeCombo_ = new ValuedComboBox< const ObjectType* >( this );
    typeCombo_->setSorting( true );
    Iterator< const ObjectType& > it = objectTypes.Resolver< ObjectType >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const ObjectType& type = it.NextElement();
        typeCombo_->AddItem( type.GetName(), &type );
    }

    new QLabel( tr( "Sous type:" ), this );
    preliminaryCombo_ = new QComboBox( this );
    for( int i = 0; i < eNbrMissionGenSousTypeObstacle; ++i )
        preliminaryCombo_->insertItem( ENT_Tr::ConvertFromMissionGenSousTypeObstacle( ( E_MissionGenSousTypeObstacle )i ).c_str(), i );

    new QLabel( tr( "Urgence:" ), this );
    urgencyCombo_ = new QComboBox( this );
    for( int i = 0; i < eNbrMissionGenUrgence; ++i )
        urgencyCombo_->insertItem( ENT_Tr::ConvertFromMissionGenUrgence( ( E_MissionGenUrgence )i ).c_str(), i );

    new QLabel( tr( "Priorité:" ), this );
    priorityCombo_ = new QComboBox( this );
    for( int i = 0; i < eNbrMissionGenPriorite; ++i )
        priorityCombo_->insertItem( ENT_Tr::ConvertFromMissionGenPriorite( ( E_MissionGenPriorite )i ).c_str(), i );

    new QLabel( tr( "Localisation:" ), this );
    location_ = new ParamLocation( this, asnObject.pos_obstacle, label, layer, converter );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacle destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::~ParamObstacle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::RemoveFromController
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacle::RemoveFromController()
{
    location_->RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::RegisterIn
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacle::RegisterIn( ActionController& controller )
{
    location_->RegisterIn( controller );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CheckValidity
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
bool ParamObstacle::CheckValidity()
{
    return location_->CheckValidity();
}


// -----------------------------------------------------------------------------
// Name: ParamObstacle::Commit
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacle::Commit()
{
    asnObject_.type_obstacle = (ASN1T_EnumObjectType)typeCombo_->GetValue()->id_;
    asnObject_.urgence = (ASN1T_EnumMissionGenUrgence)urgencyCombo_->currentItem();
    asnObject_.preliminaire = (ASN1T_EnumMissionGenSousTypeObstacle)preliminaryCombo_->currentItem();
    asnObject_.priorite = (ASN1T_EnumMissionGenPriorite)priorityCombo_->currentItem();
    location_->Commit();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CommitTo
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacle::CommitTo( ASN1T_MissionGenObject& destination )
{
    destination.oid_obstacle_planifie = asnObject_.oid_obstacle_planifie;
    destination.preliminaire = asnObject_.preliminaire;
    destination.type_obstacle = asnObject_.type_obstacle;
    destination.urgence = asnObject_.urgence;
    destination.priorite = asnObject_.priorite;
    location_->CommitTo( destination.pos_obstacle );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Draw
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void ParamObstacle::Draw( const geometry::Point2f& point, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const
{
    location_->Draw( point, viewport, tools );
}
