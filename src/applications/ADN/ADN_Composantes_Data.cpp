//*****************************************************************************
//
// $Created: JDY 03-07-17 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_Data.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 13:48 $
// $Revision: 26 $
// $Workfile: ADN_Composantes_Data.cpp $
//
//*****************************************************************************
#include "ADN_pch.h"
#include "ADN_Composantes_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
#include "ADN_Tr.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_DataException.h"

#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::AmbulanceInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Composantes_Data::AmbulanceInfos::AmbulanceInfos()
: ADN_DataTreeNode_ABC  ()
, rCapacity_            ( 0 )
, loadTimePerPerson_    ( "0s" )
, unloadTimePerPerson_  ( "0s" )
{
}


// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::GetNodeName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::AmbulanceInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::GetItemName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::AmbulanceInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::AmbulanceInfos::CopyFrom( AmbulanceInfos& src )
{
    rCapacity_ = src.rCapacity_.GetData();
    loadTimePerPerson_ = src.loadTimePerPerson_.GetData();
    unloadTimePerPerson_ = src.unloadTimePerPerson_.GetData();
}


// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::AmbulanceInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadField( "Capacite", rCapacity_ );
    input.ReadField( "TempsChargementParHumain", loadTimePerPerson_ );
    input.ReadField( "TempsDechargementParHumain", unloadTimePerPerson_ );
}


// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::AmbulanceInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.WriteField( "Capacite", rCapacity_.GetData() );
    output.WriteField( "TempsChargementParHumain", loadTimePerPerson_.GetData() );
    output.WriteField( "TempsDechargementParHumain", unloadTimePerPerson_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: LogHealthInfos::LogHealthInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Composantes_Data::LogHealthInfos::LogHealthInfos()
: ADN_DataTreeNode_ABC  ()
, bIsAmbulance_         ( false )
, bIsAmbulanceReleve_   ( false )
, bIsDoctor_            ( false )
, bIsSortingDoctor_     ( false )
, bIsCuringDoctor_      ( false )
, bCuresNBC_            ( false )
, bCuresShock_          ( false )
{
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        doctorSkills_[n] = false;
}


// -----------------------------------------------------------------------------
// Name: LogHealthInfos::GetNodeName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::LogHealthInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: LogHealthInfos::GetItemName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::LogHealthInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: LogHealthInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogHealthInfos::CopyFrom( LogHealthInfos& src )
{
    bIsAmbulance_ = src.bIsAmbulance_.GetData();
    bIsAmbulanceReleve_ = src.bIsAmbulanceReleve_.GetData();
    bIsDoctor_ = src.bIsDoctor_.GetData();
    bIsSortingDoctor_ = src.bIsSortingDoctor_.GetData();
    bIsCuringDoctor_ = src.bIsCuringDoctor_.GetData();
    bCuresNBC_ = src.bCuresNBC_.GetData();
    bCuresShock_ = src.bCuresShock_.GetData();
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        doctorSkills_[n] = src.doctorSkills_[n].GetData();

    ambulanceInfos_.CopyFrom( src.ambulanceInfos_ );
    ambulanceReleveInfos_.CopyFrom( src.ambulanceReleveInfos_ );
}


// -----------------------------------------------------------------------------
// Name: LogHealthInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogHealthInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    if( input.Section( "AmbulanceRamassage", ADN_XmlInput_Helper::eNothing ) )
    {
        bIsAmbulance_ = true;
        ambulanceInfos_.ReadArchive( input );
        input.EndSection(); // AmbulanceRamassage
    }
    if( input.Section( "AmbulanceReleve", ADN_XmlInput_Helper::eNothing ) )
    {
        bIsAmbulanceReleve_ = true;
        ambulanceReleveInfos_.ReadArchive( input );
        input.EndSection(); // AmbulanceReleve
    }

    if( input.Section( "Medecin", ADN_XmlInput_Helper::eNothing ) )
    {
        bIsDoctor_ = true;
        if( input.Section( "Tri", ADN_XmlInput_Helper::eNothing ) )
        {
            bIsSortingDoctor_ = true;
            input.EndSection(); // Tri
        }
        if( input.Section( "SoinBlessures", ADN_XmlInput_Helper::eNothing ) )
        {
            bIsCuringDoctor_ = true;
            for( int n = 0; n < eNbrDoctorSkills; ++n )
            {
                if( input.Section( ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n ), ADN_XmlInput_Helper::eNothing ) )
                {
                    doctorSkills_[n] = true;
                    input.EndSection();
                }
            }
            input.EndSection(); // SoinBlessures
        }
        if( input.Section( "SoinNBC", ADN_XmlInput_Helper::eNothing ) )
        {
            bCuresNBC_ = true;
            input.EndSection(); // SoinNBC
        }
        if( input.Section( "SoinReacMental", ADN_XmlInput_Helper::eNothing ) )
        {
            bCuresShock_ = true;
            input.EndSection(); // SoinReacMental
        }

        input.EndSection(); // Medecin
    }
}


// -----------------------------------------------------------------------------
// Name: LogHealthInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogHealthInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    if( bIsAmbulance_.GetData() )
    {
        output.Section( "AmbulanceRamassage" );
        ambulanceInfos_.WriteArchive( output );
        output.EndSection(); // AmbulanceRamassage
    }

    if( bIsAmbulanceReleve_.GetData() )
    {
        output.Section( "AmbulanceReleve" );
        ambulanceReleveInfos_.WriteArchive( output );
        output.EndSection(); // AmbulanceReleve
    }

    if( bIsDoctor_.GetData() )
    {
        output.Section( "Medecin" );
        if( bIsSortingDoctor_.GetData() )
        {
            output.Section( "Tri" );
            output.EndSection(); // Tri
        }
        if( bIsCuringDoctor_.GetData() )
        {
            output.Section( "SoinBlessures" );
            for( int n = 0; n < eNbrDoctorSkills; ++n )
            {
                if( doctorSkills_[n].GetData() )
                {
                    output.Section( ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n ) );
                    output.EndSection();
                }
            }
            output.EndSection(); // SoinBlessures
        }
        if( bCuresNBC_.GetData() )
        {
            output.Section( "SoinNBC" );
            output.EndSection(); // SoinNBC
        }
        if( bCuresShock_.GetData() )
        {
            output.Section( "SoinReacMental" );
            output.EndSection(); // SoinReacMental
        }

        output.EndSection(); // Medecin
    }
}


// -----------------------------------------------------------------------------
// Name: NTIInfos::NTIInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Composantes_Data::NTIInfos::NTIInfos( const std::string& strName )
: ADN_DataTreeNode_ABC  ()
, strName_              ( strName )
, bIsPresent_           ( false )
, bHasMaxRepairTime_    ( false )
, maxRepairTime_        ( "0s" )
, bCanRepairEA_         ( false )
, bCanRepairM_          ( false )
{
}


// -----------------------------------------------------------------------------
// Name: NTIInfos::GetNodeName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::NTIInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: NTIInfos::GetItemName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::NTIInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: NTIInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::NTIInfos::CopyFrom( NTIInfos& src )
{
    bIsPresent_ = src.bIsPresent_.GetData();
    bHasMaxRepairTime_ = src.bHasMaxRepairTime_.GetData();
    maxRepairTime_ = src.maxRepairTime_.GetData();
    bCanRepairEA_ = src.bCanRepairEA_.GetData();
    bCanRepairM_ = src.bCanRepairM_.GetData();
}


// -----------------------------------------------------------------------------
// Name: NTIInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::NTIInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    if( ! input.Section( strName_, ADN_XmlInput_Helper::eNothing ) )
        return;

    bIsPresent_ = true;

    if( input.ReadAttribute( "tempsMaxReparation", maxRepairTime_, ADN_XmlInput_Helper::eNothing ) )
        bHasMaxRepairTime_ = true;

    if( input.Section( "EA", ADN_XmlInput_Helper::eNothing ) )
    {
        bCanRepairEA_ = true;
        input.EndSection(); // EA
    }

    if( input.Section( "M", ADN_XmlInput_Helper::eNothing ) )
    {
        bCanRepairM_ = true;
        input.EndSection(); // M
    }
    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: NTIInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::NTIInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    if( ! bIsPresent_.GetData() )
        return;

    output.Section( strName_ );

    if( bHasMaxRepairTime_.GetData() )
        output.WriteAttribute( "tempsMaxReparation", maxRepairTime_.GetData() );

    if( bCanRepairEA_.GetData() )
    {
        output.Section( "EA" ),
            output.EndSection(); // EA
    }

    if( bCanRepairM_.GetData() )
    {
        output.Section( "M" );
        output.EndSection(); // M
    }
    output.EndSection();
}


// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::LogMaintenanceInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Composantes_Data::LogMaintenanceInfos::LogMaintenanceInfos()
: ADN_DataTreeNode_ABC()
, bIsTower_           ( false )
, rCapacity_          ( 0 )
, loadTimePerTon_     ( "0s" )
, unloadTimePerTon_   ( "0s" )
, NTI1Infos_          ( "NTI1" )
, NTI2Infos_          ( "NTI2" )
, NTI3Infos_          ( "NTI3" )
{
}

//
// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::GetNodeName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::LogMaintenanceInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::GetItemName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::LogMaintenanceInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogMaintenanceInfos::CopyFrom( LogMaintenanceInfos& src )
{
    bIsTower_ = src.bIsTower_.GetData();
    rCapacity_ = src.rCapacity_.GetData();
    loadTimePerTon_ = src.loadTimePerTon_.GetData();
    unloadTimePerTon_ = src.unloadTimePerTon_.GetData();

    NTI1Infos_.CopyFrom( src.NTI1Infos_ );
    NTI2Infos_.CopyFrom( src.NTI2Infos_ );
    NTI3Infos_.CopyFrom( src.NTI3Infos_ );
}


// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogMaintenanceInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    if( input.Section( "Remorqueur", ADN_XmlInput_Helper::eNothing ) )
    {
        bIsTower_ = true;
        input.ReadField( "Capacite", rCapacity_ );
        input.ReadField( "TempsChargementParTonne", loadTimePerTon_ );
        input.ReadField( "TempsDechargementParTonne", unloadTimePerTon_ );
        input.EndSection(); // Remorqueur
    }
    NTI1Infos_.ReadArchive( input );
    NTI2Infos_.ReadArchive( input );
    NTI3Infos_.ReadArchive( input );
}


// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogMaintenanceInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    if( bIsTower_.GetData() )
    {
        output.Section( "Remorqueur" );
        output.WriteField( "Capacite", rCapacity_.GetData() );
        output.WriteField( "TempsChargementParTonne", loadTimePerTon_.GetData() );
        output.WriteField( "TempsDechargementParTonne", unloadTimePerTon_.GetData() );
        output.EndSection();
    }
    NTI1Infos_.WriteArchive( output );
    NTI2Infos_.WriteArchive( output );
    NTI3Infos_.WriteArchive( output );
}


// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::LogSupplyInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Composantes_Data::LogSupplyInfos::LogSupplyInfos()
: ADN_DataTreeNode_ABC()
, bIsCarrier_         ( false )
, rWeight_            ( 0 )
, rVolume_            ( 0 )
, loadTime_           ( "0s" )
, unloadTime_         ( "0s" )
, bIsConvoyLeader_    ( false )
, bIsConveyor_        ( false )
{
}


// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::GetNodeName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::LogSupplyInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::GetItemName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::LogSupplyInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogSupplyInfos::CopyFrom( LogSupplyInfos& src )
{
    bIsCarrier_  = src.bIsCarrier_.GetData();
    rWeight_     = src.rWeight_.GetData();
    rVolume_     = src.rVolume_.GetData();
    loadTime_   = src.loadTime_.GetData();
    unloadTime_ = src.unloadTime_.GetData();
    bIsConvoyLeader_ = src.bIsConvoyLeader_.GetData();
    bIsConveyor_  = src.bIsConveyor_.GetData();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogSupplyInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    if( input.Section( "Transporteur", ADN_XmlInput_Helper::eNothing ) )
    {
        bIsCarrier_ = true;

        input.Section( "Capacite" );
        input.ReadField( "Masse", rWeight_ );
        input.ReadField( "Volume", rVolume_ );
        input.EndSection(); // Capacite
        input.ReadField( "TempsChargementMoyen", loadTime_ );
        input.ReadField( "TempsDechargementMoyen", unloadTime_ );
        input.EndSection(); // Transporteur
    }

    if( input.Section( "ChefDeConvoi", ADN_XmlInput_Helper::eNothing ) )
    {
        bIsConvoyLeader_ = true;
        input.EndSection(); // ChefDeConvoi
    }
    if( input.Section( "Convoyeur", ADN_XmlInput_Helper::eNothing ) )
    {
        bIsConveyor_ = true;
        input.EndSection(); // Convoyeur
    }
}


// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogSupplyInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    if( bIsCarrier_.GetData() )
    {
        output.Section( "Transporteur" );
        output.Section( "Capacite" );
        output.WriteField( "Masse" , rWeight_.GetData() );
        output.WriteField( "Volume", rVolume_.GetData() );
        output.EndSection();
        output.WriteField( "TempsChargementMoyen", loadTime_.GetData() );
        output.WriteField( "TempsDechargementMoyen", unloadTime_.GetData() );
        output.EndSection(); // Transporteur
    }

    if( bIsConvoyLeader_.GetData() )
    {
        output.Section( "ChefDeConvoi" );
        output.EndSection(); // ChefDeConvoi
    }

    if( bIsConveyor_.GetData() )
    {
        output.Section( "Convoyeur" );
        output.EndSection(); // Convoyeur
    }
}


// -----------------------------------------------------------------------------
// Name: LogInfos::LogInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Composantes_Data::LogInfos::LogInfos()
: ADN_DataTreeNode_ABC ()
, bHasHealthInfos_     ( false )
, bHasMaintenanceInfos_( false )
, bHasSupplyInfos_     ( false )
{
}


// -----------------------------------------------------------------------------
// Name: LogInfos::GetNodeName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::LogInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: LogInfos::GetItemName
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::LogInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: LogInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogInfos::CopyFrom( LogInfos& src )
{
    bHasHealthInfos_ = src.bHasHealthInfos_.GetData();
    bHasMaintenanceInfos_ = src.bHasMaintenanceInfos_.GetData();
    bHasSupplyInfos_ = src.bHasSupplyInfos_.GetData();

    healthInfos_.CopyFrom( src.healthInfos_ );
    maintenanceInfos_.CopyFrom( src.maintenanceInfos_ );
    supplyInfos_.CopyFrom( src.supplyInfos_ );
}


// -----------------------------------------------------------------------------
// Name: LogInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    if( ! input.Section( "FonctionsLogistiques", ADN_XmlInput_Helper::eNothing ) )
        return;

    if( input.Section( "Sante", ADN_XmlInput_Helper::eNothing ) )
    {
        bHasHealthInfos_ = true;
        healthInfos_.ReadArchive( input );
        input.EndSection(); // Sante
    }

    if( input.Section( "Maintenance", ADN_XmlInput_Helper::eNothing ) )
    {
        bHasMaintenanceInfos_ = true;
        maintenanceInfos_.ReadArchive( input );
        input.EndSection(); // Maintenance
    }

    if( input.Section( "Ravitaillement", ADN_XmlInput_Helper::eNothing ) )
    {
        bHasSupplyInfos_ = true;
        supplyInfos_.ReadArchive( input );
        input.EndSection(); // Ravitaillement
    }

    input.EndSection(); // FonctionsLogistiques
}


