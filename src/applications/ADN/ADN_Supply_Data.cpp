// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Supply_Data.cpp $
// $Author: Ape $
// $Modtime: 25/04/05 18:18 $
// $Revision: 4 $
// $Workfile: ADN_Supply_Data.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Supply_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_DataException.h"

#include "ENT/ENT_Tr.h"


// -----------------------------------------------------------------------------
// Name: ConvoySetupInfo::ConvoySetupInfo
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Supply_Data::ConvoySetupInfo::ConvoySetupInfo()
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, nNbrTrucks_         ( 1 )
, rSetupTime_         ( 1.0 )
{
}


// -----------------------------------------------------------------------------
// Name: ConvoySetupInfo::GetNodeName
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
std::string ADN_Supply_Data::ConvoySetupInfo::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ConvoySetupInfo::GetItemName
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
std::string ADN_Supply_Data::ConvoySetupInfo::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ConvoySetupInfo::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::ConvoySetupInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Temps" );
    input.ReadAttribute( "nbCamions", nNbrTrucks_ );
//    std::string strTime;
//    input.Read( strTime );
    input.ReadTime( rSetupTime_ );
    input.EndSection(); // Temps
}


// -----------------------------------------------------------------------------
// Name: ConvoySetupInfo::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::ConvoySetupInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Temps" );
    output.WriteAttribute( "nbCamions", nNbrTrucks_.GetData() );
    output << ADN_Tools::SecondToString( rSetupTime_.GetData() );
    output.EndSection(); // Temps
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data constructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Supply_Data::ADN_Supply_Data()
: ADN_Data_ABC()
, ptrUnit_    ( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos(), 0, "ADN_Supply_Data::ptrUnit_" )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data destructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Supply_Data::~ADN_Supply_Data()
{
    vConvoySetupInfos_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::FilesNeeded
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szSupply_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::Reset
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::Reset()
{
    vConvoySetupInfos_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Ravitaillement" );
    input.Section( "Convois" );

    input.BeginList( "TempsConstitution" );
    while( input.NextListElement() )
    {
        std::auto_ptr<ConvoySetupInfo> spNew( new ConvoySetupInfo() );
        spNew->ReadArchive( input );
        vConvoySetupInfos_.AddItem( spNew.release() );
    }
    vConvoySetupInfos_.AddItem( 0 );  // Signals the end of the vector, allows certain parts of the gui to update.
    input.EndList(); // TempsConstitution 

    input.Section( "TypeUnite" );
    std::string strUnit;
    input.ReadAttribute( "nom", strUnit );
    ADN_Units_Data::UnitInfos* pUnit = ADN_Workspace::GetWorkspace().GetUnits().GetData().FindUnit( strUnit );
    if( pUnit == 0 )
        input.ThrowError( tr( "Unit '%1' does not exist." ).arg( strUnit.c_str() ).ascii() );
    ptrUnit_ = pUnit;
    input.EndSection(); // TypeUnite

    input.Section( "Mission" );
    input.ReadAttribute( "nom", nSupplyMission_, ENT_Tr::ConvertToUnitMission, ADN_XmlInput_Helper::eThrow );
    input.EndSection(); // Mission

    input.EndSection(); // Convois
    input.EndSection(); // Ravitaillement
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Ravitaillement" );
    output.Section( "Convois" );

    output.BeginList( "TempsConstitution", vConvoySetupInfos_.size() );
    for( IT_ConvoySetupInfoVector it = vConvoySetupInfos_.begin(); it != vConvoySetupInfos_.end(); ++it )
        (*it)->WriteArchive( output );
    output.EndList(); // TempsConstitution 

    if( ptrUnit_.GetData() == 0 )
        throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Convoy unit undefined." ).ascii() );

    if( ptrUnit_.GetData()->eTypeId_.GetData() != eAgentTypePionLOGConvoi )
        throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Convoy unit is not of type 'Pion LOG Convoi'." ).ascii() );


    output.Section( "TypeUnite" );
    output.WriteAttribute( "nom", ptrUnit_.GetData()->strName_.GetData() );
    output.EndSection(); // TypeUnite

    output.Section( "Mission" );
    output.WriteAttribute( "nom", ENT_Tr::ConvertFromUnitMission( nSupplyMission_.GetData() ) );
    output.EndSection(); // Mission

    output.EndSection(); // Convois
    output.EndSection(); // Ravitaillement
}
