// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Supply_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_DataException.h"

#include "ENT/ENT_Tr.h"


// -----------------------------------------------------------------------------
// Name: ConvoyInfo::ConvoyInfo
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
template< typename T >
ADN_Supply_Data::ConvoyInfo< T >::ConvoyInfo( typename T::BaseType value )
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, nNbrTrucks_         ( 1 )
, value_              ( value )
{
}

// -----------------------------------------------------------------------------
// Name: ConvoyInfo::GetNodeName
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
template< typename T >
std::string ADN_Supply_Data::ConvoyInfo< T >::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ConvoyInfo::GetItemName
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
template< typename T >
std::string ADN_Supply_Data::ConvoyInfo< T >::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ConvoyInfo::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Supply_Data::ConvoyInfo< T >::ReadArchive( const std::string& strSection, ADN_XmlInput_Helper& input )
{
    input.Section( strSection );
    input.ReadAttribute( "nbCamions", nNbrTrucks_ );
    T::BaseType tmp;
    input.Read( tmp );
    value_ = tmp;
    input.EndSection(); // strSection
}

// -----------------------------------------------------------------------------
// Name: ConvoyInfo::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Supply_Data::ConvoyInfo< T >::WriteArchive( const std::string& strSection, MT_OutputArchive_ABC& output )
{
    output.Section( strSection );
    output.WriteAttribute( "nbCamions", nNbrTrucks_.GetData() );
    output << value_.GetData();
    output.EndSection(); // strSection
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data constructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Supply_Data::ADN_Supply_Data()
: ADN_Data_ABC()
, ptrUnit_    ( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos(), 0, "ADN_Supply_Data::ptrUnit_" )
, ptrSupplyMission_( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissions(), 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data destructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Supply_Data::~ADN_Supply_Data()
{
    vConvoySetupInfos_    .Reset();
    vConvoyLoadingInfos_  .Reset();
    vConvoyUnloadingInfos_.Reset();
    vConvoySpeedModificatorInfos_.Reset();
    vVectorWarnings_      .Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::FilesNeeded
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSupply_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::Reset
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::Reset()
{
    vConvoySetupInfos_    .Reset();
    vConvoyLoadingInfos_  .Reset();
    vConvoyUnloadingInfos_.Reset();
    vConvoySpeedModificatorInfos_.Reset();
    vVectorWarnings_      .Reset();
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
        std::auto_ptr< ConvoyInfo< ADN_Type_Time > > spNew( new ConvoyInfo< ADN_Type_Time >( "1s" ) );
        spNew->ReadArchive( "Temps", input );
        vConvoySetupInfos_.AddItem( spNew.release() );
    }
    vConvoySetupInfos_.AddItem( 0 );  // Signals the end of the vector, allows certain parts of the gui to update.
    input.EndList(); // TempsConstitution 

    input.BeginList( "TempsChargement" );
    while( input.NextListElement() )
    {
        std::auto_ptr< ConvoyInfo< ADN_Type_Time > > spNew( new ConvoyInfo< ADN_Type_Time >( "1s" ) );
        spNew->ReadArchive( "Temps", input );
        vConvoyLoadingInfos_.AddItem( spNew.release() );
    }
    vConvoyLoadingInfos_.AddItem( 0 );
    input.EndList(); // TempsChargement 

    input.BeginList( "TempsDechargement" );
    while( input.NextListElement() )
    {
        std::auto_ptr< ConvoyInfo< ADN_Type_Time > > spNew( new ConvoyInfo< ADN_Type_Time >( "1s" ) );
        spNew->ReadArchive( "Temps", input );
        vConvoyUnloadingInfos_.AddItem( spNew.release() );
    }
    vConvoyUnloadingInfos_.AddItem( 0 );
    input.EndList(); // TempsDechargement

    input.BeginList( "CoefModificationVitesse" );
    while( input.NextListElement() )
    {
        std::auto_ptr< ConvoyInfo< ADN_Type_Double > > spNew( new ConvoyInfo< ADN_Type_Double >( 1 ) );
        spNew->ReadArchive( "Coef", input );
        vConvoySpeedModificatorInfos_.AddItem( spNew.release() );
    }
    vConvoySpeedModificatorInfos_.AddItem( 0 );
    input.EndList(); // CoefModificationVitesse

    input.Section( "TypeUnite" );
    std::string strUnit;
    input.ReadAttribute( "nom", strUnit );
    ADN_Units_Data::UnitInfos* pUnit = ADN_Workspace::GetWorkspace().GetUnits().GetData().FindUnit( strUnit );
    if( pUnit == 0 )
        input.ThrowError( tr( "Unit '%1' does not exist." ).arg( strUnit.c_str() ).ascii() );
    ptrUnit_ = pUnit;
    input.EndSection(); // TypeUnite

    input.Section( "Mission" );
    std::string supplyMission;
    input.ReadAttribute( "nom", supplyMission );
    ADN_Missions_Data::T_Mission_Vector& missions = ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissions();
    ADN_Missions_Data::Mission* mission = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindMission( missions, supplyMission );
    if( mission == 0 )
        input.ThrowError( tr( "Mission '%1' does not exist." ).arg( supplyMission.c_str() ).ascii() );
    ptrSupplyMission_ = mission;
    input.EndSection(); // Mission

    input.EndSection(); // Convois

    input.Section( "AlertesDisponibiliteMoyens" );
        input.BeginList( "AlertesDisponibiliteVecteurs" );
        while( input.NextListElement() )
        {
            std::auto_ptr< ADN_AvailabilityWarning > pNew( new ADN_AvailabilityWarning() );
            pNew->ReadArchive( input, "AlerteDisponibiliteVecteurs" );
            vVectorWarnings_.AddItem( pNew.release() );
        }
        vVectorWarnings_.AddItem( 0 );
        input.EndList();
    input.EndSection();

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

    {
        output.BeginList( "TempsConstitution", vConvoySetupInfos_.size() );
        for( IT_ConvoyTimeInfoVector it = vConvoySetupInfos_.begin(); it != vConvoySetupInfos_.end(); ++it )
            (*it)->WriteArchive( "Temps", output );
        output.EndList(); // TempsConstitution
    }
    {
        output.BeginList( "TempsChargement", vConvoyLoadingInfos_.size() );
        for( IT_ConvoyTimeInfoVector it = vConvoyLoadingInfos_.begin(); it != vConvoyLoadingInfos_.end(); ++it )
            (*it)->WriteArchive( "Temps", output );
        output.EndList(); // TempsChargement
    }
    {
        output.BeginList( "TempsDechargement", vConvoyUnloadingInfos_.size() );
        for( IT_ConvoyTimeInfoVector it = vConvoyUnloadingInfos_.begin(); it != vConvoyUnloadingInfos_.end(); ++it )
            (*it)->WriteArchive( "Temps", output );
        output.EndList(); // TempsDechargement
    }
    {
        output.BeginList( "CoefModificationVitesse", vConvoySpeedModificatorInfos_.size() );
        for( IT_ConvoyDoubleInfoVector it = vConvoySpeedModificatorInfos_.begin(); it != vConvoySpeedModificatorInfos_.end(); ++it )
            (*it)->WriteArchive( "Coef", output );
        output.EndList(); // CoefModificationVitesse
    }

    if( ptrUnit_.GetData() == 0 )
        throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Convoy unit undefined." ).ascii() );

    if( ptrUnit_.GetData()->eTypeId_.GetData() != eAgentTypePionLOGConvoi )
        throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Convoy unit is not of type 'Pion LOG Convoi'." ).ascii() );


    output.Section( "TypeUnite" );
    output.WriteAttribute( "nom", ptrUnit_.GetData()->strName_.GetData() );
    output.EndSection(); // TypeUnite

    output.Section( "Mission" );
    output.WriteAttribute( "nom", ptrSupplyMission_.GetData()->strName_.GetData() );
    output.EndSection(); // Mission

    output.EndSection(); // Convois

    output.Section( "AlertesDisponibiliteMoyens" );
        output.BeginList( "AlertesDisponibiliteVecteurs", vVectorWarnings_.size() );
        for( IT_AvailabilityWarning_Vector it = vVectorWarnings_.begin(); it != vVectorWarnings_.end(); ++it )
            (*it)->WriteArchive( output, "AlerteDisponibiliteVecteurs" );
        output.EndList();
    output.EndSection();

    output.EndSection(); // Ravitaillement
}
