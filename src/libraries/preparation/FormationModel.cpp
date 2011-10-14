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
#include "StaticModel.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "LogisticBaseStates.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationModel constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationModel::FormationModel( kernel::Controllers& controllers, FormationFactory_ABC& formationFactory,
                                const tools::Resolver< kernel::Automat_ABC>& automatResolver, const StaticModel& staticModel )
    : controllers_    ( controllers )
    , factory_        ( formationFactory )
    , levels_         ( *new FormationLevels() )
    , automatResolver_( automatResolver )
    , staticModel_    ( staticModel )
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
kernel::Formation_ABC* FormationModel::Create( kernel::Entity_ABC& parent, unsigned int levelId, const QString& name )
{
    const HierarchyLevel_ABC* level = levels_.Resolve( levelId );
    if( !level )
        return 0;
    Formation_ABC* formation = factory_.Create( parent, *level, name );
    Register( formation->GetId(), *formation );
    return formation;
}

// -----------------------------------------------------------------------------
// Name: FormationModel::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void FormationModel::Create( xml::xistream& xis, kernel::Entity_ABC& parent, Model& model, std::string& loadingErrors )
{
    Formation_ABC* formation = factory_.Create( xis, parent, levels_ );
    Register( formation->GetId(), *formation );
    xis >> xml::list( "formation"   , *this               , &FormationModel::Create    , *(Entity_ABC*)formation, model, loadingErrors )
        >> xml::list( "automat"     , model.agents_       , &AgentsModel::CreateAutomat, *formation, model.limits_, loadingErrors )
        >> xml::list( "lima"        , model.limits_       , &LimitsModel::CreateLima   , *(Entity_ABC*)formation )
        >> xml::list( "limit"       , model.limits_       , &LimitsModel::CreateLimit  , *(Entity_ABC*)formation );
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
