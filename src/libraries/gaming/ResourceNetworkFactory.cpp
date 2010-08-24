// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ResourceNetworkFactory.h"
#include "Model.h"
#include "ObjectsModel.h"
#include "ResourceNetwork.h"
#include "UrbanModel.h"

// -----------------------------------------------------------------------------
// Name: ResourceNetworkFactory constructor
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
ResourceNetworkFactory::ResourceNetworkFactory( kernel::Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkFactory destructor
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
ResourceNetworkFactory::~ResourceNetworkFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkFactory::Create
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
ResourceNetwork* ResourceNetworkFactory::Create( unsigned int id, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg, bool isUrban, kernel::PropertiesDictionary& dico )
{
    return new ResourceNetwork( controllers_, id, isUrban, model_.urbanObjects_, model_.objects_, msg, dico );
}
