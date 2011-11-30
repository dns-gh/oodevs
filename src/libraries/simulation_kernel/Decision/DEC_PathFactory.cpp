// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PathFactory.h"
#include "Decision/DEC_Agent_PathClass.h"
#include "Decision/DEC_Population_PathClass.h"
#include "MT_Tools/MT_ScipioException.h"

DEC_PathFactory* DEC_PathFactory::pInstance_ = 0;

// -----------------------------------------------------------------------------
// Name: DEC_PathFactory::Initialize
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
void DEC_PathFactory::Initialize( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects )
{
    if( !pInstance_ )
        pInstance_ = new DEC_PathFactory( xis, dangerousObjects );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFactory::Terminate
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
void DEC_PathFactory::Terminate()
{
    delete pInstance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFactory::GetInstance
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
const DEC_PathFactory& DEC_PathFactory::GetInstance()
{
    if( !pInstance_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "DEC_PathFactory singleton not initialized" );
    return *pInstance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFactory constructor
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
DEC_PathFactory::DEC_PathFactory( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects )
{
    DEC_Agent_PathClass::Initialize( xis, dangerousObjects );
    DEC_Population_PathClass::Initialize( xis, dangerousObjects );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFactory destructor
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
DEC_PathFactory::~DEC_PathFactory()
{
    DEC_Agent_PathClass::Terminate();
    DEC_Population_PathClass::Terminate();
}
