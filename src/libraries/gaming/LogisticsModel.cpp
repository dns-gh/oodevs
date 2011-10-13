// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogisticsModel.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "LogFuneralConsign.h"
#include "LogisticConsignFactory_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogisticsModel::LogisticsModel( LogisticConsignFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogisticsModel::~LogisticsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void LogisticsModel::Purge()
{
    tools::Resolver< LogMaintenanceConsign >::DeleteAll();
    tools::Resolver< LogMedicalConsign >::DeleteAll();
    tools::Resolver< LogSupplyConsign >::DeleteAll();
    tools::Resolver< LogFuneralConsign >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::Delete
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
template< typename Type >
void LogisticsModel::Delete( unsigned long id )
{
    delete tools::Resolver< Type >::Find( id );
    tools::Resolver< Type >::Remove( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message )
{
    tools::Resolver< LogMaintenanceConsign >::Register( message.request().id(), *factory_.CreateMaintenanceConsign( message ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::GetMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogMaintenanceConsign& LogisticsModel::GetMaintenanceConsign( unsigned long id )
{
    return tools::Resolver< LogMaintenanceConsign >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteMaintenanceConsign( unsigned long id )
{
    Delete< LogMaintenanceConsign >( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateMedicalConsign( const sword::LogMedicalHandlingCreation& message )
{
    tools::Resolver< LogMedicalConsign >::Register( message.request().id(), *factory_.CreateMedicalConsign( message ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::GetMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogMedicalConsign& LogisticsModel::GetMedicalConsign( unsigned long id )
{
    return tools::Resolver< LogMedicalConsign >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteMedicalConsign( unsigned long id )
{
    Delete< LogMedicalConsign >( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateSupplyConsign( const sword::LogSupplyHandlingCreation& message )
{
    tools::Resolver< LogSupplyConsign >::Register(  message.request().id(), *factory_.CreateSupplyConsign( message ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::GetSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogSupplyConsign& LogisticsModel::GetSupplyConsign( unsigned long id )
{
    return tools::Resolver< LogSupplyConsign >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteSupplyConsign( unsigned long id )
{
    Delete< LogSupplyConsign >( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateFuneralConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateFuneralConsign( const sword::LogFuneralHandlingCreation& message )
{
    tools::Resolver< LogFuneralConsign >::Register(  message.request().id(), *factory_.CreateFuneralConsign( message ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::GetFuneralConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogFuneralConsign& LogisticsModel::GetFuneralConsign( unsigned long id )
{
    return tools::Resolver< LogFuneralConsign >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteFuneralConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteFuneralConsign( unsigned long id )
{
    Delete< LogFuneralConsign >( id );
}
