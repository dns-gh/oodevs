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
#include "preparation/ObjectsModel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "CampPrototype.h"
//#include "CrossingSitePrototype.h"
//#include "LogisticRoutePrototype.h"
//#include "NBCPrototype.h"
//#include "RotaPrototype.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( QWidget* parent, Controllers& controllers, const StaticModel& model, ObjectsModel& objectsModel, ParametersLayer& layer )
    : ObjectPrototype_ABC( parent, controllers, model.objectTypes_, layer )
    , objectsModel_( objectsModel )
    , creation_( 0 )
{
    campAttributes_          = new CampPrototype         ( parent, controllers, objectsModel, creation_ );        campAttributes_->hide();
//    crossingSiteAttributes_  = new CrossingSitePrototype ( parent, creation_ );                     crossingSiteAttributes_->hide();
//    logisticRouteAttributes_ = new LogisticRoutePrototype( parent, creation_ );                     logisticRouteAttributes_->hide();
//    nbcAttributes_           = new NBCPrototype          ( parent, model.objectTypes_, creation_ ); nbcAttributes_->hide();
//    rotaAttributes_          = new RotaPrototype         ( parent, model.objectTypes_, creation_ ); rotaAttributes_->hide();
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
    creation_ = objectsModel_.CreateObject( const_cast< ObjectType& >( GetType() ), const_cast< Team_ABC& >( *teams_->GetValue() ), *location_ );
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