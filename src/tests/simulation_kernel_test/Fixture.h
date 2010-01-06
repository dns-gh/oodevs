// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Fixture_h_
#define __Fixture_h_

#include "StubMIL_AgentPion.h"
#include "StubMIL_AgentTypePion.h"
#include "StubMIL_Automate.h"
#include "StubMIL_AutomateType.h"
#include "Decision/DEC_Model.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "AlgorithmsFactories.h"
#include <xeumeuleu/xml.h>

struct FixturePion
{
    FixturePion()
        : algorithmsFactories_( new AlgorithmsFactories())
    {
        xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
        xis.start( "main" );
        std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;

        pModel_.reset( new DEC_Model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes ) );
        pType_.reset( new StubMIL_AgentTypePion( *pModel_ ) );
        pTypeAutomat_.reset( new StubMIL_AutomateType( *pModel_ ) );
        pAutomat_.reset( new StubMIL_Automate( *pTypeAutomat_ ) );
        pPion_.reset( new StubMIL_AgentPion( *pType_, *pAutomat_, *algorithmsFactories_ ) );
    }

    std::auto_ptr< AlgorithmsFactories >   algorithmsFactories_;
    std::auto_ptr< DEC_Model >             pModel_;
    std::auto_ptr< StubMIL_AgentTypePion > pType_;
    std::auto_ptr< StubMIL_AutomateType >  pTypeAutomat_;
    std::auto_ptr< MIL_Automate >          pAutomat_;
    std::auto_ptr< StubMIL_AgentPion >     pPion_;
    MIL_EffectManager                      effectManager_;
};

struct FixtureAutomate
{
    FixtureAutomate()
    {
        xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
        xis.start( "main" );
        std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;
        pModel_.reset( new DEC_Model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes ) );
        pType_.reset( new StubMIL_AutomateType( *pModel_ ) );
        pAutomat_.reset( new StubMIL_Automate( *pType_ ) );
    }

    std::auto_ptr< DEC_Model >            pModel_;
    std::auto_ptr< StubMIL_AutomateType > pType_;
    std::auto_ptr< MIL_Automate >         pAutomat_;
    MIL_EffectManager                     effectManager_;
};

#endif // __Fixture_h_
