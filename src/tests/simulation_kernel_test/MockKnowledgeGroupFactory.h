// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockKnowledgeGroupFactory_h_
#define __MockKnowledgeGroupFactory_h_

#include "simulation_kernel/Knowledge/KnowledgeGroupFactory_ABC.h"

// =============================================================================
/** @class  MockKnowledgeGroupFactory
@brief  MockKnowledgeGroupFactory
*/
// Created: SLG 2009-11-25
// =============================================================================
class MockKnowledgeGroupFactory
    : public mockpp::ChainableMockObject
    , public KnowledgeGroupFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockKnowledgeGroupFactory() 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockKnowledgeGroupFactory" ), 0 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CreateFormationShadow )       
    {
    }
    virtual ~MockKnowledgeGroupFactory() {}
    //@}

    virtual MIL_KnowledgeGroup& Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent = 0  )
    {
        return *CreateFormationShadow();
    }

    MOCKPP_CONST_CHAINABLE_EXT0( MockKnowledgeGroupFactory, MIL_KnowledgeGroup*, CreateFormationShadow, MIL_KnowledgeGroup, );
};

#endif // __MockKnowledgeGroupFactory_h_
