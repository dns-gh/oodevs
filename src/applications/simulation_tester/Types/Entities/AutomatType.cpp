// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/AutomatType.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 2 $
// $Workfile: AutomatType.cpp $
//
// *****************************************************************************

#include "Tester_pch.h"
#include "AutomatType.h"
#include "Types/TypeManager.h"
#include "AutomatModel.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: AutomatType constructor
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
AutomatType::AutomatType( const TypeManager& typeManager, const std::string& strName, XmlInputArchive& archive )
    : strName_ ( strName )
{
    try   
    {
        std::string strValue;

        archive.ReadField    ( "MosID", nId_ );

        archive.Section      ( "Automate" );
        std::string strModel;
        archive.ReadField( "ModeleDecisionnel", strModel );
        pModel_ = typeManager.FindAutomatModel( strModel );
        assert( pModel_ );
        archive.EndSection   (); // Automate

        archive.Section      ( "PionPC" );
        archive.ReadAttribute( "type", strValue );
        archive.EndSection   (); // PionPC
        pPcPawnType_ = typeManager.FindPawnType( strValue );
        assert( pPcPawnType_ );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( "Parse error " << exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatType destructor
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
AutomatType::~AutomatType()
{
    // NOTHING
}
