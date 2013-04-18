// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "StubMIL_AgentPion.h"
#include "MockRoleLocation.h"
#include "MockAgent.h"
#include "Fixture.h"

BOOST_AUTO_TEST_CASE( if_a_pion_is_transported_its_composantes_can_not_perceive )
{
    MIL_EffectManager effectManager;
    FixturePion fixture( effectManager );

    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetTickDuration ).returns( 10u );

    xml::xistringstream xisVolumes( "<volumes>"
                                    "    <volume name='Big'/>"
                                    "</volumes>" );
    PHY_Volume::Initialize( xisVolumes );

    xml::xistringstream xisProtection( "<protections>"
                                       "    <protection name='protection1' type='humain'>"
                                       "        <neutralization average-time='10s' variance='1s'/>"
                                       "    </protection>"
                                       "</protections>" );
    PHY_Protection::Initialize( xisProtection );

    xml::xistringstream xisComposanteTypePion( "<bla name='composanteTypePion' id='1' protection='protection1' weight='1' size='Big'>"
                                               "    <speeds max='8'/>"
                                               "    <weapon-systems/>"
                                               "    <sensors/>"
                                               "    <transports/>"
                                               "    <objects/>"
                                               "    <consumptions/>"
                                               "</bla>" );
    xisComposanteTypePion.start( "bla" );

    PHY_ComposanteTypePion type( time, "composanteTypePion", xisComposanteTypePion );
    PHY_RolePion_Composantes composantes( *fixture.pPion_, false );
    PHY_ComposantePion composante( time, type, composantes, 1, true, true, true );

    BOOST_CHECK( composante.CanPerceive( 0 ) );

    transport::PHY_RolePion_Transported* roleTransported = new transport::PHY_RolePion_Transported( *fixture.pPion_ );
    fixture.pPion_->RegisterRole( *roleTransported );

    BOOST_CHECK( composante.CanPerceive( 0 ) );

    MockAgent transporter;
    MockRoleLocation* locationRole = new MockRoleLocation();
    transporter.RegisterRole( *locationRole );
    const MT_Vector2D sourcePosition( 42, 42 );
    MOCK_EXPECT( locationRole, GetPosition ).once().returns( sourcePosition );

    bool bTransportedByAnother;
    roleTransported->LoadForTransport( transporter, true, bTransportedByAnother );

    BOOST_CHECK( !composante.CanPerceive( 0 ) );
    PHY_Protection::Terminate();
    PHY_Volume::Terminate();
}
