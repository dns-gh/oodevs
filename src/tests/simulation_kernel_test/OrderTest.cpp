// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "protocol/Protocol.h"
#include "MockDEC_KnowledgeResolver_ABC.h"
#include "simulation_kernel/Entities/Orders/MIL_OrderContext.h"

namespace
{
    /* Context:
       - danger direction
       - phase lines
       - boundary limit 1
       - boundary limit 2
       - intelligences
    */
    const unsigned int contextSize = 5;
}

// -----------------------------------------------------------------------------
// Name: OrderTest_EmptyContextIsValid
// Created: SBO 2010-09-03
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( OrderTest_EmptyContextIsValid )
{
    MockDEC_KnowledgeResolver_ABC resolver;
    sword::MissionParameters parameters;
    for( unsigned int i = 0; i < contextSize; ++i )
        parameters.add_elem()->set_null_value( true );
    std::unique_ptr< MIL_OrderContext > context;
    BOOST_CHECK_NO_THROW( context.reset( new MIL_OrderContext( parameters, MT_Vector2D( 0, 0 ), resolver ) ) );
}
