// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ConsignHelper.h"
#include "Tools/MIL_IDManager.h"

namespace
{

MIL_IDManager idManager;

}  // namespace

unsigned int logistic::NewConsignId()
{
    return idManager.GetId(); 
}

unsigned int logistic::RegisterConsignId( unsigned int id )
{
    return idManager.GetId( id, true );
}
