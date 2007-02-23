// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MissionInterface_ABC.h"
#include "moc_MissionInterface_ABC.cpp"

#include "ParamPoint.h"
#include "ParamAgent.h"
#include "ParamAgentList.h"
#include "ParamAutomat.h"
#include "ParamAutomatList.h"
#include "ParamPath.h"
#include "ParamPathList.h"
#include "ParamGDH.h"
#include "ParamBool.h"
#include "ParamLocation.h"
#include "ParamLocationList.h"
#include "ParamAgentType.h"
#include "ParamAgentKnowledge.h"
#include "ParamAgentKnowledgeList.h"
#include "ParamPopulationKnowledge.h"
#include "ParamObjectKnowledge.h"
#include "ParamObjectKnowledgeList.h"
#include "ParamObstacle.h"
#include "ParamObstacleList.h"
#include "ParamDirection.h"
#include "ParamNumericField.h"
#include "ParamEquipmentList.h"
#include "ParamHumanWoundList.h"
#include "ParamDotationDType.h"
#include "OptionalParamFunctor_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Viewport_ABC.h"
#include "gaming/ObjectKnowledge_ABC.h"
#include "gaming/AgentKnowledge_ABC.h"
#include "gaming/PopulationKnowledge_ABC.h"
#include "gaming/Lima.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::MissionInterface_ABC( QWidget* parent, Entity_ABC& agent, ActionController& controller )
    : QVBox      ( parent )
    , controller_( controller )
    , agent_     ( agent )
{
    setMargin( 5 );
    setSpacing( 4 );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::~MissionInterface_ABC()
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->RemoveFromController();
    for( CIT_OptionalFunctors it = optionalFunctors_.begin(); it != optionalFunctors_.end(); ++it )
        delete *it;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CheckValidity
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::CheckValidity()
{
    bool b = true;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        b = (*it)->CheckValidity() && b;
    return b;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::IsEmpty
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::IsEmpty() const
{
    return parameters_.empty();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateOkCancelButtons
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateOkCancelButtons()
{
    QHBox* pBox = new QHBox( this );
    QPushButton* pOk = new QPushButton( tr( "Ok" ), pBox );
    QPushButton* pCancel = new QPushButton( tr( "Cancel" ), pBox );
    pOk->setDefault( true );

    connect( pOk, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( pCancel, SIGNAL( clicked() ), parent(), SLOT( hide() ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::AddParameter
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::AddParameter( Param_ABC& parameter, OptionalParamFunctor_ABC* pOptional )
{
    parameters_.push_back( &parameter );
    parameter.SetOptional( pOptional );
    parameter.RegisterIn( controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Commit
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Commit()
{
    for( CIT_Parameters it = parameters_.begin() ; it != parameters_.end() ; ++it )
        (*it)->Commit();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Draw( const GlTools_ABC& tools, Viewport_ABC& extent ) const
{
    for( CIT_Parameters it = parameters_.begin() ; it != parameters_.end() ; ++it )
    {
        const geometry::Point2f p = agent_.Get< Positions >().GetPosition();
        extent.SetHotpoint( p );
        (*it)->Draw( p, extent, tools );
    }
}
