// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Disasters_Data.h"
#include "ADN_Project_Data.h"

//-----------------------------------------------------------------------------
// Name: DisasterInfos::DisasterInfos
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_Data::DisasterInfos::DisasterInfos()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DisasterInfos::DisasterInfos
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_Data::DisasterInfos::~DisasterInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterInfos::ReadArchive
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::DisasterInfos::ReadArchive( xml::xistream& /*input*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterInfos::WriteArchive
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::DisasterInfos::WriteArchive( xml::xostream& /*output*/ )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_Data constructor
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_Data::ADN_Disasters_Data()
    : ADN_Data_ABC()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_Data destructor
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_Data::~ADN_Disasters_Data()
{
    Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::FilesNeeded
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
void ADN_Disasters_Data::FilesNeeded( T_StringList& /*files*/ ) const
{
//    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDisasters_.GetData() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::Reset
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
void ADN_Disasters_Data::Reset()
{
    vDisasters_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::ReadArchive
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::ReadArchive( xml::xistream& /*input*/ )
{

}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::WriteArchive
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::WriteArchive( xml::xostream& /*output*/ )
{

}
