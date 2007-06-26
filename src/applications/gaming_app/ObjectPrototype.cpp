// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectPrototype.h"
#include "gaming/StaticModel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Team_ABC.h"
#include "CampPrototype.h"
#include "CrossingSitePrototype.h"
#include "LogisticRoutePrototype.h"
#include "NBCPrototype.h"
#include "RotaPrototype.h"
#include "MinePrototype.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( QWidget* parent, Controllers& controllers, const StaticModel& model, ParametersLayer& layer )
    : ObjectPrototype_ABC( parent, controllers, model.objectTypes_, layer )
    , msg_()
    , creation_()
    , serializer_( model.coordinateConverter_, creation_.location )
{
    msg_().action.t                 = T_MsgObjectMagicAction_action_create_object;
    msg_().action.u.create_object   = &creation_;

    campAttributes_          = new CampPrototype         ( parent, controllers, creation_ );        campAttributes_->hide();
    crossingSiteAttributes_  = new CrossingSitePrototype ( parent, creation_ );                     crossingSiteAttributes_->hide();
    logisticRouteAttributes_ = new LogisticRoutePrototype( parent, creation_ );                     logisticRouteAttributes_->hide();
    nbcAttributes_           = new NBCPrototype          ( parent, model.objectTypes_, creation_ ); nbcAttributes_->hide();
    rotaAttributes_          = new RotaPrototype         ( parent, model.objectTypes_, creation_ ); rotaAttributes_->hide();
    mineAttributes_          = new MinePrototype         ( parent, creation_ );                     mineAttributes_->hide();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::~ObjectPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::Commit( Publisher_ABC& publisher )
{
    creation_.m.namePresent = 0;
    if( !name_->text().isEmpty() )
    {
        creation_.m.namePresent = 1;
        creation_.name = name_->text().ascii();
    }

    creation_.m.reserved_obstacle_activatedPresent = creation_.m.obstacle_typePresent = GetType().CanBeReservedObstacle();
    if( creation_.m.obstacle_typePresent )
    {
        creation_.obstacle_type = (ASN1T_EnumObstacleType)obstacleTypes_->GetValue();
        creation_.m.reserved_obstacle_activatedPresent = creation_.obstacle_type == eObstacleType_Reserved;
        creation_.reserved_obstacle_activated = reservedObstacleActivated_->isChecked();
    }
    creation_.team = teams_->GetValue()->GetId();
    creation_.type = (ASN1T_EnumObjectType)objectTypes_->GetValue()->id_;
    if( location_ )
        serializer_.Serialize( *location_ );

    if( activeAttributes_ )
    {
        creation_.m.specific_attributesPresent = 1;
        activeAttributes_->Commit();
    }
    else
        creation_.m.specific_attributesPresent = 0;

    msg_.Send( publisher );
    Clean();
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