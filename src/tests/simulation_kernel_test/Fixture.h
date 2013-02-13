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

#include "MockMIL_Time_ABC.h"
#include "StubMIL_AgentPion.h"
#include "StubMIL_AgentTypePion.h"
#include "StubMIL_Automate.h"
#include "StubMIL_AutomateType.h"
#include "Decision/DEC_Model.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "AlgorithmsFactories.h"
#include "tools/RealFileLoaderObserver_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/noncopyable.hpp>

class Observer : public tools::RealFileLoaderObserver_ABC
{
public:
    Observer() {}
    virtual ~Observer() {}
    virtual bool NotifyInvalidXml          ( const std::string&, const xml::exception& ) { return false; };
    virtual void NotifyNoXmlSchemaSpecified( const std::string& ) {};
    virtual void NotifyFileMigrated        ( const std::string&, const std::string&, const std::string& ) {};
};

struct FixturePion : private boost::noncopyable
{
    FixturePion( MIL_EffectManager& effectManager )
        : effectManager_( effectManager )
        , observer_()
        , config_( observer_ )
    {
        xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms' id='12' name='stuff'/>" );
        xis >> xml::start( "main" );
        std::map< std::string, const MIL_MissionType_ABC* > missionTypes;

        pModel_.reset( new DEC_Model( "test", xis, BOOST_RESOLVE( "." ), missionTypes, false, BOOST_RESOLVE( "resources" ) ) );
        MockMIL_Time_ABC time;
        MOCK_EXPECT( time.GetCurrentTimeStep ).returns( 1u );
        pType_.reset( new StubMIL_AgentTypePion( *pModel_ ) );
        pTypeAutomat_.reset( new StubMIL_AutomateType( *pModel_ ) );
        pAutomat_.reset( new StubMIL_Automate( *pTypeAutomat_ ) );
        pPion_.reset( new StubMIL_AgentPion( *pType_, *pAutomat_, algorithmsFactories_, xis, config_ ) );
    }
    AlgorithmsFactories                    algorithmsFactories_;
    MIL_EffectManager&                     effectManager_;
    Observer                               observer_;
    MIL_Config                             config_;
    std::auto_ptr< DEC_Model >             pModel_;
    std::auto_ptr< StubMIL_AgentTypePion > pType_;
    std::auto_ptr< StubMIL_AutomateType >  pTypeAutomat_;
    std::auto_ptr< MIL_Automate >          pAutomat_;
    std::auto_ptr< StubMIL_AgentPion >     pPion_;
};

struct FixtureAutomate : private boost::noncopyable
{
    FixtureAutomate()
    {
        xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
        xis >> xml::start( "main" );
        std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
        pModel_.reset( new DEC_Model( "test", xis, BOOST_RESOLVE( "." ), missionTypes, false, BOOST_RESOLVE( "resources" ) ) );
        pType_.reset( new StubMIL_AutomateType( *pModel_ ) );
        pAutomat_.reset( new StubMIL_Automate( *pType_ ) );
    }
    MIL_EffectManager                     effectManager_;
    std::auto_ptr< DEC_Model >            pModel_;
    std::auto_ptr< StubMIL_AutomateType > pType_;
    std::auto_ptr< MIL_Automate >         pAutomat_;
};

#endif // __Fixture_h_
