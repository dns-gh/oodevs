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
class ReentrantRole
{
    public:
        explicit ReentrantRole( moving::MoveComputerFactory& moveComputerFactory ) : factory_( moveComputerFactory ) {}

        void Execute( moving::SpeedComputer_ABC& algorithm ) const
        {
            {
                moving::SpeedComputerStrategy strategy( true, false, 0 );
                // Must comment the following line until code is made reentrant:
                //factory_.CreateSpeedComputer( strategy );
            }
            MockAgent stubby;
            algorithm.ApplyOnReinforcement( stubby );
        }
    private:
        moving::MoveComputerFactory& factory_;
};

BOOST_AUTO_TEST_CASE( DefaultSpeedComputerTest )
{
    moving::MoveComputerFactory moveComputerFactory;
    ReentrantRole role( moveComputerFactory );
    moving::SpeedComputerStrategy strategy( true, false, 0 );
    moving::SpeedComputer_ABC& computer = moveComputerFactory.CreateSpeedComputer( strategy );
    role.Execute( computer );
    double rSpeed = computer.GetSpeed();
}