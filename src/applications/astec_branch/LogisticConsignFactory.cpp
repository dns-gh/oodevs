// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticConsignFactory.h"
#include "Model.h"
#include "AgentsModel.h"
#include "LogMaintenanceConsign.h"
#include "LogSupplyConsign.h"
#include "LogMedicalConsign.h"
#include "ObjectTypes.h"
#include "Controllers.h"

// -----------------------------------------------------------------------------
// Name: LogisticConsignFactory constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogisticConsignFactory::LogisticConsignFactory( Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
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
LogMaintenanceConsign* LogisticConsignFactory::CreateMaintenanceConsign( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& message )
{
    return new LogMaintenanceConsign( controllers_.controller_, model_.agents_, message );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignFactory::CreateSupplyConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign* LogisticConsignFactory::CreateSupplyConsign( const ASN1T_MsgLogRavitaillementTraitementCreation& message )
{
    return new LogSupplyConsign( controllers_.controller_, model_.agents_, model_.objectTypes_, message );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticConsignFactory::CreateMedicalConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMedicalConsign* LogisticConsignFactory::CreateMedicalConsign( const ASN1T_MsgLogSanteTraitementHumainCreation& message )
{
    return new LogMedicalConsign( controllers_.controller_, model_.agents_, message );
}
