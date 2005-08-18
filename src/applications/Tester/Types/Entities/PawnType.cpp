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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PawnType.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 7 $
// $Workfile: PawnType.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"

#include "PawnType.h"
#include "PawnModel.h"
#include "Types/TypeManager.h"

using namespace TEST;


// -----------------------------------------------------------------------------
// Name: PawnType constructor
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
PawnType::PawnType( const TypeManager& typeManager, const std::string& strName, XmlInputArchive& archive )
    : strName_ ( strName )
    , pModel_  ( 0 )
{
    try   
    {
        std::string strValue;

        archive.ReadField( "MosID", nId_ );
        std::string strModel;
        archive.ReadField( "ModeleDecisionnel", strModel );
        pModel_ = typeManager.FindPawnModel( strModel );
        assert( pModel_ );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( "Parse error " << exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PawnType destructor
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
PawnType::~PawnType()
{
    // NOTHING
}
