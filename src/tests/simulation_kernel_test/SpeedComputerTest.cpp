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
#include "MoveComputerFactory.h"
#include "SpeedComputer_ABC.h"
#include "MockAgent.h"

// -----------------------------------------------------------------------------
// Name: DefaultSpeedComputerTest
// Created: LDC 2009-12-16
// -----------------------------------------------------------------------------
namespace
{
    class ReentrantRole : private boost::noncopyable
    {
        public:
            explicit ReentrantRole( moving::MoveComputerFactory& moveComputerFactory )
                : factory_( moveComputerFactory )
            {
                // NOTHING
            }
            void Execute( moving::SpeedComputer_ABC& algorithm ) const
            {
                factory_.CreateSpeedComputer( moving::SpeedComputerStrategy( true, false ) );
                MockAgent stubby;
                algorithm.ApplyOnReinforcement( stubby );
            }
        private:
            moving::MoveComputerFactory& factory_;
    };
}

BOOST_AUTO_TEST_CASE( DefaultSpeedComputerTest )
{
    moving::MoveComputerFactory moveComputerFactory;
    ReentrantRole role( moveComputerFactory );
    moving::SpeedComputerStrategy strategy( true, false );
    std::auto_ptr< moving::SpeedComputer_ABC > computer = moveComputerFactory.CreateSpeedComputer( strategy );
    role.Execute( *computer );
    BOOST_CHECK_EQUAL( 0., computer->GetSpeed() );
}
