// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-08-05 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_ArchiveDirectoryHelper.h"
#include "MT/MT_IO/MT_Dir.h"

// -----------------------------------------------------------------------------
// Name: MT_ArchiveDirectoryHelper constructor
// Created: AGE 2005-08-05
// -----------------------------------------------------------------------------
MT_ArchiveDirectoryHelper::MT_ArchiveDirectoryHelper( const std::string& strNextArchiveFile )
    : strInitialDirectory_( MT_GetCurrentDir() )
{
    std::string strNewDir;
    MT_ExtractFilePath( strNextArchiveFile, strNewDir );
    MT_ChangeDir( strNewDir );
}

// -----------------------------------------------------------------------------
// Name: MT_ArchiveDirectoryHelper destructor
// Created: AGE 2005-08-05
// -----------------------------------------------------------------------------
MT_ArchiveDirectoryHelper::~MT_ArchiveDirectoryHelper()
{
    MT_ChangeDir( strInitialDirectory_ );
}
