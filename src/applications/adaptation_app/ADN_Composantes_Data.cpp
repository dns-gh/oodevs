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
#include "adaptation_app_pch.h"
#include "ADN_Composantes_Data.h"
#include "ADN_GuiTools.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Objects_Data.h"
#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tr.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_DataException.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::AmbulanceInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Composantes_Data::AmbulanceInfos::AmbulanceInfos()
    : ADN_DataTreeNode_ABC()
    , transportSkills_    ()
    , bTransportNBC_      ( false )
    , bTransportShock_    ( false )
    , rCapacity_          ( 0 )
    , loadTimePerPerson_  ( "0s" )
    , unloadTimePerPerson_( "0s" )
{
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        transportSkills_[n] = false;
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
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        transportSkills_[n] = src.transportSkills_[n].GetData();
    bTransportNBC_ = src.bTransportNBC_.GetData();
    bTransportShock_ = src.bTransportShock_.GetData();
}

// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::AmbulanceInfos::ReadArchive( xml::xistream& input )
{
    std::string woundedTransport;
    input >> xml::attribute( "capacity", rCapacity_ )
          >> xml::attribute( "man-loading-time", loadTimePerPerson_ )
          >> xml::attribute( "man-unloading-time", unloadTimePerPerson_ )
          >> xml::optional >> xml::attribute( "wounded-transport", woundedTransport )
          >> xml::optional >> xml::attribute( "nbc-transport", bTransportNBC_ )
          >> xml::optional >> xml::attribute( "reac-mental-transport", bTransportShock_ );

    for( int n = 0; n < eNbrDoctorSkills; ++n )
    {
        const std::string skill = ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n );
        transportSkills_[n] = woundedTransport.find( skill ) != std::string::npos;
    }
}

// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::AmbulanceInfos::WriteArchive( const std::string& section, xml::xostream& output ) const
{
    std::string woundedTransports;
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        if( transportSkills_[n].GetData() )
        {
            if( ! woundedTransports.empty() )
                woundedTransports += ", ";
            woundedTransports += ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n );
        }
    output << xml::start( section )
            << xml::attribute( "capacity", rCapacity_ )
            << xml::attribute( "man-loading-time", loadTimePerPerson_ )
            << xml::attribute( "man-unloading-time", unloadTimePerPerson_ )
            << xml::attribute( "wounded-transport", woundedTransports );

    if( bTransportNBC_.GetData() )
        output << xml::attribute( "nbc-transport", bTransportNBC_ );
    if( bTransportShock_.GetData() )
        output << xml::attribute( "reac-mental-transport", bTransportShock_ );
    output << xml::end;
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
// Name: ADN_Composantes_Data::LogHealthInfos::ReadInfo
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogHealthInfos::ReadInfo( const std::string& type, xml::xistream& input )
{
    if( type == "collecting" )
    {
        bIsAmbulance_ = true;
        ambulanceInfos_.ReadArchive( input );
    } else if( type == "relieving" )
    {
        bIsAmbulanceReleve_ = true;
        ambulanceReleveInfos_.ReadArchive( input );
    } else if( type == "caring" )
    {
        bIsDoctor_ = true;
        std::string caring;
        input >> xml::optional >> xml::attribute( "sorting", bIsSortingDoctor_ )
              >> xml::optional >> xml::attribute( "nbc", bCuresNBC_ )
              >> xml::optional >> xml::attribute( "psychiatry", bCuresShock_ )
              >> xml::optional >> xml::attribute( "caring", caring );
        bIsCuringDoctor_ = ! caring.empty();
        for( int n = 0; n < eNbrDoctorSkills; ++n )
        {
            const std::string skill = ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n );
            doctorSkills_[n] = caring.find( skill ) != std::string::npos;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogHealthInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogHealthInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( *this, &ADN_Composantes_Data::LogHealthInfos::ReadInfo );
}

// -----------------------------------------------------------------------------
// Name: LogHealthInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogHealthInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "health-functions" );
    if( bIsAmbulance_.GetData() )
        ambulanceInfos_.WriteArchive( "collecting", output );

    if( bIsAmbulanceReleve_.GetData() )
        ambulanceReleveInfos_.WriteArchive( "relieving", output );

    if( bIsDoctor_.GetData() )
    {
        output << xml::start( "caring" );
        if( bIsSortingDoctor_.GetData() )
            output << xml::attribute( "sorting", true );
        if( bIsCuringDoctor_.GetData() )
        {
            std::string caring;
            for( int n = 0; n < eNbrDoctorSkills; ++n )
                if( doctorSkills_[n].GetData() )
                {
                    if( ! caring.empty() )
                        caring += ", ";
                    caring += ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n );
                }
            output << xml::attribute( "caring", caring );
        }
        if( bCuresNBC_.GetData() )
            output << xml::attribute( "nbc", true );
        if( bCuresShock_.GetData() )
            output << xml::attribute( "psychiatry", true );

        output << xml::end;
    }
    output << xml::end;
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
void ADN_Composantes_Data::NTIInfos::ReadArchive( xml::xistream& input )
{
    bIsPresent_ = true;
    std::string type;
    input >> xml::optional >> xml::attribute( "max-reparation-time", maxRepairTime_ )
          >> xml::optional >> xml::attribute( "type", type );

    bHasMaxRepairTime_ = maxRepairTime_ != "0s";
    bCanRepairEA_ = type.find( "EA" ) != std::string::npos;
    bCanRepairM_  = type.find( "M"  ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: NTIInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::NTIInfos::WriteArchive( xml::xostream& output ) const
{
    if( ! bIsPresent_.GetData() )
        return;
    output << xml::start( "repairing" );
    output << xml::attribute( "category", strName_ );
    if( bHasMaxRepairTime_.GetData() )
        output << xml::attribute( "max-reparation-time", maxRepairTime_ );
    std::string type;
    if( bCanRepairEA_.GetData() && bCanRepairM_.GetData() )
        type = "EA, M";
    else if( bCanRepairEA_.GetData() )
        type = "EA";
    else if( bCanRepairM_.GetData() )
        type = "M";
    if( ! type.empty() )
        output << xml::attribute( "type", type );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::LogMaintenanceInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Composantes_Data::LogMaintenanceInfos::LogMaintenanceInfos()
    : ADN_DataTreeNode_ABC()
    , bIsTower_           ( false )
    , rCapacity_          ( 0 )
    , loadTime_           ( "0s" )
    , unloadTime_         ( "0s" )
    , NTI1Infos_          ( "NTI1" )
    , NTI2Infos_          ( "NTI2" )
    , NTI3Infos_          ( "NTI3" )
{
}

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
    bIsTower_   = src.bIsTower_.GetData();
    rCapacity_  = src.rCapacity_.GetData();
    loadTime_   = src.loadTime_.GetData();
    unloadTime_ = src.unloadTime_.GetData();

    NTI1Infos_.CopyFrom( src.NTI1Infos_ );
    NTI2Infos_.CopyFrom( src.NTI2Infos_ );
    NTI3Infos_.CopyFrom( src.NTI3Infos_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::LogMaintenanceInfos::ReadInfo
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogMaintenanceInfos::ReadInfo( const std::string& type, xml::xistream& input )
{
    if( type == "towing" )
    {
        bIsTower_ = true;
        input >> xml::attribute( "capacity", rCapacity_ )
              >> xml::attribute( "loading-time", loadTime_ )
              >> xml::attribute( "unloading-time", unloadTime_ );
    } else if( type == "repairing" )
    {
        std::string category;
        input >> xml::attribute( "category", category );
        if( category == "NTI1" )
            NTI1Infos_.ReadArchive( input );
        else if( category == "NTI2" )
            NTI2Infos_.ReadArchive( input );
        else if( category == "NTI3" )
            NTI3Infos_.ReadArchive( input );
    }
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogMaintenanceInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( *this, &ADN_Composantes_Data::LogMaintenanceInfos::ReadInfo );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogMaintenanceInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "maintenance-functions" );
    if( bIsTower_.GetData() )
    {
        output << xml::start( "towing" )
                << xml::attribute( "capacity", rCapacity_ )
                << xml::attribute( "loading-time", loadTime_ )
                << xml::attribute( "unloading-time", unloadTime_ )
               << xml::end;
    }
    NTI1Infos_.WriteArchive( output );
    NTI2Infos_.WriteArchive( output );
    NTI3Infos_.WriteArchive( output );
    output << xml::end;
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
, ptrResourceNature_  ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetDotationNaturesInfos(), 0 )
{
    // NOTHING
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
    ptrResourceNature_ = src.ptrResourceNature_.GetData();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogSupplyInfos::ReadArchive( xml::xistream& input )
{
    std::string strNature;
    input >> xml::optional
          >> xml::start( "carrying" )
            >> xml::attribute( "mass", rWeight_ )
            >> xml::attribute( "volume", rVolume_ )
            >> xml::attribute( "nature", strNature )
          >> xml::end;
    if( !strNature.empty() )
    {
        bIsCarrier_ = true;
        helpers::ResourceNatureInfos* pNature = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindDotationNature( strNature );
        if( !pNature )
            throw ADN_DataException( tools::translate( "Composante_Data", "Invalid data" ).ascii(), tools::translate( "Composante_Data", "Equipment - Invalid resource nature '%1'" ).arg( strNature.c_str() ).ascii() );
        ptrResourceNature_ = pNature;
    }
}

// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogSupplyInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "supply-functions" );
    if( bIsCarrier_.GetData() )
        output << xml::start( "carrying" )
                << xml::attribute( "mass", rWeight_ )
                << xml::attribute( "volume", rVolume_ )
                << xml::attribute( "nature", ptrResourceNature_.GetData()->GetData() )
               << xml::end;
    output << xml::end;
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
// Name: ADN_Composantes_Data::LogInfos::ReadLogisticFunction
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogInfos::ReadLogisticFunction( const std::string& type, xml::xistream& input )
{
    if( type == "supply-functions" ) {
        bHasSupplyInfos_ = true;
        supplyInfos_.ReadArchive( input );
    } else if( type == "health-functions" ) {
        bHasHealthInfos_ = true;
        healthInfos_.ReadArchive( input );
    } else if( type == "maintenance-functions" ) {
        bHasMaintenanceInfos_ = true;
        maintenanceInfos_.ReadArchive( input );
    }
}

// -----------------------------------------------------------------------------
// Name: LogInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::optional
            >> xml::start( "logistic-functions" )
                >> xml::list( *this, &ADN_Composantes_Data::LogInfos::ReadLogisticFunction )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: LogInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::LogInfos::WriteArchive( xml::xostream& output ) const
{
    if( ! bHasHealthInfos_.GetData() && ! bHasMaintenanceInfos_.GetData() && ! bHasSupplyInfos_.GetData() )
        return;

    output << xml::start( "logistic-functions" );
    if( bHasHealthInfos_.GetData() )
        healthInfos_.WriteArchive( output );
    if( bHasMaintenanceInfos_.GetData() )
        maintenanceInfos_.WriteArchive( output );
    if( bHasSupplyInfos_.GetData() )
        supplyInfos_.WriteArchive( output );
    output << xml::end;
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
void ADN_Composantes_Data::BreakdownInfos::ReadArchive( xml::xistream& input )
{
    std::string strName;
    input >> xml::attribute( "type", strName )
          >> xml::attribute( "percentage", rPercentage_ );
    ADN_Breakdowns_Data::BreakdownInfo* pBreakdown = ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().FindBreakdown( strName );
    if( pBreakdown == 0 )
        throw ADN_DataException( tools::translate( "Composante_Data", "Invalid data" ).ascii(), tools::translate( "Composante_Data", "Equipment - Invalid breakdown type '%1'" ).arg( strName.c_str() ).ascii() );
    ptrBreakdown_ = pBreakdown;
    ptrBreakdown_.SetVector( ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().vBreakdowns_ );
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfos::WriteArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::BreakdownInfos::WriteArchive( const std::string& origin, xml::xostream& output ) const
{
    output << xml::start( "breakdown" )
            << xml::attribute( "origin", origin )
            << xml::attribute( "type", ptrBreakdown_.GetData()->strName_ )
            << xml::attribute( "percentage", rPercentage_ )
           << xml::end;
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
    vBreakdowns_.Delete();
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
// Name: ADN_Composantes_Data::BreakdownGroupInfos::ReadBreakdown
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::BreakdownGroupInfos::ReadBreakdown( xml::xistream& input )
{
    if( input.attribute< std::string >( "origin" ) == strName_ )
    {
        std::auto_ptr<BreakdownInfos> spNew( new BreakdownInfos() );
        spNew->ReadArchive( input );
        vBreakdowns_.AddItem( spNew.release() );
    }
}

// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::ReadArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::BreakdownGroupInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::optional
          >> xml::start( "breakdowns" )
            >> xml::list( "breakdown", *this, &ADN_Composantes_Data::BreakdownGroupInfos::ReadBreakdown )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::WriteArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::BreakdownGroupInfos::WriteArchive( xml::xostream& output ) const
{
    double rSum = 0.0;
    for( CIT_BreakdownInfos_Vector it = vBreakdowns_.begin(); it != vBreakdowns_.end() ; ++it )
        rSum += (*it)->rPercentage_.GetData();
    if( rSum != 100.0 )
        throw ADN_DataException( tools::translate( "Composante_Data",  "Invalid data" ).ascii(), tools::translate( "Composante_Data",  "Equipment - Invalid breakdown data : sum != 100" ).ascii() );

    for( CIT_BreakdownInfos_Vector it = vBreakdowns_.begin(); it != vBreakdowns_.end() ; ++it )
        (*it)->WriteArchive( strName_, output );
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
void ADN_Composantes_Data::SpeedInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: SpeedInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::SpeedInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "speed" )
            << xml::attribute( "terrain", ADN_Tr::ConvertFromLocation( nTypeTerrain_ ) )
            << xml::attribute( "value", rSpeed_ )
           << xml::end;
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
void ADN_Composantes_Data::SensorInfos::ReadArchive( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "height", rHeight_ )
          >> xml::attribute( "type", type );
    ADN_Sensors_Data::SensorInfos* pSensor = ADN_Workspace::GetWorkspace().GetSensors().GetData().FindSensor( type );
    if( pSensor == NULL )
        throw ADN_DataException( tools::translate( "Composante_Data",  "Invalid data" ).ascii(),  tools::translate( "Composante_Data", "Equipment - Invalid sensor type '%1'" ).arg( type.c_str() ).ascii() );
    ptrSensor_ = pSensor;
}

// -----------------------------------------------------------------------------
// Name: SensorInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::SensorInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "sensor" )
            << xml::attribute( "height", rHeight_ )
            << xml::attribute( "type", ptrSensor_.GetData()->strName_ )
           << xml::end;
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
void ADN_Composantes_Data::RadarInfos::ReadArchive( xml::xistream& input )
{
    std::string strRadarName;
    input >> xml::attribute( "type", strRadarName );
    ADN_Radars_Data::RadarInfos* pRadar = ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_.FindRadar( strRadarName );
    if( pRadar == 0 )
        throw ADN_DataException( tools::translate( "Composante_Data",  "Invalid data" ).ascii(),  tools::translate( "Composante_Data", "Equipment - Invalid radar type '%1'" ).arg( strRadarName.c_str() ).ascii() );
    ptrRadar_ = pRadar;
    strName_ = strRadarName;
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::WriteArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::RadarInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "radar" )
            << xml::attribute( "type", ptrRadar_.GetData()->strName_ )
           << xml::end;
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
void ADN_Composantes_Data::WeaponInfos::ReadArchive( xml::xistream& input )
{
    std::string strLauncher, strAmmunition;
    input >> xml::attribute( "launcher", strLauncher )
          >> xml::attribute( "munition", strAmmunition );
    ADN_Weapons_Data::WeaponInfos* pWeapon = ADN_Workspace::GetWorkspace().GetWeapons().GetData().FindWeapon( strLauncher, strAmmunition );
    if( !pWeapon )
        throw ADN_DataException( tools::translate( "Composante_Data",  "Invalid data" ).ascii(), tools::translate( "Composante_Data",  "Equipment - Invalid weapon system '%1/%2'" ).arg( strLauncher.c_str(), strAmmunition.c_str() ).ascii() );
    ptrWeapon_ = pWeapon;
    strName_ = pWeapon->strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: WeaponInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::WeaponInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "weapon-system" )
            << xml::attribute( "launcher", ptrWeapon_.GetData()->ptrLauncher_.GetData()->strName_ )
            << xml::attribute( "munition", ptrWeapon_.GetData()->ptrAmmunition_.GetData()->strName_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::HumanProtectionInfos
// Created: JCR 2009-05-17
// -----------------------------------------------------------------------------
ADN_Composantes_Data::HumanProtectionInfos::HumanProtectionInfos()
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::GetNodeName
// Created: JCR 2009-05-17
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::HumanProtectionInfos::GetNodeName()
{
    return "human-protection";
}
// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::GetItemName
// Created: JCR 2009-05-17
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::HumanProtectionInfos::GetItemName()
{
    return "nothing";
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::CopyFrom
// Created: JCR 2009-05-17
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::HumanProtectionInfos::CopyFrom( HumanProtectionInfos& )
{

}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ReadArchive
// Created: JCR 2009-05-17
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::HumanProtectionInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::optional >> xml::start( "human-protections" )
                             >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::WriteArchive
// Created: JCR 2009-05-17
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::HumanProtectionInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "human-protections" ) << xml::end;
}


//-----------------------------------------------------------------------------
// Name: ActiveProtectionsInfos::ActiveProtectionsInfos
// Created: FDS 10-02-24
//-----------------------------------------------------------------------------
ADN_Composantes_Data::ActiveProtectionsInfos::ActiveProtectionsInfos()
: ADN_Ref_ABC()
, ptrActiveProtections_ ( ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsInfos(), 0 )
{
    BindExistenceTo(&ptrActiveProtections_);
}

// -----------------------------------------------------------------------------
// Name: ActiveProtectionsInfos::ActiveProtectionsInfos
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_Composantes_Data::ActiveProtectionsInfos::ActiveProtectionsInfos( ADN_ActiveProtections_Data::ActiveProtectionsInfos& activeProtections )
: ADN_Ref_ABC()
, ptrActiveProtections_ ( ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsInfos(), &activeProtections, "" )
, strName_   ( activeProtections.strName_.GetData() )
{
    BindExistenceTo(&ptrActiveProtections_);
}

// -----------------------------------------------------------------------------
// Name: ActiveProtectionsInfos::GetNodeName
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ActiveProtectionsInfos::GetNodeName()
{
    std::string strResult( "de la protection active " );
    return strResult + ptrActiveProtections_.GetData()->GetNodeName();
}

// -----------------------------------------------------------------------------
// Name: ActiveProtectionsInfos::GetItemName
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ActiveProtectionsInfos::GetItemName()
{
    return ((ADN_ActiveProtections_Data::ActiveProtectionsInfos*)ptrActiveProtections_.GetData())->strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ActiveProtectionsInfos::CreateCopy
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_Composantes_Data::ActiveProtectionsInfos* ADN_Composantes_Data::ActiveProtectionsInfos::CreateCopy()
{
    ActiveProtectionsInfos* pCopy = new ActiveProtectionsInfos();
    pCopy->ptrActiveProtections_ = ptrActiveProtections_.GetData();
    pCopy->strName_ = strName_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ActiveProtectionsInfos::ReadArchive
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ActiveProtectionsInfos::ReadArchive( xml::xistream& input )
{
    std::string strActiveProtectionName;
    input >> xml::attribute( "name", strActiveProtectionName );
    ADN_ActiveProtections_Data::ActiveProtectionsInfos* pActiveProtection = ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().FindActiveProtection( strActiveProtectionName );
    if( pActiveProtection == 0 )
        throw ADN_DataException( tools::translate( "Composante_Data",  "Invalid data" ).ascii(), tools::translate( "Composante_Data",  "Equipment - Invalid active protection type '%1'" ).arg( strActiveProtectionName.c_str() ).ascii() );
    ptrActiveProtections_ = pActiveProtection;
    strName_ = strActiveProtectionName;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::WriteArchive
// Created: LDC 2010-01-12
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ActiveProtectionsInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "protection" )
            << xml::attribute( "name", ptrActiveProtections_.GetData()->strName_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfos::CategoryInfos
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Composantes_Data::CategoryInfos::CategoryInfos( ADN_Equipement_Data::ResourceInfos& parentDotation )
: ADN_Ref_ABC( "ADN_Composantes_Data::CategoryInfos" )
, ptrDotation_( &parentDotation, "" )
, ptrCategory_( parentDotation.categories_, 0, "ptr cat ds ADN_Composantes_Data::CategoryInfos" )
, rNormalizedConsumption_( 0 )
, rLogThreshold_( 0 )
, rNbr_         ( 0 )

{
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
void ADN_Composantes_Data::CategoryInfos::ReadArchive( xml::xistream& input )
{
    std::string strCategory;
    input >> xml::attribute( "name", strCategory )
          >> xml::attribute( "capacity", rNbr_ )
          >> xml::attribute( "logistic-threshold", rLogThreshold_ )
          >> xml::optional >> xml::attribute( "normalized-consumption", rNormalizedConsumption_ );
    ADN_Equipement_Data::CategoryInfo* pCat = ptrDotation_.GetData()->FindCategory( strCategory );
    if( !pCat )
        throw ADN_DataException( tools::translate( "Composante_Data",  "Invalid data" ).ascii(), tools::translate( "Composante_Data",  "Equipment - Invalid resource type '%1'" ).arg( strCategory.c_str() ).ascii() );
    ptrCategory_ = pCat;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfos::WriteArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::CategoryInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "resource" )
            << xml::attribute( "name", ptrCategory_.GetData()->strName_ )
            << xml::attribute( "capacity", rNbr_ )
            << xml::attribute( "logistic-threshold", rLogThreshold_ )
            << xml::attribute( "normalized-consumption", rNormalizedConsumption_ )
           << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ResourceInfos::ResourceInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Composantes_Data::ResourceInfos::ResourceInfos()
: ADN_Ref_ABC()
, categories_()
{
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ResourceInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Composantes_Data::ResourceInfos::GetItemName()
{
    return GetNodeName();
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::CreateCopy
// Created: APE 2004-11-29
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ResourceInfos::CopyFrom( ADN_Composantes_Data::ResourceInfos& src )
{
    for( IT_CategoryInfos_Vector it = src.categories_.begin(); it != src.categories_.end(); ++it )
        categories_.AddItem( (*it)->CreateCopy() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ResourceInfos::ReadCategory
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ResourceInfos::ReadCategory( xml::xistream& input )
{
    std::string name = input.attribute< std::string >( "name" );
    E_DotationFamily family = ENT_Tr::ConvertToDotationFamily( name );
    ADN_Equipement_Data::ResourceInfos& dotation = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( family );

    input >> xml::list( "resource", *this, &ADN_Composantes_Data::ResourceInfos::ReadDotation, dotation );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ResourceInfos::ReadDotation
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ResourceInfos::ReadDotation( xml::xistream& input, ADN_Equipement_Data::ResourceInfos& dotation )
{
    std::auto_ptr< CategoryInfos > pInfo( new CategoryInfos( dotation ) );
    pInfo->ReadArchive( input );
    categories_.AddItem( pInfo.release() );
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ResourceInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( "category", *this, &ADN_Composantes_Data::ResourceInfos::ReadCategory );
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ResourceInfos::WriteArchive( xml::xostream& output ) const
{
    for( uint n = 0; n < eNbrDotationFamily; ++n )
    {
        bool entered = false;
        for( CIT_CategoryInfos_Vector it = categories_.begin(); it != categories_.end(); ++it )
            if( (*it)->ptrDotation_.GetData()->nType_ == E_DotationFamily( n ) )
            {
                if( !entered )
                {
                    output << xml::start( "category" )
                            << xml::attribute( "name", (*it)->ptrDotation_.GetData()->strName_ );
                    entered = true;
                }
                (*it)->WriteArchive( output );
            }
        if( entered )
            output << xml::end;
    }
}

//-----------------------------------------------------------------------------
// Name: ObjectInfos::ObjectInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Composantes_Data::ObjectInfos::ObjectInfos()
: ADN_Ref_ABC()
, ptrObject_( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos() ,0 )
, initialBuildTime_         ( "-1s" )
, initialDestructionTime_   ( "-1s" )
, coeffBuildTime_           ( "-1s" )
, coeffDestructionTime_     ( "-1s" )
, valorizationTime_         ( "-1s" )
, devalorizationTime_       ( "-1s" )
, extinguishingTime_        ( "-1s" )
, rCoeffCirc_               ( 0 )
, rSpeedCirc_               ( 0 )
, rSpeedNotCirc_            ( 0 )
, bInitialBuildTime_        ( false )
, bInitialDestructionTime_  ( false )
, bCoeffBuildTime_          ( false )
, bCoeffDestructionTime_    ( false )
, bValorizationTime_        ( false )
, bDevalorizationTime_      ( false )
, bExtinguishingTime_       ( false )
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
    extinguishingTime_.SetDataName( "le temps d'extinction" );
    extinguishingTime_.SetParentNode( *this );
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
    pCopy->extinguishingTime_ = extinguishingTime_.GetData();
    pCopy->rCoeffCirc_ = rCoeffCirc_.GetData();
    pCopy->rSpeedCirc_ = rSpeedCirc_.GetData();
    pCopy->rSpeedNotCirc_ = rSpeedNotCirc_.GetData();

    pCopy->bInitialBuildTime_ = bInitialBuildTime_.GetData();
    pCopy->bInitialDestructionTime_ = bInitialDestructionTime_.GetData();
    pCopy->bCoeffBuildTime_ = bCoeffBuildTime_.GetData();
    pCopy->bCoeffDestructionTime_ = bCoeffDestructionTime_.GetData();
    pCopy->bValorizationTime_ = bValorizationTime_.GetData();
    pCopy->bDevalorizationTime_ = bDevalorizationTime_.GetData();
    pCopy->bExtinguishingTime_ = bExtinguishingTime_.GetData();
    pCopy->bCoeffCircTime_ = bCoeffCircTime_.GetData();
    pCopy->bSpeedCirc_ = bSpeedCirc_.GetData();
    pCopy->bSpeedNotCirc_ = bSpeedNotCirc_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ObjectInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ObjectInfos::ReadArchive( xml::xistream& input )
{
    std::string strType;
    input >> xml::attribute( "type", strType );

    ADN_Objects_Data_ObjectInfos* pObject = ADN_Workspace::GetWorkspace().GetObjects().GetData().FindObject( strType );
    if( !pObject )
        throw ADN_DataException( tools::translate( "Composante_Data",  "Invalid data" ).ascii(), tools::translate( "Composante_Data",  "Equipment - Invalid object type '%1'" ).arg( strType.c_str() ).ascii() );
    ptrObject_ = pObject;

    input >> xml::optional >> xml::attribute( "initial-construction-time", initialBuildTime_ )
          >> xml::optional >> xml::attribute( "initial-destruction-time", initialDestructionTime_ )
          >> xml::optional >> xml::attribute( "construction-time-factor", coeffBuildTime_ )
          >> xml::optional >> xml::attribute( "destruction-time-factor", coeffDestructionTime_ )
          >> xml::optional >> xml::attribute( "valorization-time", valorizationTime_ )
          >> xml::optional >> xml::attribute( "devalorization-time", devalorizationTime_ )
          >> xml::optional >> xml::attribute( "extinguishing-time", extinguishingTime_ )
          >> xml::optional >> xml::attribute( "bypass-gain-factor", rCoeffCirc_ )
          >> xml::optional >> xml::attribute( "non-bypassed-speed", rSpeedCirc_ )
          >> xml::optional >> xml::attribute( "bypassed-speed", rSpeedNotCirc_ );

    bInitialBuildTime_          = initialBuildTime_ != "-1s";
    bInitialDestructionTime_    = initialDestructionTime_ != "-1s";
    bCoeffBuildTime_            = coeffBuildTime_ != "-1s";
    bCoeffDestructionTime_      = coeffDestructionTime_ != "-1s";
    bValorizationTime_          = valorizationTime_ != "-1s";
    bDevalorizationTime_        = devalorizationTime_ != "-1s";
    bExtinguishingTime_         = extinguishingTime_ != "-1s";
    bCoeffCircTime_             = rCoeffCirc_ != 0;
    bSpeedCirc_                 = rSpeedCirc_ != 0;
    bSpeedNotCirc_              = rSpeedNotCirc_ != 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ObjectInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "object" )
            << xml::attribute( "type", ptrObject_.GetData()->strType_ );
    if( bInitialBuildTime_.GetData() )
        output << xml::attribute( "initial-construction-time", initialBuildTime_ );
    if( bInitialDestructionTime_.GetData() )
        output << xml::attribute( "initial-destruction-time", initialDestructionTime_ );
    if( bCoeffBuildTime_.GetData() )
        output << xml::attribute( "construction-time-factor", coeffBuildTime_ );
    if( bCoeffDestructionTime_.GetData() )
        output << xml::attribute( "destruction-time-factor", coeffDestructionTime_ );
    if( bValorizationTime_.GetData() )
        output << xml::attribute( "valorization-time", valorizationTime_ );
    if( bDevalorizationTime_.GetData() )
        output << xml::attribute( "devalorization-time", devalorizationTime_ );
    if( bExtinguishingTime_.GetData() )
        output << xml::attribute( "extinguishing-time", extinguishingTime_ );
    if( bCoeffCircTime_.GetData() )
        output << xml::attribute( "bypass-gain-factor", rCoeffCirc_ );
    if( bSpeedCirc_.GetData() )
        output << xml::attribute( "non-bypassed-speed", rSpeedCirc_ );
    if( bSpeedNotCirc_.GetData() )
        output << xml::attribute( "bypassed-speed", rSpeedNotCirc_ );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ConsumptionItem::ConsumptionItem
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
ADN_Composantes_Data::ConsumptionItem::ConsumptionItem( E_ConsumptionType nConsumptionType, ADN_Equipement_Data::CategoryInfo& category )
: nConsumptionType_    ( nConsumptionType )
, ptrCategory_         ( category.parentResource_.categories_, &category, "" )
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
void ADN_Composantes_Data::ConsumptionItem::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", nQuantityUsedPerHour_ );
}

// -----------------------------------------------------------------------------
// Name: ConsumptionItem::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ConsumptionItem::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "resource" )
            << xml::attribute( "category", ptrCategory_.GetData()->parentResource_.strName_ )
            << xml::attribute( "name", ptrCategory_.GetData()->strName_ )
            << xml::attribute( "value", nQuantityUsedPerHour_ )
           << xml::end;
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
// Name: ADN_Composantes_Data::ConsumptionsInfos::ReadConsumption
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ConsumptionsInfos::ReadConsumption( xml::xistream& input )
{
    std::string status;
    input >> xml::attribute( "status", status );
    E_ConsumptionType type = ADN_Tr::ConvertToConsumptionType( status );
    if( type == E_ConsumptionType( -1 ) )
        throw ADN_DataException( tools::translate( "Composante_Data",  "Invalid data" ).ascii(), tools::translate( "Composante_Data",  "Equipment - Invalid activty '%1'" ).arg( status.c_str() ).ascii() );
    input >> xml::list( "resource", *this, &ADN_Composantes_Data::ConsumptionsInfos::ReadDotation, type );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ConsumptionsInfos::ReadDotation
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ConsumptionsInfos::ReadDotation( xml::xistream& input, const E_ConsumptionType& type )
{
    std::string category, name;
    input >> xml::attribute( "category", category )
          >> xml::attribute( "name", name );
    ADN_Equipement_Data::CategoryInfo* pCategory = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( category, name );
    if( !pCategory )
        throw ADN_DataException( tools::translate( "Composante_Data",  "Invalid data" ).ascii(), tools::translate( "Composante_Data",  "Equipment - Invalid resource type '%1'" ).arg( name.c_str() ).ascii() );
    std::auto_ptr<ConsumptionItem> spNew( new ConsumptionItem( type, *pCategory ) );
    spNew->ReadArchive( input );
    vConsumptions_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::ReadArchive
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ConsumptionsInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "consumptions" )
            >> xml::list( "consumption", *this, &ADN_Composantes_Data::ConsumptionsInfos::ReadConsumption )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::WriteArchive
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ConsumptionsInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "consumptions" );
    for( int nType = 0; nType < eNbrConsumptionType; ++nType )
    {
        bool entered = false;
        for( CIT_ConsumptionItem_Vector it = vConsumptions_.begin(); it != vConsumptions_.end(); ++it )
            if( (*it)->nConsumptionType_ == nType )
            {
                if( ! entered )
                {
                    output << xml::start( "consumption" )
                            << xml::attribute( "status", ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)nType ) );
                    entered = true;
                }
                (*it)->WriteArchive( output );
            }
        if( entered )
            output << xml::end;
    }
    output << xml::end;
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
    , nMosId_                        ( ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetNextId() )
    , equipmentCategory_             ( "Autres" )
    , ptrArmor_                      ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos(), 0 )
    , ptrSize_                       ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos(), 0 )
    , rWeight_                       ( 100 )
    , vSpeeds_                       ( false )
    , bTroopEmbarkingTimes_          ( false )
    , embarkingTimePerPerson_        ( "0s" )
    , disembarkingTimePerPerson_     ( "0s" )
    , bCanCarryCargo_                ( false )
    , rWeightTransportCapacity_      ( 0 )
    , embarkingTimePerTon_           ( "0s" )
    , disembarkingTimePerTon_        ( "0s" )
    , bCanCarryCrowd_                ( false )
    , nCrowdTransportCapacity_       ( 0 )
    , crowdEmbarkingTimePerPerson_   ( "0s" )
    , crowdDisembarkingTimePerPerson_( "0s" )
    , rMaxSpeed_                     ( 100 )
    , attritionBreakdowns_           ( "attrition" )
    , randomBreakdowns_              ( "random" )
    , bMaxSlope_                     ( false )
    , rMaxSlope_                     ( 60 )
    , nPowerDirectFire_              ( 0 )
    , nPowerIndirectFire_            ( 0 )
    , nPowerCloseCombat_             ( 0 )
    , nPowerEngineering_             ( 0 )
    {
    BindExistenceTo( &ptrArmor_ );
    BindExistenceTo( &ptrSize_ );

    strName_.SetDataName( "le nom" );
    strName_.SetParentNode( *this );

    ptrArmor_.SetNodeName( "le niveau de blindage" );
    ptrArmor_.SetParentNode( *this );
    ptrSize_.SetNodeName( "le volume" );
    ptrSize_.SetParentNode( *this );

    vSpeeds_.SetParentNode( *this );

    vSensors_.SetParentNode( *this );
    vSensors_.SetItemTypeName( "un senseur" );

    vWeapons_.SetParentNode( *this );
    vWeapons_.SetItemTypeName( "un lanceur" );

    vActiveProtections_.SetParentNode( *this );
    vActiveProtections_.SetItemTypeName( "une protection active" );

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
    vSpeeds_.Delete();
    vWeapons_.Delete();
    vActiveProtections_.Delete();
    vSensors_.Delete();
    vRadars_.Delete();
    vObjects_.Delete();
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

    for( int iTerrain = 0; iTerrain < eNbrLocation; ++iTerrain )
        pCopy->vSpeeds_[ iTerrain ]->rSpeed_ = vSpeeds_[ iTerrain ]->rSpeed_.GetData();


    for( IT_WeaponInfos_Vector itWeapon = vWeapons_.begin(); itWeapon != vWeapons_.end(); ++itWeapon )
    {
        WeaponInfos* pNew = (*itWeapon)->CreateCopy();
        pCopy->vWeapons_.AddItem( pNew );
    }

    for( IT_ActiveProtectionsInfos_Vector itActiveProtections = vActiveProtections_.begin(); itActiveProtections != vActiveProtections_.end(); ++itActiveProtections )
    {
        ActiveProtectionsInfos* pNew = (*itActiveProtections)->CreateCopy();
        pCopy->vActiveProtections_.AddItem( pNew );
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
    pCopy->resources_.CopyFrom( resources_ );

    pCopy->bTroopEmbarkingTimes_ = bTroopEmbarkingTimes_.GetData();
    pCopy->embarkingTimePerPerson_ = embarkingTimePerPerson_.GetData();
    pCopy->disembarkingTimePerPerson_ = disembarkingTimePerPerson_.GetData();
    pCopy->bCanCarryCargo_ = bCanCarryCargo_.GetData();
    pCopy->rWeightTransportCapacity_ = rWeightTransportCapacity_.GetData();
    pCopy->embarkingTimePerTon_ = embarkingTimePerTon_.GetData();
    pCopy->disembarkingTimePerTon_ = disembarkingTimePerTon_.GetData();
    pCopy->bCanCarryCrowd_ = bCanCarryCrowd_.GetData();
    pCopy->nCrowdTransportCapacity_ = nCrowdTransportCapacity_.GetData();
    pCopy->crowdEmbarkingTimePerPerson_ = crowdEmbarkingTimePerPerson_.GetData();
    pCopy->crowdDisembarkingTimePerPerson_ = crowdDisembarkingTimePerPerson_.GetData();

    pCopy->logInfos_.CopyFrom( logInfos_ );

    pCopy->attritionBreakdowns_.CopyFrom( attritionBreakdowns_ );
    pCopy->randomBreakdowns_.CopyFrom( randomBreakdowns_ );

    pCopy->bMaxSlope_ = bMaxSlope_.GetData();
    pCopy->rMaxSlope_ = rMaxSlope_.GetData();

    pCopy->nPowerDirectFire_    = nPowerDirectFire_.GetData();
    pCopy->nPowerIndirectFire_  = nPowerIndirectFire_.GetData();
    pCopy->nPowerCloseCombat_   = nPowerCloseCombat_.GetData();
    pCopy->nPowerEngineering_   = nPowerEngineering_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ComposanteInfos::ReadSpeed
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ComposanteInfos::ReadSpeed( xml::xistream& input )
{
    std::string strLocation;
    input >> xml::attribute( "terrain", strLocation );
    
    try
    {
        E_Location nLocation = ADN_Tr::ConvertToLocation( strLocation );
        vSpeeds_.at( nLocation )->ReadArchive( input );
    }
    catch ( const std::exception& /*ex*/ )
    {
        throw ADN_DataException( tools::translate( "Composantes_Data", "Invalid data" ).ascii(), tools::translate( "Composantes_Data", "Equipment - Invalid terrain type '%1'" ).arg( strLocation.c_str() ).ascii() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ComposanteInfos::ReadSensor
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ComposanteInfos::ReadSensor( xml::xistream& input )
{
    std::auto_ptr<SensorInfos> spNew( new SensorInfos() );
    spNew->ReadArchive( input );
    vSensors_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ComposanteInfos::ReadRadar
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ComposanteInfos::ReadRadar( xml::xistream& input )
{
    std::auto_ptr<RadarInfos> spNew( new RadarInfos() );
    spNew->ReadArchive( input );
    vRadars_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ComposanteInfos::ReadWeapon
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ComposanteInfos::ReadWeapon( xml::xistream& input )
{
    std::auto_ptr<WeaponInfos> spNew( new WeaponInfos() );
    spNew->ReadArchive( input );
    vWeapons_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ComposanteInfos::ReadActiveProtection
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ComposanteInfos::ReadActiveProtection( xml::xistream& input )
{
    std::auto_ptr<ActiveProtectionsInfos> spNew( new ActiveProtectionsInfos() );
    spNew->ReadArchive( input );
    vActiveProtections_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ComposanteInfos::ReadObject
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ComposanteInfos::ReadObject( xml::xistream& input )
{
    std::auto_ptr<ObjectInfos> spNew( new ObjectInfos() );
    spNew->ReadArchive( input );
    vObjects_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ComposanteInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ComposanteInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "comment", strAdditionalComments_ )
          >> xml::attribute( "name", strName_ );

    strCodeEMAT6_ = strName_.GetData();
    strCodeEMAT8_ = strName_.GetData();
    strCodeLFRIL_ = strName_.GetData();
    strCodeNNO_ = strName_.GetData();
    input >> xml::optional >> xml::attribute( "codeEMAT6", strCodeEMAT6_ )
          >> xml::optional >> xml::attribute( "codeEMAT8", strCodeEMAT8_ )
          >> xml::optional >> xml::attribute( "codeLFRIL", strCodeLFRIL_ )
          >> xml::optional >> xml::attribute( "codeNNO",   strCodeNNO_   );

    std::string strArmor, strSize;
    input >> xml::attribute( "protection", strArmor )
          >> xml::attribute( "size", strSize )
          >> xml::attribute( "weight", rWeight_ );
    input >> xml::optional >> xml::content( "equipment-category", equipmentCategory_ );

    ptrArmor_ = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindArmor( strArmor );
    ptrSize_ = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindSize( strSize );
    if( ! ptrArmor_.GetData() || ! ptrSize_.GetData() )
        throw ADN_DataException( tools::translate( "Composante_Data",  "Invalid data" ).ascii(), tools::translate( "Composante_Data",  "Equipment - Invalid armor and/or volume" ).ascii() );

    input >> xml::start( "speeds" )
            >> xml::attribute( "max", rMaxSpeed_ )
            >> xml::list( "speed", *this, &ADN_Composantes_Data::ComposanteInfos::ReadSpeed )
          >> xml::end;

    input >> xml::start( "composition" );
    resources_.ReadArchive( input );
    input >> xml::end;

    input >> xml::start( "sensors" )
            >> xml::list( "sensor", *this, &ADN_Composantes_Data::ComposanteInfos::ReadSensor )
          >> xml::end;

    input >> xml::optional >> xml::start( "radars" )
            >> xml::list( "radar", *this, &ADN_Composantes_Data::ComposanteInfos::ReadRadar )
          >> xml::end;

    input >> xml::start( "transports" )
            >> xml::optional
            >> xml::start( "crew" )
                >> xml::attribute( "man-boarding-time", embarkingTimePerPerson_ )
                >> xml::attribute( "man-unloading-time", disembarkingTimePerPerson_ )
            >> xml::end
            >> xml::optional
            >> xml::start( "unit" )
                >> xml::attribute( "capacity", rWeightTransportCapacity_ )
                >> xml::attribute( "ton-loading-time", embarkingTimePerTon_ )
                >> xml::attribute( "ton-unloading-time", disembarkingTimePerTon_ )
            >> xml::end
            >> xml::optional
            >> xml::start( "crowd" )
                >> xml::attribute( "capacity", nCrowdTransportCapacity_ )
                >> xml::attribute( "man-boarding-time", crowdEmbarkingTimePerPerson_ )
                >> xml::attribute( "man-unloading-time", crowdDisembarkingTimePerPerson_ )
            >> xml::end
          >> xml::end;
    bTroopEmbarkingTimes_ = embarkingTimePerPerson_ != "0s" || disembarkingTimePerPerson_ != "0s";
    bCanCarryCargo_ = rWeightTransportCapacity_ != 0.;
    bCanCarryCrowd_ = nCrowdTransportCapacity_ != 0;

    consumptions_.ReadArchive( input );

    input >> xml::start( "weapon-systems" )
            >> xml::list( "weapon-system", *this, &ADN_Composantes_Data::ComposanteInfos::ReadWeapon )
          >> xml::end
          >> xml::optional >> xml::start ( "active-protections" )
            >> xml::list( "protection", *this, &ADN_Composantes_Data::ComposanteInfos::ReadActiveProtection )
          >> xml::end
          >> xml::start( "objects" )
            >> xml::list( "object", *this, &ADN_Composantes_Data::ComposanteInfos::ReadObject )
          >> xml::end;
    if ( input.has_child( "power-indicators" ) )
    {
        input >> xml::start( "power-indicators" )
                >> xml::attribute( "direct-fire", nPowerDirectFire_ )
                >> xml::attribute( "indirect-fire", nPowerIndirectFire_ )
                >> xml::attribute( "close-combat", nPowerCloseCombat_ )
                >> xml::attribute( "engineering", nPowerEngineering_ )
              >> xml::end;
    }

    logInfos_.ReadArchive( input );

    randomBreakdowns_.ReadArchive( input );
    attritionBreakdowns_.ReadArchive( input );

    input >> xml::optional >> xml::attribute( "max-slope", rMaxSlope_ );
    if( rMaxSlope_ != 60. )
    {
        bMaxSlope_ = true;
        rMaxSlope_ =  rMaxSlope_.GetData() * 100.0;
    }

    input >> xml::optional >> xml::start( "operational-information" )
            >> xml::optional >> xml::attribute( "native-country", strNativeCountry_ )
            >> xml::optional >> xml::attribute( "starting-country", strStartingCountry_ )
            >> xml::optional >> xml::attribute( "starting-date", strStartingDate_ )
            >> xml::optional >> xml::attribute( "information-origin", strInformationOrigin_ )
          >> xml::end;

    for( CIT_WeaponInfos_Vector it = vWeapons_.begin(); it != vWeapons_.end(); ++it )
    {
        std::string name( (*it)->strName_.GetData() );
        bool ok = false;
        for( CIT_CategoryInfos_Vector itc = resources_.categories_.begin(); itc != resources_.categories_.end(); ++itc )
        {
            std::string resourcename = (*itc)->ptrCategory_.GetData()->strName_.GetData();
            if( name.find( resourcename ) != std::string::npos )
            {
                ok = true;
                break;
            }
        }
        if( !ok )
        {
            std::stringstream str;
            str << strName_ << " lacks ammo: " << name;
            MT_LOG_ERROR_MSG( str.str() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::IsValidDatabase
// Created: PHC 2011-01-20
// -----------------------------------------------------------------------------
bool ADN_Composantes_Data::ComposanteInfos::IsValidDatabase() const
{
    if( attritionBreakdowns_.vBreakdowns_.empty() || randomBreakdowns_.vBreakdowns_.empty() )
        if( ptrArmor_.GetData()->nType_.GetData() != eProtectionType_Human )
            return ADN_GuiTools::MissingBreakdownWarning( strName_.GetData() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::IsValidDatabase
// Created: PHC 2011-01-20
// -----------------------------------------------------------------------------
bool ADN_Composantes_Data::IsValidDatabase() const
{
    for( CIT_ComposanteInfos_Vector it = vComposantes_.begin(); it != vComposantes_.end(); ++it )
        if( !(*it)->IsValidDatabase() )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ComposanteInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ComposanteInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "equipment" )
               << xml::attribute( "comment", strAdditionalComments_ )
               << xml::attribute( "name", strName_ )
               << xml::attribute( "id", nMosId_ )
               << xml::attribute( "codeEMAT6", strCodeEMAT6_ )
               << xml::attribute( "codeEMAT8", strCodeEMAT8_ )
               << xml::attribute( "codeLFRIL", strCodeLFRIL_ )
               << xml::attribute( "codeNNO",   strCodeNNO_   )
               << xml::attribute( "protection", ptrArmor_.GetData()->strName_ )
               << xml::attribute( "size", *ptrSize_.GetData() )
               << xml::attribute( "weight", rWeight_ );

    output << xml::start( "speeds" )
            << xml::attribute( "max", rMaxSpeed_ );
    for( CIT_SpeedInfos_Vector itSpeed = vSpeeds_.begin(); itSpeed != vSpeeds_.end(); ++itSpeed )
        (*itSpeed)->WriteArchive( output );
    output << xml::end;

    output << xml::start( "composition" );
    resources_.WriteArchive( output );
    output << xml::end;

    output << xml::start( "sensors" );
    for( CIT_SensorInfos_Vector itSensor = vSensors_.begin(); itSensor != vSensors_.end(); ++itSensor )
        (*itSensor)->WriteArchive( output );
    output << xml::end;

    output << xml::start( "radars" );
    for( CIT_RadarInfos_Vector itRadar = vRadars_.begin(); itRadar != vRadars_.end(); ++itRadar )
        (*itRadar)->WriteArchive( output );
    output << xml::end;

    output << xml::start( "transports" );
    if( bTroopEmbarkingTimes_.GetData() )
        output << xml::start( "crew" )
                << xml::attribute( "man-boarding-time", embarkingTimePerPerson_ )
                << xml::attribute( "man-unloading-time", disembarkingTimePerPerson_ )
            << xml::end;
    if( bCanCarryCargo_.GetData() )
        output << xml::start( "unit" )
                << xml::attribute( "capacity", rWeightTransportCapacity_ )
                << xml::attribute( "ton-loading-time", embarkingTimePerTon_ )
                << xml::attribute( "ton-unloading-time", disembarkingTimePerTon_ )
               << xml::end;
    if( bCanCarryCrowd_.GetData() )
        output << xml::start( "crowd" )
                << xml::attribute( "capacity", nCrowdTransportCapacity_ )
                << xml::attribute( "man-boarding-time", crowdEmbarkingTimePerPerson_ )
                << xml::attribute( "man-unloading-time", crowdDisembarkingTimePerPerson_ )
               << xml::end;
    output << xml::end;

    consumptions_.WriteArchive( output );

    output << xml::start( "weapon-systems" );
    for( CIT_WeaponInfos_Vector itWeapon = vWeapons_.begin(); itWeapon != vWeapons_.end(); ++itWeapon )
        (*itWeapon)->WriteArchive( output );
    output << xml::end;

    if( ! vActiveProtections_.empty() )
    {
        output << xml::start( "active-protections" );
        for( CIT_ActiveProtectionsInfos_Vector itActiveProtections = vActiveProtections_.begin(); itActiveProtections != vActiveProtections_.end(); ++itActiveProtections )
            (*itActiveProtections)->WriteArchive( output );
        output << xml::end;
    }

    if ( nPowerDirectFire_ != 0 || nPowerIndirectFire_ != 0 || nPowerCloseCombat_ != 0 || nPowerEngineering_ != 0 )
    {
        output << xml::start( "power-indicators" )
                 << xml::attribute( "direct-fire", nPowerDirectFire_ )
                 << xml::attribute( "indirect-fire", nPowerIndirectFire_ )
                 << xml::attribute( "close-combat", nPowerCloseCombat_ )
                 << xml::attribute( "engineering", nPowerEngineering_ )
               << xml::end;
    }

    humanProtections_.WriteArchive( output );

    output << xml::start( "objects" );
    for( CIT_ObjectInfos_Vector itObject = vObjects_.begin(); itObject != vObjects_.end(); ++itObject )
        (*itObject)->WriteArchive( output );
    output << xml::end;

    logInfos_.WriteArchive( output );

    if( ! attritionBreakdowns_.vBreakdowns_.empty() && !randomBreakdowns_.vBreakdowns_.empty() )
    {
        output << xml::start( "breakdowns" );
        randomBreakdowns_.WriteArchive( output );
        attritionBreakdowns_.WriteArchive( output );
        output << xml::end;
    }
    else if( !IsValidDatabase() )
        throw ADN_DataException( tools::translate( "Composante_Data", "Missing breakdown" ).ascii(), tools::translate( "Composante_Data", "Equipment - Shall contain at least one breakdown for " ).ascii() + strName_.GetData() );
    if( bMaxSlope_.GetData() )
        output << xml::attribute( "max-slope", rMaxSlope_.GetData() / 100.0 );

    if( !equipmentCategory_.GetData().empty() )
        output << xml::content( "equipment-category", equipmentCategory_ );

    if( !( strNativeCountry_.GetData().empty() && strStartingCountry_.GetData().empty() && strStartingDate_.GetData().empty() && strInformationOrigin_.GetData().empty() ) )
    {
        output << xml::start( "operational-information" );
        if( !strNativeCountry_.GetData().empty() )
            output << xml::attribute( "native-country", strNativeCountry_ );
        if( !strStartingCountry_.GetData().empty() )
            output << xml::attribute( "starting-country", strStartingCountry_ );
        if( !strStartingDate_.GetData().empty() )
            output << xml::attribute( "starting-date", strStartingDate_ );
        if( !strInformationOrigin_.GetData().empty() )
            output << xml::attribute( "information-origin", strInformationOrigin_ );
        output << xml::end;
    }

    output << xml::end;
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
    vComposantes_.SetItemTypeName( "an equipment" );
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
    files.push_back(ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szComponents_.GetData());
}

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::Reset
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
void ADN_Composantes_Data::Reset()
{
    nNextId_ = 1;
    vComposantes_.Delete();
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ReadElement
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ReadElement( xml::xistream& input )
{
    std::auto_ptr<ComposanteInfos> spNew( new ComposanteInfos() );
    spNew->ReadArchive( input );
    vComposantes_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::ReadArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "equipments" )
            >> xml::list( "equipment", *this, &ADN_Composantes_Data::ReadElement )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::WriteArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Composantes_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "equipments" );
    ADN_Tools::AddSchema( output, "Equipments" );
    for( CIT_ComposanteInfos_Vector it = vComposantes_.begin(); it != vComposantes_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
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
QStringList ADN_Composantes_Data::GetComposantesThatUse( ADN_Sensors_Data::SensorInfos& sensor )
{
    QStringList result;
    for( IT_ComposanteInfos_Vector it = vComposantes_.begin(); it != vComposantes_.end(); ++it )
    {
        ComposanteInfos* pComp = *it;
        for( IT_SensorInfos_Vector it2 = pComp->vSensors_.begin(); it2 != pComp->vSensors_.end(); ++it2 )
            if( (*it2)->ptrSensor_.GetData() == &sensor )
            {
                result << pComp->strName_.GetData().c_str();
                break;
            }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::GetComposantesThatUse
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
QStringList ADN_Composantes_Data::GetComposantesThatUse( ADN_Weapons_Data::WeaponInfos& weapon )
{
    QStringList result;
    for( IT_ComposanteInfos_Vector it = vComposantes_.begin(); it != vComposantes_.end(); ++it )
    {
        ComposanteInfos* pComp = *it;
        for( IT_WeaponInfos_Vector it2 = pComp->vWeapons_.begin(); it2 != pComp->vWeapons_.end(); ++it2 )
            if( (*it2)->ptrWeapon_.GetData() == &weapon )
            {
                result << pComp->strName_.GetData().c_str();
                break;
            }
    }
    return result;
}