// -----------------------------------------------------------------------------
// Name: LogInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    if( ! bHasHealthInfos_.GetData() && ! bHasMaintenanceInfos_.GetData() && ! bHasSupplyInfos_.GetData() )
        return;

    output.Section( "FonctionsLogistiques" );

    if( bHasHealthInfos_.GetData() )
    {
        output.Section( "Sante" );
        healthInfos_.WriteArchive( output );
        output.EndSection(); // Sante
    }

    if( bHasMaintenanceInfos_.GetData() )
    {
        output.Section( "Maintenance" );
        maintenanceInfos_.WriteArchive( output );
        output.EndSection(); // Maintenance
    }

    if( bHasSupplyInfos_.GetData() )
    {
        output.Section( "Ravitaillement" );
        supplyInfos_.WriteArchive( output );
        output.EndSection(); // Ravitaillement
    }

    output.EndSection(); // FonctionsLogistiques
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfos::BreakdownInfos
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Composantes_Data::BreakdownInfos::BreakdownInfos()
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, rPercentage_        ( 0 )
{
    BindExistenceTo( &ptrBreakdown_ );
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfos::GetNodeName
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::BreakdownInfos::GetNodeName()
{
    return std::string( "" );
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfos::GetItemName
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::BreakdownInfos::GetItemName()
{
    return std::string( "" );
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfos::CreateCopy
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Composantes_Data::BreakdownInfos* ADN_Composantes_Data::BreakdownInfos::CreateCopy()
{
    BreakdownInfos* pCopy = new BreakdownInfos();
    pCopy->ptrBreakdown_ = ptrBreakdown_.GetData();
    pCopy->ptrBreakdown_.SetVector( ptrBreakdown_.GetVector() );
    pCopy->rPercentage_ = rPercentage_.GetData();
    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfos::ReadArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::BreakdownInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Panne" );
    std::string strName;
    input.ReadAttribute( "nom", strName );

    ADN_Breakdowns_Data::BreakdownInfo* pBreakdown = ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().FindBreakdown( strName );
    if( pBreakdown == 0 )
        input.ThrowError( tr( "Breakdown %1 does not exist." ).arg( strName.c_str() ).ascii() );

    ptrBreakdown_ = pBreakdown;
    ptrBreakdown_.SetVector( ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().vBreakdowns_ );
    //ptrBreakdown_.SetVector( pBreakdown->nti_.vBreakdowns_ );

    input.ReadAttribute( "pourcentage", rPercentage_ );
    input.EndSection(); // Panne
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfos::WriteArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::BreakdownInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Panne" );
    output.WriteAttribute( "nom", ptrBreakdown_.GetData()->strName_.GetData() );
    output.WriteAttribute( "pourcentage", rPercentage_.GetData() );
    output.EndSection(); // Panne
}


// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::BreakdownGroupInfos
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Composantes_Data::BreakdownGroupInfos::BreakdownGroupInfos( const std::string& strName )
: strName_       ( strName )
{
}

// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::~BreakdownGroupInfos
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Composantes_Data::BreakdownGroupInfos::~BreakdownGroupInfos()
{
    vBreakdowns_.Reset();
}

// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::GetNodeName
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::BreakdownGroupInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::GetItemName
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::BreakdownGroupInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::CopyFrom
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::BreakdownGroupInfos::CopyFrom( BreakdownGroupInfos& src )
{
    assert( strName_ == src.strName_ );
    vBreakdowns_.Reset();
    for( IT_BreakdownInfos_Vector it = src.vBreakdowns_.begin(); it != src.vBreakdowns_.end(); ++it )
        vBreakdowns_.AddItem( (*it)->CreateCopy() );
}


// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::ReadArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::BreakdownGroupInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( strName_ );
    while( input.NextListElement() )
    {
        std::auto_ptr<BreakdownInfos> spNew( new BreakdownInfos() );
        spNew->ReadArchive( input );
        vBreakdowns_.AddItem( spNew.release() );
    }
    input.EndList();
}


// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::WriteArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::BreakdownGroupInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    double rSum = 0.0;
    for( IT_BreakdownInfos_Vector it = vBreakdowns_.begin(); it != vBreakdowns_.end() ; ++it )
        rSum += (*it)->rPercentage_.GetData();

    if( rSum != 100.0 )
        throw ADN_DataException( tr( "Data error" ).ascii(), tr( "The sum of breakdown odds for composante x is not equal to 100." ).ascii() );

    output.BeginList( strName_, vBreakdowns_.size() );
    for( IT_BreakdownInfos_Vector it = vBreakdowns_.begin(); it != vBreakdowns_.end() ; ++it )
        (*it)->WriteArchive( output );
    output.EndList();
}


//-----------------------------------------------------------------------------
// Name: SpeedInfos::SpeedInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Composantes_Data::SpeedInfos::SpeedInfos( E_Location nTypeTerrain )
: nTypeTerrain_(nTypeTerrain)
, rSpeed_(0.0)
{
    rSpeed_.SetDataName( "la vitesse" );
    rSpeed_.SetParentNode( *this );
}


// -----------------------------------------------------------------------------
// Name: SpeedInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::SpeedInfos::GetNodeName()
{
    std::string strResult( "sur terrain de type " );
    return strResult + ADN_Tr::ConvertFromLocation( nTypeTerrain_, ENT_Tr_ABC::eToTr );
}


// -----------------------------------------------------------------------------
// Name: SpeedInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::SpeedInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: SpeedInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::SpeedInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    double rSpeed;
    input.Read( rSpeed );
    rSpeed_ = rSpeed;
}


// -----------------------------------------------------------------------------
// Name: SpeedInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::SpeedInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Terrain" );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromLocation( nTypeTerrain_ ) );
    output << rSpeed_.GetData();
    output.EndSection(); // Terrain
}


//-----------------------------------------------------------------------------
// Name: SensorInfos::SensorInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Composantes_Data::SensorInfos::SensorInfos()
:   ADN_Ref_ABC()
,   ptrSensor_(ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsInfos(),0)
,   rHeight_(0)
{
    BindExistenceTo(&ptrSensor_);

    rHeight_.SetDataName( "la hauteur" );
    rHeight_.SetParentNode( *this );
}


// -----------------------------------------------------------------------------
// Name: SensorInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::SensorInfos::GetNodeName()
{
    std::string strResult( "du senseur " );
    return strResult + ptrSensor_.GetData()->strName_.GetData().c_str();
}


// -----------------------------------------------------------------------------
// Name: SensorInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::SensorInfos::GetItemName()
{
    return ptrSensor_.GetData()->strName_.GetData().c_str();
}


// -----------------------------------------------------------------------------
// Name: SensorInfos::CreateCopy
// Created: APE 2004-11-29
// -----------------------------------------------------------------------------
ADN_Composantes_Data::SensorInfos* ADN_Composantes_Data::SensorInfos::CreateCopy()
{
    SensorInfos* pCopy = new SensorInfos();
    pCopy->ptrSensor_ = ptrSensor_.GetData();
    pCopy->rHeight_ = rHeight_.GetData();
    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: SensorInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::SensorInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Senseur" );
    input.ReadAttribute( "hauteur", rHeight_ );
    std::string strName;
    input.Read( strName );
    ADN_Sensors_Data::SensorInfos* pSensor = ADN_Workspace::GetWorkspace().GetSensors().GetData().FindSensor( strName );
    assert( pSensor != 0 );
    ptrSensor_ = pSensor;
    input.EndSection(); // Senseur
}


// -----------------------------------------------------------------------------
// Name: SensorInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::SensorInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Senseur" );
    output.WriteAttribute( "hauteur", rHeight_.GetData() );
    output << ptrSensor_.GetData()->strName_.GetData();
    output.EndSection(); // Senseur
}


// -----------------------------------------------------------------------------
// Name: RadarInfos::RadarInfos
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Composantes_Data::RadarInfos::RadarInfos()
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, ptrRadar_( ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_.vRadars_, 0 )
{
    BindExistenceTo( &ptrRadar_ );
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::GetNodeName
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::RadarInfos::GetNodeName()
{
    return std::string( "" );
}


// -----------------------------------------------------------------------------
// Name: RadarInfos::GetItemName
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::RadarInfos::GetItemName()
{
    return std::string( "" );
}


// -----------------------------------------------------------------------------
// Name: RadarInfos::CreateCopy
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Composantes_Data::RadarInfos* ADN_Composantes_Data::RadarInfos::CreateCopy()
{
    RadarInfos* pCopy = new RadarInfos();
    pCopy->ptrRadar_ = ptrRadar_.GetData();
    pCopy->strName_ = strName_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::ReadArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::RadarInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Radar" ); 
    std::string strRadarName;
    input.Read( strRadarName );

    ADN_Radars_Data::RadarInfos* pRadar = ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_.FindRadar( strRadarName );
    if( pRadar == 0 )
        input.ThrowError( tr( "Radar %1 does not exist." ).arg( strRadarName.c_str() ).ascii() );

    ptrRadar_ = pRadar;
    strName_ = strRadarName;

    input.EndSection(); // Radar
}


// -----------------------------------------------------------------------------
// Name: RadarInfos::WriteArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::RadarInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Radar" );
    output << ptrRadar_.GetData()->strName_.GetData();
    output.EndSection(); //Radar
}




