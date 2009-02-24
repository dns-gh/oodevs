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
DEC_Model::DEC_Model( const DEC_Workspace& decWorkspace, const std::string& strName, xml::xistream& xis, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strArchivePath, const std::string& strPrefix, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes )
    : DEC_Model_ABC( decWorkspace, strName, xis, bNeedParsing, bUseOnlyArchive, strArchivePath, strPrefix, missionTypes )
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
