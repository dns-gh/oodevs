// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationModel.h"
#include "FormationFactory_ABC.h"
#include "AgentsModel.h"
#include "Model.h"
#include "LimitsModel.h"
#include "IntelligencesModel.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: FormationModel constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationModel::FormationModel( kernel::Controllers& controllers, FormationFactory_ABC& formationFactory )
    : controllers_( controllers )
    , factory_( formationFactory )
    , levels_( *new FormationLevels() )
{
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: FormationModel destructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationModel::~FormationModel()
{
    delete &levels_;
    Purge();
    controllers_.Unregister( *this );
}
    
// -----------------------------------------------------------------------------
// Name: FormationModel::Create
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
kernel::Formation_ABC* FormationModel::Create( kernel::Entity_ABC& parent, unsigned int levelId )
{
    const HierarchyLevel_ABC* level = levels_.Resolve( levelId );
    if( !level )
        return 0;
    Formation_ABC* formation = factory_.Create( parent, *level );
    Register( formation->GetId(), *formation );
    return formation;
}

// -----------------------------------------------------------------------------
// Name: FormationModel::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void FormationModel::Create( xml::xistream& xis, kernel::Entity_ABC& parent, Model& model )
{
    Formation_ABC* formation = factory_.Create( xis, parent, levels_ );
    Register( formation->GetId(), *formation );
    xis >> list( "formation"   , *this               , &FormationModel::Create    , *(Entity_ABC*)formation, model )
        >> list( "automat"     , model.agents_       , &AgentsModel::CreateAutomat, *formation, model.limits_ )
        >> list( "lima"        , model.limits_       , &LimitsModel::CreateLima   , *(Entity_ABC*)formation )
        >> list( "limit"       , model.limits_       , &LimitsModel::CreateLimit  , *(Entity_ABC*)formation )
        >> list( "intelligence", model.intelligences_, &IntelligencesModel::Create, *(Entity_ABC*)formation );
}

// -----------------------------------------------------------------------------
// Name: FormationModel::Purge
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
void FormationModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: FormationModel::NotifyDeleted
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
void FormationModel::NotifyDeleted( const Formation_ABC& formation )
{
    Remove( formation.GetId() );
}
