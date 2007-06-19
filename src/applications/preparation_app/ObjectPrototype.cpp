// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObjectPrototype.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamsModel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "CampPrototype.h"
#include "CrossingSitePrototype.h"
#include "LogisticRoutePrototype.h"
#include "NBCPrototype.h"
#include "RotaPrototype.h"
#include "MinePrototype.h"
#include "preparation/EnumTypes.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( QWidget* parent, Controllers& controllers, const StaticModel& model, TeamsModel& teamsModel, ParametersLayer& layer )
    : ObjectPrototype_ABC( parent, controllers, model.objectTypes_, layer )
    , teamsModel_( teamsModel )
    , creation_( 0 )
{
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
void ObjectPrototype::Commit()
{
    const QString name = name_->text();
    const kernel::Team_ABC* team = teams_->GetValue();
    
    const E_ObstacleType obstacleType = obstacleTypes_->GetValue();
    const bool reservedObstacleActivated = reservedObstacleActivated_->isChecked();

    creation_ = teamsModel_.CreateObject( *team, const_cast< ObjectType& >( GetType() ), name, Enum_ObstacleType( obstacleType ), reservedObstacleActivated, *location_ );
    if( creation_ && activeAttributes_ )
        activeAttributes_->Commit();
    creation_ = 0;
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