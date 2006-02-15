// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationKnowledges.h"
#include "PopulationKnowledge.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
PopulationKnowledges::PopulationKnowledges( Controller& controller )
    : controller_( controller )
{
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
PopulationKnowledges::~PopulationKnowledges()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::UpdatePopulation
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void PopulationKnowledges::UpdatePopulation( const T& message )
{
    Get( message.oid_connaissance_population ).Update( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const ASN1T_MsgPopulationKnowledgeCreation& message )
{
    if( ! Find( message.oid_connaissance ) )
    {
        PopulationKnowledge* knowledge = new PopulationKnowledge( controller_, message );
        Register( message.oid_connaissance, *knowledge );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const ASN1T_MsgPopulationKnowledgeUpdate& message )
{
    Get( message.oid_connaissance ).Update( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const ASN1T_MsgPopulationKnowledgeDestruction& message )
{
    delete Find( message.oid_connaissance );
    Remove( message.oid_connaissance );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& message )
{
    UpdatePopulation( message );
}   

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const ASN1T_MsgPopulationFluxKnowledgeCreation& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const ASN1T_MsgPopulationFluxKnowledgeUpdate& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const ASN1T_MsgPopulationFluxKnowledgeDestruction& message )
{
    UpdatePopulation( message );
}
