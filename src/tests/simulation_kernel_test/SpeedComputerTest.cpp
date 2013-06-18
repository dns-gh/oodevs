// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "Entities/Agents/Actions/Moving/SpeedComputerStrategy.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_InterfaceMoving.h"
#include "MoveComputerFactory.h"
#include "SpeedComputer_ABC.h"
#include "MockAgent.h"

namespace
{
    MOCK_BASE_CLASS( MockPHY_RoleAction_Moving, moving::PHY_RoleAction_InterfaceMoving )
    {
        MOCK_METHOD( Update, 1 )
        MOCK_METHOD( Clean, 0 )
        MOCK_METHOD( GetMaxSpeed, 0, double(), GetMaxSpeed1 )
        MOCK_METHOD( GetMaxSpeed, 1, double( const TerrainData& ), GetMaxSpeed2 )
        MOCK_METHOD( GetTheoricMaxSpeed, 0 )
        MOCK_METHOD( GetSpeed, 1, double( const TerrainData& ), GetSpeed1 )
        MOCK_METHOD( GetSpeed, 2, double( const TerrainData&, const MIL_Object_ABC& ), GetSpeed2 )
        MOCK_METHOD( GetTheoricSpeed, 1 )
        MOCK_METHOD( GetMaxSpeedModificator, 0 )
        MOCK_METHOD( GetMaxSlope, 0 )
        MOCK_METHOD( SetSpeedModificator, 1 )
        MOCK_METHOD( SetMaxSpeedModificator, 1 )
        MOCK_METHOD( ApplyTrafficModifier, 0 )
    };

    class ReentrantRole : private boost::noncopyable
    {
        public:
            explicit ReentrantRole( moving::MoveComputerFactory& factory )
                : factory_( factory )
            {
                // NOTHING
            }
            void Execute( moving::SpeedComputer_ABC& algorithm ) const
            {
                factory_.CreateSpeedComputer( moving::SpeedComputerStrategy( true ) );
                MockAgent agent;
                auto role = new MockPHY_RoleAction_Moving();
                MOCK_EXPECT( role->GetMaxSpeed1 ).returns( 0 );
                agent.RegisterRole( *role );
                algorithm.ApplyOnReinforcement( agent );
            }
        private:
            moving::MoveComputerFactory& factory_;
    };
}

BOOST_AUTO_TEST_CASE( DefaultSpeedComputerTest )
{
    moving::MoveComputerFactory factory;
    ReentrantRole role( factory );
    moving::SpeedComputerStrategy strategy( true );
    std::auto_ptr< moving::SpeedComputer_ABC > computer = factory.CreateSpeedComputer( strategy );
    role.Execute( *computer );
    BOOST_CHECK_EQUAL( 0., computer->GetSpeed() );
}
