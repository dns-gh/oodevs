// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Model.h"

// -----------------------------------------------------------------------------
// Name: DEC_Model constructor
// Created: LDC 2009-02-24
// -----------------------------------------------------------------------------
DEC_Model::DEC_Model( const std::string& strName, xml::xistream& xis, const std::string& strSourcePath, const std::map< std::string, const MIL_MissionType_ABC* >& missionTypes, bool isMasalife )
    : DEC_Model_ABC( strName, xis, strSourcePath, missionTypes, isMasalife )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: DEC_Model destructor
// Created: LDC 2009-02-24
// -----------------------------------------------------------------------------
DEC_Model::~DEC_Model()
{
    // NOTHING
}