//-----------------------------------------------------------------------------
// Name: WeaponInfos::WeaponInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Composantes_Data::WeaponInfos::WeaponInfos()
: ADN_Ref_ABC()
, ptrWeapon_ ( ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos(), 0 )
{
    BindExistenceTo(&ptrWeapon_);
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::WeaponInfos
// Created: APE 2005-01-24
// -----------------------------------------------------------------------------
ADN_Composantes_Data::WeaponInfos::WeaponInfos( ADN_Weapons_Data::WeaponInfos& weapon )
: ADN_Ref_ABC()
, ptrWeapon_ ( ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos(), &weapon, "" )
, strName_   ( weapon.strName_.GetData() )
{
    BindExistenceTo(&ptrWeapon_);
}



// -----------------------------------------------------------------------------
// Name: WeaponInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::WeaponInfos::GetNodeName()
{
    std::string strResult( "de l'arme " );
    return strResult + ptrWeapon_.GetData()->GetNodeName();
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::WeaponInfos::GetItemName()
{
    return ((ADN_Weapons_Data::WeaponInfos*)ptrWeapon_.GetData())->ptrLauncher_.GetData()->strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::CreateCopy
// Created: APE 2004-11-29
// -----------------------------------------------------------------------------
ADN_Composantes_Data::WeaponInfos* ADN_Composantes_Data::WeaponInfos::CreateCopy()
{
    WeaponInfos* pCopy = new WeaponInfos();
    pCopy->ptrWeapon_ = ptrWeapon_.GetData();
    pCopy->strName_ = strName_.GetData();
    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::WeaponInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Armement" );

    std::string strLauncher;
    std::string strAmmunition;
    input.ReadAttribute( "lanceur", strLauncher );
    input.ReadAttribute( "munition", strAmmunition );
    ADN_Weapons_Data::WeaponInfos* pWeapon = ADN_Workspace::GetWorkspace().GetWeapons().GetData().FindWeapon( strLauncher, strAmmunition );
    assert( pWeapon != 0 );
    ptrWeapon_ = pWeapon;
    strName_ = pWeapon->strName_.GetData();

    input.EndSection(); // Armement
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::WeaponInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Armement" );

    output.WriteAttribute( "lanceur", ptrWeapon_.GetData()->ptrLauncher_.GetData()->strName_.GetData() );
    output.WriteAttribute( "munition", ptrWeapon_.GetData()->ptrAmmunition_.GetData()->strName_.GetData() );

    output.EndSection();
}


// -----------------------------------------------------------------------------
// Name: CategoryInfos::CategoryInfos
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Composantes_Data::CategoryInfos::CategoryInfos( ADN_Equipement_Data::DotationInfos& parentDotation )
: ADN_Ref_ABC( "ADN_Composantes_Data::CategoryInfos" )
, ptrDotation_( &parentDotation, "" )
, ptrCategory_( parentDotation.categories_, 0, "ptr cat ds ADN_Composantes_Data::CategoryInfos" )
, rNormalizedConsumption_( 0 )
, rLogThreshold_( 0 )
, rNbr_         ( 0 )

{
    //    BindExistenceTo( &ptrDotation_ );
    BindExistenceTo( &ptrCategory_ );
}


// -----------------------------------------------------------------------------
// Name: CategoryInfos::GetNodeName
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::CategoryInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: CategoryInfos::GetItemName
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::CategoryInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: CategoryInfos::CreateCopy
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Composantes_Data::CategoryInfos* ADN_Composantes_Data::CategoryInfos::CreateCopy()
{
    CategoryInfos* pCopy = new CategoryInfos( * ptrDotation_.GetData() );
    pCopy->ptrCategory_ = ptrCategory_.GetData();
    pCopy->ptrDotation_ = ptrDotation_.GetData();
    pCopy->rNormalizedConsumption_ = rNormalizedConsumption_.GetData();
    pCopy->rNbr_ = rNbr_.GetData();
    pCopy->rLogThreshold_ = rLogThreshold_.GetData();
    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: CategoryInfos::ReadArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::CategoryInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Categorie" );

    std::string strCategory;
    input.ReadAttribute( "nom", strCategory );
    ADN_Equipement_Data::CategoryInfo* pCat = ptrDotation_.GetData()->FindCategory( strCategory );
    assert( pCat != 0 );
    ptrCategory_ = pCat;

    input.ReadAttribute( "contenance", rNbr_ );
    input.ReadAttribute( "seuilLogistique", rLogThreshold_ );
    input.ReadAttribute( "consommationNormalisee", rNormalizedConsumption_, ADN_XmlInput_Helper::eNothing );

    input.EndSection(); // Categorie
}


// -----------------------------------------------------------------------------
// Name: CategoryInfos::WriteArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::CategoryInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Categorie" );
    output.WriteAttribute( "nom", ptrCategory_.GetData()->strName_.GetData() );
    output.WriteAttribute( "contenance", rNbr_.GetData() );
    output.WriteAttribute( "seuilLogistique", rLogThreshold_.GetData() );
    output.WriteAttribute( "consommationNormalisee", rNormalizedConsumption_.GetData() );
    output.EndSection(); // Categorie
}


//-----------------------------------------------------------------------------
// Name: DotationInfos::DotationInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Composantes_Data::DotationInfos::DotationInfos()
: ADN_Ref_ABC()
, categories_()
{
}


// -----------------------------------------------------------------------------
// Name: DotationInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::DotationInfos::GetNodeName()
{
    /*
    std::string strResult( ENT_Tr::ConvertFromTypeDotation( eTypeDotation_.GetData(), ENT_Tr_ABC::eToTr ) );
    if( eTypeDotation_.GetData() == eTypeDotation_Munition )
    strResult += " " + ptrMunition_.GetData()->strName_.GetData();
    return strResult;*/
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: DotationInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::DotationInfos::GetItemName()
{
    return GetNodeName();
}


// -----------------------------------------------------------------------------
// Name: DotationInfos::CreateCopy
// Created: APE 2004-11-29
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::DotationInfos::CopyFrom( ADN_Composantes_Data::DotationInfos& src )
{
    for( IT_CategoryInfos_Vector it = src.categories_.begin(); it != src.categories_.end(); ++it )
        categories_.AddItem( (*it)->CreateCopy() );
}


// -----------------------------------------------------------------------------
// Name: DotationInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
bool ADN_Composantes_Data::DotationInfos::ReadArchive( const std::string& strListName, ADN_XmlInput_Helper& input, bool bOptionnal )
{
    if( ! input.BeginList( strListName, bOptionnal ? ADN_XmlInput_Helper::eNothing : ADN_XmlInput_Helper::eThrow ) )
        return false;

    while( input.NextListElement() )
    {
        input.Section( "Dotation" );

        // Read the dotation name and find the corresponding equipement dotation.
        std::string strDotation;
        input.ReadAttribute( "nom", strDotation );
        E_FamilleDotation nTypeDotation = ENT_Tr::ConvertToFamilleDotation( strDotation );
        assert( nTypeDotation != -1 );

        ADN_Equipement_Data::DotationInfos* pDotation = & ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( nTypeDotation );

        // Read the categories.
        input.BeginList( "Categories" );
        while( input.NextListElement() )
        {
            std::auto_ptr<CategoryInfos> spNew( new CategoryInfos( *pDotation ) );
            spNew->ReadArchive( input );
            categories_.AddItem( spNew.release() );
        }
        input.EndList(); // Categories
        input.EndSection(); // Dotation
    }

    input.EndList(); // Contenance
    return true;
}


// -----------------------------------------------------------------------------
// Name: DotationInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::DotationInfos::WriteArchive( const std::string& strListName, MT_OutputArchive_ABC& output )
{
    if( categories_.empty() )
        return;

    output.BeginList( strListName, eNbrFamilleDotation );
    for( uint n = 0; n < eNbrFamilleDotation; ++n )
    {
        output.Section( "Dotation" );
        ADN_Equipement_Data::DotationInfos& dotation = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( (E_FamilleDotation)n );
        output.WriteAttribute( "nom", dotation.strName_.GetData() );
        output.BeginList( "Categories", categories_.size() );
        for( IT_CategoryInfos_Vector it = categories_.begin(); it != categories_.end(); ++it )
        {
            if( (*it)->ptrDotation_.GetData() == &dotation )
                (*it)->WriteArchive( output );
        }
        output.EndList(); // Categories
        output.EndSection(); // Dotation
    }
    output.EndList(); // Contenance
}


//-----------------------------------------------------------------------------
// Name: ObjectInfos::ObjectInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Composantes_Data::ObjectInfos::ObjectInfos()
: ADN_Ref_ABC()
, ptrObject_(ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(),0)
, initialBuildTime_         ( "0s" )
, initialDestructionTime_   ( "0s" )
, coeffBuildTime_           ( "0s" )
, coeffDestructionTime_     ( "0s" )
, valorizationTime_         ( "0s" )
, devalorizationTime_       ( "0s" )
, rCoeffCirc_               ( 0 )
, rSpeedCirc_               ( 0 )
, rSpeedNotCirc_            ( 0 )
, bInitialBuildTime_        ( false )
, bInitialDestructionTime_  ( false )
, bCoeffBuildTime_          ( false )
, bCoeffDestructionTime_    ( false )
, bValorizationTime_        ( false )
, bDevalorizationTime_      ( false )
, bCoeffCircTime_           ( false )
, bSpeedCirc_               ( false )
, bSpeedNotCirc_            ( false )
{
    BindExistenceTo(&ptrObject_);

    initialBuildTime_.SetDataName( "le temps initial de construction" );
    initialBuildTime_.SetParentNode( *this );
    initialDestructionTime_.SetDataName( "le temps initial de destruction" );
    initialDestructionTime_.SetParentNode( *this );
    coeffBuildTime_.SetDataName( "le coefficient de construction" );
    coeffBuildTime_.SetParentNode( *this );
    coeffDestructionTime_.SetDataName( "le coefficient de destruction" );
    coeffDestructionTime_.SetParentNode( *this );
    valorizationTime_.SetDataName( "le temps de valorisation" );
    valorizationTime_.SetParentNode( *this );
    devalorizationTime_.SetDataName( "le temps de dévalorisation" );
    devalorizationTime_.SetParentNode( *this );
    rCoeffCirc_.SetDataName( "le temps de contournement" );
    rCoeffCirc_.SetParentNode( *this );
    rSpeedCirc_.SetDataName( "la vitesse d'une unité si contourné" );
    rSpeedCirc_.SetParentNode( *this );
    rSpeedNotCirc_.SetDataName( "la vitesse d'une unité si non-contourné" );
    rSpeedNotCirc_.SetParentNode( *this );
}


// -----------------------------------------------------------------------------
// Name: ObjectInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ObjectInfos::GetNodeName()
{
    std::string strResult( "des objets de type " );
    return strResult + ptrObject_.GetData()->strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: ObjectInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ObjectInfos::GetItemName()
{
    return ptrObject_.GetData()->strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: ObjectInfos::CreateCopy
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Composantes_Data::ObjectInfos* ADN_Composantes_Data::ObjectInfos::CreateCopy()
{
    ObjectInfos* pCopy = new ObjectInfos();

    pCopy->ptrObject_ = ptrObject_.GetData();
    pCopy->initialBuildTime_ = initialBuildTime_.GetData();
    pCopy->initialDestructionTime_ = initialDestructionTime_.GetData();
    pCopy->coeffBuildTime_ = coeffBuildTime_.GetData();
    pCopy->coeffDestructionTime_ = coeffDestructionTime_.GetData();
    pCopy->valorizationTime_ = valorizationTime_.GetData();
    pCopy->devalorizationTime_ = devalorizationTime_.GetData();
    pCopy->rCoeffCirc_ = rCoeffCirc_.GetData();
    pCopy->rSpeedCirc_ = rSpeedCirc_.GetData();
    pCopy->rSpeedNotCirc_ = rSpeedNotCirc_.GetData();

    pCopy->bInitialBuildTime_ = bInitialBuildTime_.GetData();
    pCopy->bInitialDestructionTime_ = bInitialDestructionTime_.GetData();
    pCopy->bCoeffBuildTime_ = bCoeffBuildTime_.GetData();
    pCopy->bCoeffDestructionTime_ = bCoeffDestructionTime_.GetData();
    pCopy->bValorizationTime_ = bValorizationTime_.GetData();
    pCopy->bDevalorizationTime_ = bDevalorizationTime_.GetData();
    pCopy->bCoeffCircTime_ = bCoeffCircTime_.GetData();
    pCopy->bSpeedCirc_ = bSpeedCirc_.GetData();
    pCopy->bSpeedNotCirc_ = bSpeedNotCirc_.GetData();

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: ObjectInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ObjectInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Objet" );

    std::string strName;
    input.ReadAttribute( "type", strName );
    ADN_Objects_Data::ObjectInfos* pObject = ADN_Workspace::GetWorkspace().GetObjects().GetData().FindObject( strName );

    if( !pObject )
        throw ADN_Xml_Exception( input.GetContext(), "Type d'objet invalide" );

    ptrObject_ = pObject;

    bInitialBuildTime_       = input.ReadField( "TempsInitialConstruction", initialBuildTime_, ADN_XmlInput_Helper::eNothing );
    bInitialDestructionTime_ = input.ReadField( "TempsInitialDestruction", initialDestructionTime_, ADN_XmlInput_Helper::eNothing );
    bCoeffBuildTime_         = input.ReadField( "TempsConstructionCoef", coeffBuildTime_, ADN_XmlInput_Helper::eNothing );
    bCoeffDestructionTime_   = input.ReadField( "TempsDestructionCoef", coeffDestructionTime_, ADN_XmlInput_Helper::eNothing );
    bValorizationTime_       = input.ReadField( "TempsValorisation", valorizationTime_, ADN_XmlInput_Helper::eNothing );
    bDevalorizationTime_     = input.ReadField( "TempsDevalorisation", devalorizationTime_, ADN_XmlInput_Helper::eNothing );
    bCoeffCircTime_          = input.ReadField( "CoefGainContournement", rCoeffCirc_, ADN_XmlInput_Helper::eNothing );
    bSpeedCirc_              = input.ReadField( "VitesseNonContourne", rSpeedCirc_, ADN_XmlInput_Helper::eNothing );
    bSpeedNotCirc_           = input.ReadField( "VitesseContourne", rSpeedNotCirc_, ADN_XmlInput_Helper::eNothing );

    input.EndSection(); // Objet
}


// -----------------------------------------------------------------------------
// Name: ObjectInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ObjectInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Objet" );
    output.WriteAttribute( "type", ptrObject_.GetData()->strName_.GetData() );

    if( bInitialBuildTime_.GetData() )
        output.WriteField( "TempsInitialConstruction", initialBuildTime_.GetData() );
    if( bInitialDestructionTime_.GetData() )
        output.WriteField( "TempsInitialDestruction", initialDestructionTime_.GetData() );
    if( bCoeffBuildTime_.GetData() )
        output.WriteField( "TempsConstructionCoef", coeffBuildTime_.GetData() );
    if( bCoeffDestructionTime_.GetData() )
        output.WriteField( "TempsDestructionCoef", coeffDestructionTime_.GetData() );
    if( bValorizationTime_.GetData() )
        output.WriteField( "TempsValorisation", valorizationTime_.GetData() );
    if( bDevalorizationTime_.GetData() )
        output.WriteField( "TempsDevalorisation", devalorizationTime_.GetData() );
    if( bCoeffCircTime_.GetData() )
        output.WriteField( "CoefGainContournement", rCoeffCirc_.GetData() );
    if( bSpeedCirc_.GetData() )
        output.WriteField( "VitesseNonContourne", rSpeedCirc_.GetData() );
    if( bSpeedNotCirc_.GetData() )
        output.WriteField( "VitesseContourne", rSpeedNotCirc_.GetData() );

    output.EndSection(); // Objet
}


// -----------------------------------------------------------------------------
// Name: ConsumptionItem::ConsumptionItem
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
ADN_Composantes_Data::ConsumptionItem::ConsumptionItem( E_ConsumptionType nConsumptionType, ADN_Equipement_Data::CategoryInfo& category )
: nConsumptionType_    ( nConsumptionType )
, ptrCategory_         ( category.parentDotation_.categories_, &category, "" )
, nQuantityUsedPerHour_( 0 )
{
    this->BindExistenceTo( &ptrCategory_ );
}


// -----------------------------------------------------------------------------
// Name: ConsumptionItem::GetNodeName
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ConsumptionItem::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ConsumptionItem::GetItemName
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ConsumptionItem::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ConsumptionItem::CreateCopy
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Composantes_Data::ConsumptionItem* ADN_Composantes_Data::ConsumptionItem::CreateCopy()
{
    ConsumptionItem* pCopy = new ConsumptionItem( nConsumptionType_, *ptrCategory_.GetData() );
    pCopy->nQuantityUsedPerHour_ = nQuantityUsedPerHour_.GetData();
    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: ConsumptionItem::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ConsumptionItem::ReadArchive( ADN_XmlInput_Helper& input )
{
    double nUsed;
    input.Read( nUsed );
    nQuantityUsedPerHour_ = nUsed;
}


// -----------------------------------------------------------------------------
// Name: ConsumptionItem::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ConsumptionItem::WriteArchive( MT_OutputArchive_ABC& output )
{
    output << nQuantityUsedPerHour_.GetData();
}


// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::ConsumptionsInfos
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
ADN_Composantes_Data::ConsumptionsInfos::ConsumptionsInfos()
{
}


// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::GetNodeName
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ConsumptionsInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::GetItemName
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ConsumptionsInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::CopyFrom
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ConsumptionsInfos::CopyFrom( ConsumptionsInfos& source )
{
    for( IT_ConsumptionItem_Vector it = source.vConsumptions_.begin(); it != source.vConsumptions_.end(); ++it )
        vConsumptions_.AddItem( (*it)->CreateCopy() );
}

// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::ReadArchive
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ConsumptionsInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Consommations" );

    for( int nType = 0; nType < eNbrConsumptionType; ++nType )
    {
        input.BeginList( ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)nType ) );

        while( input.NextListElement() )
        {
            input.Section( "Dotation" );
            std::string strDotationName;
            input.ReadAttribute( "nom", strDotationName );

            input.BeginList( "Categories" );
            while( input.NextListElement() )
            {
                input.Section( "Categorie" );
                std::string strCategoryName;
                input.ReadAttribute( "nom", strCategoryName );

                ADN_Equipement_Data::CategoryInfo* pCategory = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( strDotationName, strCategoryName );
                if( !pCategory )
                    throw ADN_Xml_Exception( input.GetContext(), "Type de dotation invalide" );

                std::auto_ptr<ConsumptionItem> spNew( new ConsumptionItem( (E_ConsumptionType)nType, *pCategory ) );
                spNew->ReadArchive( input );
                vConsumptions_.AddItem( spNew.release() );

                input.EndSection(); // Categorie
            }
            input.EndList(); // Categories
            input.EndSection(); // Dotation
        }

        input.EndList(); // ArretMoteur & cie
    }

    input.EndSection(); // Consommations
}


// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::WriteArchive
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ConsumptionsInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Consommations" );

    for( int nType = 0; nType < eNbrConsumptionType; ++nType )
    {
        std::vector<ConsumptionItem*> vConsumptionTypeItems;
        for( IT_ConsumptionItem_Vector it = vConsumptions_.begin(); it != vConsumptions_.end(); ++it )
        {
            if( (*it)->nConsumptionType_ == nType )
                vConsumptionTypeItems.push_back( *it );
        }

        output.Section( ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)nType ) );

        for( int nDotationType = 0; nDotationType < eNbrFamilleDotation; ++nDotationType )
        {
            std::vector<ConsumptionItem*> vDotationItems;
            for( std::vector<ConsumptionItem*>::iterator it2 = vConsumptionTypeItems.begin(); it2 != vConsumptionTypeItems.end(); ++it2 )
            {
                if( (*it2)->ptrCategory_.GetData()->parentDotation_.nType_ == nDotationType )
                    vDotationItems.push_back( *it2 );
            }

            if( vDotationItems.empty() )
                continue;

            output.Section( "Dotation" );
            output.WriteAttribute( "nom", ENT_Tr::ConvertFromFamilleDotation( (E_FamilleDotation)nDotationType ) );
            output.BeginList( "Categories", vDotationItems.size() );
            for( std::vector<ConsumptionItem*>::iterator it3 = vDotationItems.begin(); it3 != vDotationItems.end(); ++it3 )
            {
                output.Section( "Categorie" );
                output.WriteAttribute( "nom", (*it3)->ptrCategory_.GetData()->strName_.GetData() );
                output << (*it3)->nQuantityUsedPerHour_.GetData();
                output.EndSection(); // Categorie
            }
            output.EndList(); // Categories
            output.EndSection(); // Dotation
        }

        output.EndSection(); // ConsumptionType
    }
    output.EndSection(); // Consumptions
}


