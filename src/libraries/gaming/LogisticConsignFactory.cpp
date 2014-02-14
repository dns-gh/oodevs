// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogisticConsignFactory.h"
#include "AgentsModel.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "LogFuneralConsign.h"
#include "Model.h"
#include "StaticModel.h"
#include "TeamsModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"

// -----------------------------------------------------------------------------
// Name: LogisticConsignFactory constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogisticConsignFactory::LogisticConsignFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, const Simulation& simulation )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
    , simulation_( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignFactory destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogisticConsignFactory::~LogisticConsignFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignFactory::CreateMaintenanceConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign* LogisticConsignFactory::CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message, kernel::Agent_ABC& consumer )
{
    return new LogMaintenanceConsign( controllers_.controller_, message, model_, static_.types_, static_.objectTypes_, simulation_, consumer );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignFactory::CreateSupplyConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign* LogisticConsignFactory::CreateSupplyConsign( const sword::LogSupplyHandlingCreation& message )
{
    return new LogSupplyConsign( controllers_.controller_, model_.agents_, model_.agents_, model_.teams_, static_.objectTypes_, simulation_, message );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignFactory::CreateMedicalConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMedicalConsign* LogisticConsignFactory::CreateMedicalConsign( const sword::LogMedicalHandlingCreation& message, kernel::Agent_ABC& consumer )
{
    return new LogMedicalConsign( controllers_.controller_, model_.agents_, simulation_, message, consumer );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignFactory::CreateFuneralConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogFuneralConsign* LogisticConsignFactory::CreateFuneralConsign( const sword::LogFuneralHandlingCreation& message, kernel::Agent_ABC& consumer )
{
    return new LogFuneralConsign( controllers_.controller_, message, model_.agents_, model_.agents_, model_.teams_, static_.objectTypes_, simulation_, consumer );
}