//=============================================================================
//
//=============================================================================

//-----------------------------------------------------------------------------
// Name: ComposanteInfos::ComposanteInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Composantes_Data::ComposanteInfos::ComposanteInfos()
: ADN_Ref_ABC()
, strName_()
, strAdditionalComments_()
, nMosId_( ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetNextId() )
, ptrArmor_(ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos(),0)
, ptrSize_(ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos(),0)
, eDeviceCategory_((E_DeviceCategory)0)
, rWeight_(0)
, vSpeeds_( false )
, vWeapons_()
, vSensors_()
, vRadars_ ()
, vObjects_()
, nTroopTransportCapacity_(0)
, bTroopEmbarkingTimes_( false )
, embarkingTimePerPerson_( "0s" )
, disembarkingTimePerPerson_( "0s" )
, bCanCarryCargo_( false )
, rWeightTransportCapacity_( 0 )
, embarkingTimePerTon_( "0s" )
, disembarkingTimePerTon_( "0s" )
, rMaxSpeed_( 0 )
, attritionBreakdowns_( "PannesAttritions" )
, randomBreakdowns_   ( "PannesAleatoires" )
, bMaxSlope_( false )
, rMaxSlope_( 60 )
{
    strName_.SetDataName( "le nom" );
    strName_.SetParentNode( *this );

    ptrArmor_.SetNodeName( "le niveau de blindage" );
    ptrArmor_.SetParentNode( *this );
    ptrSize_.SetNodeName( "le volume" );
    ptrSize_.SetParentNode( *this );

    ADN_Type_Enum<E_DeviceCategory, eNbrDeviceCategory>::SetConverter( &ADN_Tr::ConvertFromDeviceCategory );
    eDeviceCategory_.SetDataName( "la catégorie de matériel" );
    eDeviceCategory_.SetParentNode( *this );

    vSpeeds_.SetParentNode( *this );

    vSensors_.SetParentNode( *this );
    vSensors_.SetItemTypeName( "un senseur" );

    vWeapons_.SetParentNode( *this );
    vWeapons_.SetItemTypeName( "un lanceur" );

    vObjects_.SetParentNode( *this );
    vObjects_.SetItemTypeName( "un objet" );

    // initialize speeds
    for( int iTerrain=0; iTerrain < eNbrLocation; ++iTerrain )
    {
        SpeedInfos * pNewSpeedInfos = new SpeedInfos( (E_Location)iTerrain );
        vSpeeds_.AddItem(pNewSpeedInfos);
    }
}



//-----------------------------------------------------------------------------
// Name: ComposanteInfos::ComposanteInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Composantes_Data::ComposanteInfos::~ComposanteInfos()
{
    vSpeeds_.Reset();
    vWeapons_.Reset();
    vSensors_.Reset();
    vRadars_.Reset();
    vObjects_.Reset();

    for( int iTerrain=0; iTerrain < eNbrLocation; ++iTerrain )
    {
        SpeedInfos * pNewSpeedInfos = new SpeedInfos( (E_Location)iTerrain );
        vSpeeds_.AddItem(pNewSpeedInfos);
    }
}


// -----------------------------------------------------------------------------
// Name: ComposanteInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ComposanteInfos::GetNodeName()
{
    std::string strResult( "de l'équipement " );
    return strResult + strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: ComposanteInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ComposanteInfos::GetItemName()
{
    return strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: ComposanteInfos::CreateCopy
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Composantes_Data::ComposanteInfos* ADN_Composantes_Data::ComposanteInfos::CreateCopy()
{
    ADN_Composantes_Data::ComposanteInfos* pCopy = new ADN_Composantes_Data::ComposanteInfos();

    pCopy->strAdditionalComments_ = strAdditionalComments_.GetData();
    pCopy->ptrArmor_ = ptrArmor_.GetData();
    pCopy->ptrSize_ = ptrSize_.GetData();
    pCopy->rWeight_ = rWeight_.GetData();
    pCopy->rMaxSpeed_ = rMaxSpeed_.GetData();
    pCopy->eDeviceCategory_ = eDeviceCategory_.GetData();

    for( int iTerrain = 0; iTerrain < eNbrLocation; ++iTerrain )
        pCopy->vSpeeds_[ iTerrain ]->rSpeed_ = vSpeeds_[ iTerrain ]->rSpeed_.GetData();


    for( IT_WeaponInfos_Vector itWeapon = vWeapons_.begin(); itWeapon != vWeapons_.end(); ++itWeapon )
    {
        WeaponInfos* pNew = (*itWeapon)->CreateCopy();
        pCopy->vWeapons_.AddItem( pNew );
    }

    for( IT_SensorInfos_Vector itSensor = vSensors_.begin(); itSensor != vSensors_.end(); ++itSensor )
    {
        SensorInfos* pNew = (*itSensor)->CreateCopy();
        pCopy->vSensors_.AddItem( pNew );
    }

    for( IT_RadarInfos_Vector itRadar = vRadars_.begin(); itRadar != vRadars_.end(); ++itRadar )
    {
        RadarInfos* pNew = (*itRadar)->CreateCopy();
        pCopy->vRadars_.AddItem( pNew );
    }

    for( IT_ObjectInfos_Vector itObject = vObjects_.begin(); itObject != vObjects_.end(); ++itObject )
    {
        ObjectInfos* pNew = (*itObject)->CreateCopy();
        pCopy->vObjects_.AddItem( pNew );
    }

    pCopy->consumptions_.CopyFrom( consumptions_ );
    pCopy->dotations_.CopyFrom( dotations_ );

    pCopy->nTroopTransportCapacity_ = nTroopTransportCapacity_.GetData();
    pCopy->bTroopEmbarkingTimes_ = bTroopEmbarkingTimes_.GetData();
    pCopy->embarkingTimePerPerson_ = embarkingTimePerPerson_.GetData();
    pCopy->disembarkingTimePerPerson_ = disembarkingTimePerPerson_.GetData();
    pCopy->bCanCarryCargo_ = bCanCarryCargo_.GetData();
    pCopy->rWeightTransportCapacity_ = rWeightTransportCapacity_.GetData();
    pCopy->embarkingTimePerTon_ = embarkingTimePerTon_.GetData();
    pCopy->disembarkingTimePerTon_ = disembarkingTimePerTon_.GetData();

    pCopy->logInfos_.CopyFrom( logInfos_ );

    pCopy->attritionBreakdowns_.CopyFrom( attritionBreakdowns_ );
    pCopy->randomBreakdowns_.CopyFrom( randomBreakdowns_ );

    pCopy->bMaxSlope_ = bMaxSlope_.GetData();
    pCopy->rMaxSlope_ = rMaxSlope_.GetData();

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: ComposanteInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ComposanteInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Composante" );
    input.ReadAttribute( "Commentaire", strAdditionalComments_ );
    input.ReadAttribute( "nom", strName_ );

    strCodeEMAT6_ = strName_.GetData();
    strCodeEMAT8_ = strName_.GetData();
    strCodeLFRIL_ = strName_.GetData();
    strCodeNNO_ = strName_.GetData();

    input.ReadField( "CodeEMAT6", strCodeEMAT6_, ADN_XmlInput_Helper::eNothing );
    input.ReadField( "CodeEMAT8", strCodeEMAT8_, ADN_XmlInput_Helper::eNothing );
    input.ReadField( "CodeLFRIL", strCodeLFRIL_, ADN_XmlInput_Helper::eNothing );
    input.ReadField( "CodeNNO", strCodeNNO_    , ADN_XmlInput_Helper::eNothing );

    std::string strDeviceCategory;
    if( input.ReadField( "CategorieMateriel", strDeviceCategory, ADN_XmlInput_Helper::eNothing ) )
    {
        E_DeviceCategory eDeviceCategory = ADN_Tr::ConvertToDeviceCategory( strDeviceCategory );
        if( eDeviceCategory == (E_DeviceCategory)-1 )
            input.ThrowError( MT_FormatString( "La catégorie de matériel '%s' est invalide", strDeviceCategory.c_str() ) );
        eDeviceCategory_ = eDeviceCategory;
    }
    else
        eDeviceCategory_ = eDeviceCategory_Autres; // default value

    std::string strArmor;
    input.ReadField( "Protection", strArmor );
    ADN_Categories_Data::ArmorInfos* pArmor = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindArmor( strArmor );
    assert( pArmor != 0 );
    ptrArmor_ = pArmor;

    std::string strSize;
    input.ReadField( "Volume", strSize );
    ADN_Categories_Data::SizeInfos* pSize = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindSize( strSize );
    assert( pSize != 0 );
    ptrSize_ = pSize;

    input.BeginList( "Vitesses" );
    input.ReadAttribute( "maximum", rMaxSpeed_ );
    while( input.NextListElement() )
    {
        input.Section( "Terrain" );
        std::string strLocation;
        input.ReadAttribute( "type", strLocation );
        E_Location nLocation = ADN_Tr::ConvertToLocation( strLocation );
        vSpeeds_[nLocation]->ReadArchive( input );
        input.EndSection(); // Terrain
    }
    input.EndList(); // Vitesses

    dotations_.ReadArchive( "Contenance", input );

    input.BeginList( "Senseurs" );
    while( input.NextListElement() )
    {
        std::auto_ptr<SensorInfos> spNew( new SensorInfos() );
        spNew->ReadArchive( input );
        vSensors_.AddItem( spNew.release() );
    }
    input.EndList(); // Senseurs

    if( input.BeginList( "Radars", ADN_XmlInput_Helper::eNothing ) )
    {
        while( input.NextListElement() )
        {
            std::auto_ptr<RadarInfos> spNew( new RadarInfos() );
            spNew->ReadArchive( input );
            vRadars_.AddItem( spNew.release() );
        }
        input.EndList(); // Radars
    }

    input.Section( "Transport" );
    input.Section( "Personnel" );
    input.ReadField( "Equipage", nTroopTransportCapacity_ );
    if( input.Section( "Temps", ADN_XmlInput_Helper::eNothing ) )
    {
        bTroopEmbarkingTimes_ = true;
        input.ReadField( "TempsEmbarquementParHomme", embarkingTimePerPerson_ );
        input.ReadField( "TempsDebarquementParHomme", disembarkingTimePerPerson_ );
        input.EndSection(); // Temps
    }
    input.EndSection(); // Personnel

    if( input.Section( "Pion", ADN_XmlInput_Helper::eNothing ) )
    {
        bCanCarryCargo_ = true;
        input.ReadField( "Capacite", rWeightTransportCapacity_ );
        input.Section( "Temps" );
        input.ReadField( "TempsChargementParTonne", embarkingTimePerTon_ );
        input.ReadField( "TempsDechargementParTonne", disembarkingTimePerTon_ );
        input.EndSection(); // Temps
        input.EndSection(); // Pion
    }
    input.EndSection(); // Transport

    input.ReadField( "Poids", rWeight_ );

    consumptions_.ReadArchive( input );

    input.BeginList( "Armements" );
    while( input.NextListElement() )
    {
        std::auto_ptr<WeaponInfos> spNew( new WeaponInfos() );
        spNew->ReadArchive( input );
        vWeapons_.AddItem( spNew.release() );
    }
    input.EndList(); // Armements

    input.BeginList( "Objets" );
    while( input.NextListElement() )
    {
        std::auto_ptr<ObjectInfos> spNew( new ObjectInfos() );
        spNew->ReadArchive( input );
        vObjects_.AddItem( spNew.release() );
    }
    input.EndList(); // Objets

    logInfos_.ReadArchive( input );

    if( input.Section( "Pannes", ADN_XmlInput_Helper::eNothing ) )
    {
        randomBreakdowns_.ReadArchive( input );
        attritionBreakdowns_.ReadArchive( input );
        input.EndSection(); // Pannes
    }

    if( input.ReadField( "DeniveleMaximum", rMaxSlope_, ADN_XmlInput_Helper::eNothing ) )
    {
        bMaxSlope_ = true;
        double rTmp = rMaxSlope_.GetData() * 100.0;
        rMaxSlope_ = rTmp; // display percentage
    }

    input.EndSection(); // Composante
}


// -----------------------------------------------------------------------------
// Name: ComposanteInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ComposanteInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Composante" );
    output.WriteAttribute( "Commentaire", strAdditionalComments_.GetData() );
    output.WriteAttribute( "nom", strName_.GetData() );
    output.WriteField( "MosID", nMosId_.GetData() );
    output.WriteField( "CodeEMAT6", strCodeEMAT6_.GetData() );
    output.WriteField( "CodeEMAT8", strCodeEMAT8_.GetData() );
    output.WriteField( "CodeLFRIL", strCodeLFRIL_.GetData() );
    output.WriteField( "CodeNNO", strCodeNNO_.GetData() );
    output.WriteField( "CategorieMateriel", ADN_Tr::ConvertFromDeviceCategory( eDeviceCategory_.GetData() ) );

    output.WriteField( "Protection", ptrArmor_.GetData()->strName_.GetData() );
    output.WriteField( "Volume", ptrSize_.GetData()->GetData() );

    output.BeginList( "Vitesses", vSpeeds_.size() );
    output.WriteAttribute( "maximum", rMaxSpeed_.GetData() );
    for( IT_SpeedInfos_Vector itSpeed = vSpeeds_.begin(); itSpeed != vSpeeds_.end(); ++itSpeed )
        (*itSpeed)->WriteArchive( output );
    output.EndList(); // Vitesses

    dotations_.WriteArchive( "Contenance", output );

    output.BeginList( "Armements", vWeapons_.size() );
    for( IT_WeaponInfos_Vector itWeapon = vWeapons_.begin(); itWeapon != vWeapons_.end(); ++itWeapon )
        (*itWeapon)->WriteArchive( output );
    output.EndList(); // Armements

    output.Section( "Transport" );
    output.Section( "Personnel" );
    output.WriteField( "Equipage", nTroopTransportCapacity_.GetData() );
    if( bTroopEmbarkingTimes_.GetData() )
    {
        output.Section( "Temps" );
        output.WriteField( "TempsEmbarquementParHomme", embarkingTimePerPerson_.GetData() );
        output.WriteField( "TempsDebarquementParHomme", disembarkingTimePerPerson_.GetData() );
        output.EndSection(); // Temps
    }
    output.EndSection(); // Personnel

    if( bCanCarryCargo_.GetData() )
    {
        output.Section( "Pion" );
        output.WriteField( "Capacite", rWeightTransportCapacity_.GetData() );
        output.Section( "Temps" );
        output.WriteField( "TempsChargementParTonne", embarkingTimePerTon_.GetData() );
        output.WriteField( "TempsDechargementParTonne", disembarkingTimePerTon_.GetData() );
        output.EndSection(); // Temps
        output.EndSection(); // Pion
    }
    output.EndSection(); // Transport

    output.BeginList( "Senseurs", vSensors_.size() );
    for( IT_SensorInfos_Vector itSensor = vSensors_.begin(); itSensor != vSensors_.end(); ++itSensor )
        (*itSensor)->WriteArchive( output );
    output.EndList(); // Senseurs

    output.BeginList( "Radars", vRadars_.size() );
    for( IT_RadarInfos_Vector itRadar = vRadars_.begin(); itRadar != vRadars_.end(); ++itRadar )
        (*itRadar)->WriteArchive( output );
    output.EndList(); // Radars

    output.WriteField( "Poids", rWeight_.GetData() );

    consumptions_.WriteArchive( output );

    output.BeginList( "Objets", vObjects_.size() );
    for( IT_ObjectInfos_Vector itObject = vObjects_.begin(); itObject != vObjects_.end(); ++itObject )
        (*itObject)->WriteArchive( output );
    output.EndList(); // Objets

    logInfos_.WriteArchive( output );

    if( ptrArmor_.GetData()->nType_ == eProtectionType_Material )
    {
        output.Section( "Pannes" );
        randomBreakdowns_.WriteArchive( output );
        attritionBreakdowns_.WriteArchive( output );
        output.EndSection(); // Pannes
    }

    if( bMaxSlope_.GetData() )
        output.WriteField( "DeniveleMaximum", rMaxSlope_.GetData() / 100.0 );

    output.EndSection(); // Composante
}

//=============================================================================
//
//=============================================================================


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Data constructor
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
ADN_Composantes_Data::ADN_Composantes_Data()
: ADN_Data_ABC ()
, nNextId_     ( 1 )
, vComposantes_()
{
    vComposantes_.SetItemTypeName( "une composante" );
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Data destructor
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
ADN_Composantes_Data::~ADN_Composantes_Data()
{
    Reset();
}


//-----------------------------------------------------------------------------
// Name: ADN_Munitions_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Composantes_Data::FilesNeeded(T_StringList& files) const
{
    files.push_back(ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szComponents_.GetData());
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::Reset
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
void ADN_Composantes_Data::Reset()
{
    nNextId_ = 1;
    vComposantes_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ReadArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "Composantes" );
    while( input.NextListElement() )
    {
        std::auto_ptr<ComposanteInfos> spNew( new ComposanteInfos() );
        spNew->ReadArchive( input );
        vComposantes_.AddItem( spNew.release() );
    }
    input.EndList(); // Composantes
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::WriteArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.BeginList( "Composantes", vComposantes_.size() );

    for( IT_ComposanteInfos_Vector it = vComposantes_.begin(); it != vComposantes_.end(); ++it )
        (*it)->WriteArchive( output );
    output.EndList();
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::GetNextId
// Created: AGN 2003-11-21
// -----------------------------------------------------------------------------
int ADN_Composantes_Data::GetNextId()
{
    return nNextId_++;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::GetComposantesThatUse
// Created: APE 2005-04-22
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::GetComposantesThatUse( ADN_Sensors_Data::SensorInfos& sensor )
{
    std::string strResult;
    for( IT_ComposanteInfos_Vector it = vComposantes_.begin(); it != vComposantes_.end(); ++it )
    {
        ComposanteInfos* pComp = *it;
        for( IT_SensorInfos_Vector it2 = pComp->vSensors_.begin(); it2 != pComp->vSensors_.end(); ++it2 )
        {
            if( (*it2)->ptrSensor_.GetData() == &sensor )
            {
                if( strResult != "" )
                    strResult += "<br>";
                strResult += "<nobr>" + pComp->strName_.GetData() + "</nobr>";
                break;
            }
        }
    }
    return strResult;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::GetComposantesThatUse
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::GetComposantesThatUse( ADN_Weapons_Data::WeaponInfos& weapon )
{
    std::string strResult;
    for( IT_ComposanteInfos_Vector it = vComposantes_.begin(); it != vComposantes_.end(); ++it )
    {
        ComposanteInfos* pComp = *it;
        for( IT_WeaponInfos_Vector it2 = pComp->vWeapons_.begin(); it2 != pComp->vWeapons_.end(); ++it2 )
        {
            if( (*it2)->ptrWeapon_.GetData() == &weapon )
            {
                if( strResult != "" )
                    strResult += "<br>";
                strResult += "<nobr>" + pComp->strName_.GetData() + "</nobr>";
                break;
            }
        }
    }
    return strResult;
}