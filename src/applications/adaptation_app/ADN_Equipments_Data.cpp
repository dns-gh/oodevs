// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_Data.h"
#include "ADN_ConsistencyChecker.h"
#include "ADN_Categories_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Objects_Data.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"
#include "clients_kernel/XmlTranslations.h"
#include "MT_Tools/MT_Tools_Types.h"
#include "protocol/Simulation.h"

tools::IdManager ADN_Equipments_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::AmbulanceInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Equipments_Data::AmbulanceInfos::AmbulanceInfos()
    : transportSkills_()
    , bTransportNBC_( false )
    , bTransportShock_( false )
    , nCapacity_( 1 )
    , loadTimePerPerson_( "0s" )
    , unloadTimePerPerson_( "0s" )
{
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        transportSkills_[ n ] = false;
}

// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::AmbulanceInfos::CopyFrom( AmbulanceInfos& src )
{
    nCapacity_ = src.nCapacity_.GetData();
    loadTimePerPerson_ = src.loadTimePerPerson_.GetData();
    unloadTimePerPerson_ = src.unloadTimePerPerson_.GetData();
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        transportSkills_[ n ] = src.transportSkills_[ n ].GetData();
    bTransportNBC_ = src.bTransportNBC_.GetData();
    bTransportShock_ = src.bTransportShock_.GetData();
}

// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::AmbulanceInfos::ReadArchive( xml::xistream& input )
{
    std::string woundedTransport;
    input >> xml::attribute( "capacity", nCapacity_ )
          >> xml::attribute( "man-loading-time", loadTimePerPerson_ )
          >> xml::attribute( "man-unloading-time", unloadTimePerPerson_ )
          >> xml::optional >> xml::attribute( "wounded-transport", woundedTransport )
          >> xml::optional >> xml::attribute( "nbc-transport", bTransportNBC_ )
          >> xml::optional >> xml::attribute( "reac-mental-transport", bTransportShock_ );

    for( int n = 0; n < eNbrDoctorSkills; ++n )
    {
        const std::string skill = ADN_Tr::ConvertFromDoctorSkills( static_cast< E_DoctorSkills >( n ) );
        transportSkills_[ n ] = woundedTransport.find( skill ) != std::string::npos;
    }
}

// -----------------------------------------------------------------------------
// Name: AmbulanceInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::AmbulanceInfos::WriteArchive( const std::string& section, xml::xostream& output ) const
{
    std::string woundedTransports;
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        if( transportSkills_[ n ].GetData() )
        {
            if( !woundedTransports.empty() )
                woundedTransports += ", ";
            woundedTransports += ADN_Tr::ConvertFromDoctorSkills( static_cast< E_DoctorSkills >( n ) );
        }
    output << xml::start( section )
            << xml::attribute( "capacity", nCapacity_ )
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
ADN_Equipments_Data::LogHealthInfos::LogHealthInfos()
    : bIsAmbulance_( false )
    , bIsAmbulanceReleve_( false )
    , bIsDoctor_( false )
    , bIsSortingDoctor_( false )
    , bIsCuringDoctor_( false )
    , bCuresNBC_( false )
    , bCuresShock_( false )
{
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        doctorSkills_[ n ] = false;
}

// -----------------------------------------------------------------------------
// Name: LogHealthInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogHealthInfos::CopyFrom( LogHealthInfos& src )
{
    bIsAmbulance_ = src.bIsAmbulance_.GetData();
    bIsAmbulanceReleve_ = src.bIsAmbulanceReleve_.GetData();
    bIsDoctor_ = src.bIsDoctor_.GetData();
    bIsSortingDoctor_ = src.bIsSortingDoctor_.GetData();
    bIsCuringDoctor_ = src.bIsCuringDoctor_.GetData();
    bCuresNBC_ = src.bCuresNBC_.GetData();
    bCuresShock_ = src.bCuresShock_.GetData();
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        doctorSkills_[ n ] = src.doctorSkills_[ n ].GetData();
    ambulanceInfos_.CopyFrom( src.ambulanceInfos_ );
    ambulanceReleveInfos_.CopyFrom( src.ambulanceReleveInfos_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::LogHealthInfos::ReadInfo
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogHealthInfos::ReadInfo( const std::string& type, xml::xistream& input )
{
    if( type == "collecting" )
    {
        bIsAmbulance_ = true;
        ambulanceInfos_.ReadArchive( input );
    }
    else if( type == "relieving" )
    {
        bIsAmbulanceReleve_ = true;
        ambulanceReleveInfos_.ReadArchive( input );
    }
    else if( type == "caring" )
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
            const std::string skill = ADN_Tr::ConvertFromDoctorSkills( static_cast< E_DoctorSkills >( n ) );
            doctorSkills_[ n ] = caring.find( skill ) != std::string::npos;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogHealthInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogHealthInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( *this, &ADN_Equipments_Data::LogHealthInfos::ReadInfo );
}

// -----------------------------------------------------------------------------
// Name: LogHealthInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogHealthInfos::WriteArchive( xml::xostream& output ) const
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
                if( doctorSkills_[ n ].GetData() )
                {
                    if( ! caring.empty() )
                        caring += ", ";
                    caring += ADN_Tr::ConvertFromDoctorSkills( static_cast< E_DoctorSkills >( n ) );
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
ADN_Equipments_Data::NTIInfos::NTIInfos( const std::string& strName )
    : strName_( strName )
    , bIsPresent_( false )
    , bHasMaxRepairTime_( false )
    , maxRepairTime_( "0s" )
    , bCanRepairEA_( false )
    , bCanRepairM_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NTIInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::NTIInfos::CopyFrom( NTIInfos& src )
{
    bIsPresent_ = src.bIsPresent_.GetData();
    bHasMaxRepairTime_ = src.bHasMaxRepairTime_.GetData();
    maxRepairTime_ = src.maxRepairTime_.GetData();
    bCanRepairEA_ = src.bCanRepairEA_.GetData();
    bCanRepairM_ = src.bCanRepairM_.GetData();
}

namespace
{

bool HasMaxRepairTime( const ADN_Type_Time& time )
{
    return time != "0s";
}

} // namespace

// -----------------------------------------------------------------------------
// Name: NTIInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::NTIInfos::ReadArchive( xml::xistream& input )
{
    bIsPresent_ = true;
    std::string type;
    input >> xml::optional >> xml::attribute( "max-reparation-time", maxRepairTime_ )
          >> xml::optional >> xml::attribute( "type", type );

    bHasMaxRepairTime_ = HasMaxRepairTime( maxRepairTime_ );
    bCanRepairEA_ = type.find( "EA" ) != std::string::npos;
    bCanRepairM_  = type.find( "M"  ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: NTIInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::NTIInfos::WriteArchive( xml::xostream& output ) const
{
    if( ! bIsPresent_.GetData() )
        return;
    output << xml::start( "repairing" );
    output << xml::attribute( "category", strName_ );
    if( bHasMaxRepairTime_.GetData() && HasMaxRepairTime( maxRepairTime_ ) )
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
// Name: NTIInfos::IsTypeValid
// Created: MMC 2013-04-02
// -----------------------------------------------------------------------------
bool ADN_Equipments_Data::NTIInfos::IsTypeValid() const
{
    if( bIsPresent_.GetData() && !bCanRepairEA_.GetData() && !bCanRepairM_.GetData() )
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::LogMaintenanceInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Equipments_Data::LogMaintenanceInfos::LogMaintenanceInfos()
    : bIsTower_( false )
    , rCapacity_( 0 )
    , loadTime_( "0s" )
    , unloadTime_( "0s" )
    , NTI1Infos_( "NTI1" )
    , NTI2Infos_( "NTI2" )
    , NTI3Infos_( "NTI3" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogMaintenanceInfos::CopyFrom( LogMaintenanceInfos& src )
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
// Name: ADN_Equipments_Data::LogMaintenanceInfos::ReadInfo
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogMaintenanceInfos::ReadInfo( const std::string& type, xml::xistream& input )
{
    if( type == "towing" )
    {
        bIsTower_ = true;
        input >> xml::attribute( "capacity", rCapacity_ )
              >> xml::attribute( "loading-time", loadTime_ )
              >> xml::attribute( "unloading-time", unloadTime_ );
    }
    else if( type == "repairing" )
    {
        std::string category = input.attribute< std::string >( "category" );
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
void ADN_Equipments_Data::LogMaintenanceInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( *this, &ADN_Equipments_Data::LogMaintenanceInfos::ReadInfo );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogMaintenanceInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "maintenance-functions" );
    if( bIsTower_.GetData() && rCapacity_.GetData() > 0 )
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
// Name: LogMaintenanceInfos::IsRepairTypeValid
// Created: MMC 2013-04-02
// -----------------------------------------------------------------------------
bool ADN_Equipments_Data::LogMaintenanceInfos::IsRepairTypeValid() const
{
    if( !NTI1Infos_.IsTypeValid() || !NTI2Infos_.IsTypeValid() || !NTI3Infos_.IsTypeValid() )
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::LogSupplyInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Equipments_Data::LogSupplyInfos::LogSupplyInfos()
    : ptr_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Natures_Data >( eNatures ).GetNaturesInfos(), 0 )
    , rMinWeight_( 0 )
    , rMaxWeight_( 0 )
    , rMinVolume_( 0 )
    , rMaxVolume_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogSupplyInfos::CopyFrom( LogSupplyInfos& src )
{
    rMinWeight_ = src.rMinWeight_.GetData();
    rMaxWeight_ = src.rMaxWeight_.GetData();
    rMinVolume_ = src.rMinVolume_.GetData();
    rMaxVolume_ = src.rMaxVolume_.GetData();
    ptr_ = src.ptr_.GetData();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogSupplyInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::optional
          >> xml::start( "carrying" )
            >> xml::attribute( "nature", ptr_ )
            >> xml::attribute( "min-mass", rMinWeight_ )
            >> xml::attribute( "max-mass", rMaxWeight_ )
            >> xml::attribute( "min-volume", rMinVolume_ )
            >> xml::attribute( "max-volume", rMaxVolume_ )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogSupplyInfos::WriteArchive( xml::xostream& output ) const
{
    if( ptr_.GetData() )
    {
        output << xml::start( "supply-functions" )
                   << xml::start( "carrying" )
                       << xml::attribute( "nature", ptr_ )
                       << xml::attribute( "min-mass", rMinWeight_ )
                       << xml::attribute( "max-mass", rMaxWeight_ )
                       << xml::attribute( "min-volume", rMinVolume_ )
                       << xml::attribute( "max-volume", rMaxVolume_ )
                   << xml::end
               << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: LogInfos::LogInfos
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_Equipments_Data::LogInfos::LogInfos()
    : bHasHealthInfos_     ( false )
    , bHasMaintenanceInfos_( false )
    , bHasSupplyInfos_     ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogInfos::CopyFrom
// Created: APE 2005-03-24
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogInfos::CopyFrom( LogInfos& src )
{
    bHasHealthInfos_ = src.bHasHealthInfos_.GetData();
    bHasMaintenanceInfos_ = src.bHasMaintenanceInfos_.GetData();
    bHasSupplyInfos_ = src.bHasSupplyInfos_.GetData();

    healthInfos_.CopyFrom( src.healthInfos_ );
    maintenanceInfos_.CopyFrom( src.maintenanceInfos_ );
    supplyInfos_.CopyFrom( src.supplyInfos_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::LogInfos::ReadLogisticFunction
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogInfos::ReadLogisticFunction( const std::string& type, xml::xistream& input )
{
    if( type == "supply-functions" )
    {
        bHasSupplyInfos_ = true;
        supplyInfos_.ReadArchive( input );
    }
    else if( type == "health-functions" )
    {
        bHasHealthInfos_ = true;
        healthInfos_.ReadArchive( input );
    }
    else if( type == "maintenance-functions" )
    {
        bHasMaintenanceInfos_ = true;
        maintenanceInfos_.ReadArchive( input );
    }
}

// -----------------------------------------------------------------------------
// Name: LogInfos::ReadArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::optional
            >> xml::start( "logistic-functions" )
                >> xml::list( *this, &ADN_Equipments_Data::LogInfos::ReadLogisticFunction )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: LogInfos::WriteArchive
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::LogInfos::WriteArchive( xml::xostream& output ) const
{
    if( ! bHasHealthInfos_.GetData() && ! bHasMaintenanceInfos_.GetData() && ! bHasSupplyInfos_.GetData() )
        return;

    output << xml::start( "logistic-functions" );
    if( bHasMaintenanceInfos_.GetData() )
        maintenanceInfos_.WriteArchive( output );
    if( bHasHealthInfos_.GetData() )
        healthInfos_.WriteArchive( output );
    if( bHasSupplyInfos_.GetData() )
        supplyInfos_.WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: LogInfos::IsRepairTypeValid
// Created: MMC 2013-04-02
// -----------------------------------------------------------------------------
bool ADN_Equipments_Data::LogInfos::IsRepairTypeValid() const
{
    if( !bHasMaintenanceInfos_.GetData() )
        return true;
    return maintenanceInfos_.IsRepairTypeValid();
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfos::BreakdownInfos
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Equipments_Data::BreakdownInfos::BreakdownInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().GetBreakdowns(), 0, true )
    , rPercentage_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfos::CreateCopy
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Equipments_Data::BreakdownInfos* ADN_Equipments_Data::BreakdownInfos::CreateCopy()
{
    BreakdownInfos* pCopy = new BreakdownInfos();
    pCopy->SetCrossedElement( GetCrossedElement() );
    pCopy->rPercentage_ = rPercentage_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfos::ReadArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::BreakdownInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "type", *this )
          >> xml::attribute( "percentage", rPercentage_ );
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfos::WriteArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::BreakdownInfos::WriteArchive( const std::string& origin, xml::xostream& output ) const
{
    output << xml::start( "breakdown" )
            << xml::attribute( "type", *this )
            << xml::attribute( "origin", origin )
            << xml::attribute( "percentage", rPercentage_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::BreakdownGroupInfos
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Equipments_Data::BreakdownGroupInfos::BreakdownGroupInfos( const std::string& strName )
    : strName_( strName )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::~BreakdownGroupInfos
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Equipments_Data::BreakdownGroupInfos::~BreakdownGroupInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::Contains
// Created: LGY 2012-05-29
// -----------------------------------------------------------------------------
bool ADN_Equipments_Data::BreakdownGroupInfos::Contains( ADN_Breakdowns_Data::BreakdownInfo& breakdown ) const
{
    for( auto it = vBreakdowns_.begin(); it != vBreakdowns_.end(); ++it )
        if( ( *it )->GetCrossedElement() && ( *it )->GetCrossedElement()->strName_.GetData() == breakdown.strName_.GetData() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::CopyFrom
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::BreakdownGroupInfos::CopyFrom( BreakdownGroupInfos& src )
{
    assert( strName_ == src.strName_ );
    for( auto it = src.vBreakdowns_.begin(); it != src.vBreakdowns_.end(); ++it )
        vBreakdowns_.AddItem( ( *it )->CreateCopy() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::BreakdownGroupInfos::ReadBreakdown
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::BreakdownGroupInfos::ReadBreakdown( xml::xistream& input, const std::string& parentName )
{
    if( input.attribute< std::string >( "origin" ) == strName_ )
    {
        std::unique_ptr< BreakdownInfos > spNew( new BreakdownInfos() );
        spNew->ReadArchive( input );
        if( spNew->GetCrossedElement() )
            vBreakdowns_.AddItem( spNew.release() );
        else
            ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, parentName, eEquipments, -1, tools::translate( "ADN_Automata_Data", "Breakdowns" ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::ReadArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::BreakdownGroupInfos::ReadArchive( xml::xistream& input, const std::string& parentName )
{
    input >> xml::optional
          >> xml::start( "breakdowns" )
            >> xml::list( "breakdown", *this, &ADN_Equipments_Data::BreakdownGroupInfos::ReadBreakdown, parentName )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: BreakdownGroupInfos::WriteArchive
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::BreakdownGroupInfos::WriteArchive( xml::xostream& output ) const
{
    for( auto it = vBreakdowns_.begin(); it != vBreakdowns_.end() ; ++it )
        ( *it )->WriteArchive( strName_, output );
}

//-----------------------------------------------------------------------------
// Name: SpeedInfos::SpeedInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Equipments_Data::SpeedInfos::SpeedInfos( E_Location nTypeTerrain )
    : nTypeTerrain_( nTypeTerrain )
    , rSpeed_( 0 )
    , nConstruction_( 100 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpeedInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::SpeedInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rSpeed_ )
          >> xml::optional >> xml::attribute( "construction-speed", nConstruction_ );
}

// -----------------------------------------------------------------------------
// Name: SpeedInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::SpeedInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "speed" )
            << xml::attribute( "terrain", ENT_Tr::ConvertFromLocation( nTypeTerrain_ ) )
            << xml::attribute( "value", rSpeed_ );
    if( nConstruction_ != 100 )
        output << xml::attribute( "construction-speed", nConstruction_ );
    output << xml::end;
}

//-----------------------------------------------------------------------------
// Name: SensorInfos::SensorInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Equipments_Data::SensorInfos::SensorInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsInfos(), 0, true )
    , rHeight_(0)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SensorInfos::CreateCopy
// Created: APE 2004-11-29
// -----------------------------------------------------------------------------
ADN_Equipments_Data::SensorInfos* ADN_Equipments_Data::SensorInfos::CreateCopy()
{
    SensorInfos* pCopy = new SensorInfos();
    pCopy->SetCrossedElement( GetCrossedElement() );
    pCopy->rHeight_ = rHeight_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: SensorInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::SensorInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "type", *this )
          >> xml::attribute( "height", rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: SensorInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::SensorInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "sensor" )
             << xml::attribute( "type", *this )
             << xml::attribute( "height", rHeight_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::RadarInfos
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Equipments_Data::RadarInfos::RadarInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_->vRadars_, 0 , true )
    , rHeight_( 10 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::CreateCopy
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Equipments_Data::RadarInfos* ADN_Equipments_Data::RadarInfos::CreateCopy()
{
    RadarInfos* pCopy = new RadarInfos();
    pCopy->SetCrossedElement( GetCrossedElement() );
    pCopy->rHeight_ = rHeight_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::ReadArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::RadarInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "type", *this )
          >> xml::optional >> xml::attribute( "height", rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::WriteArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::RadarInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "radar" )
             << xml::attribute( "type", *this )
             << xml::attribute( "height", rHeight_ )
           << xml::end;
}

//-----------------------------------------------------------------------------
// Name: WeaponInfos::WeaponInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Equipments_Data::WeaponInfos::WeaponInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos(), 0, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeaponInfos::WeaponInfos
// Created: APE 2005-01-24
// -----------------------------------------------------------------------------
ADN_Equipments_Data::WeaponInfos::WeaponInfos( ADN_Weapons_Data_WeaponInfos& weapon )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos(), &weapon, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeaponInfos::CreateCopy
// Created: APE 2004-11-29
// -----------------------------------------------------------------------------
ADN_Equipments_Data::WeaponInfos* ADN_Equipments_Data::WeaponInfos::CreateCopy()
{
    WeaponInfos* pCopy = new WeaponInfos();
    pCopy->SetCrossedElement( GetCrossedElement() );
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: WeaponInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::WeaponInfos::ReadArchive( xml::xistream& input )
{
    std::string strLauncher, strAmmunition;
    input >> xml::attribute( "launcher", strLauncher )
          >> xml::attribute( "munition", strAmmunition );
    ADN_Weapons_Data_WeaponInfos* pWeapon = ADN_Workspace::GetWorkspace().GetWeapons().GetData().FindWeapon( strLauncher, strAmmunition );
    SetCrossedElement( pWeapon );
}

// -----------------------------------------------------------------------------
// Name: WeaponInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::WeaponInfos::WriteArchive( xml::xostream& output ) const
{
    if( GetCrossedElement() )
    {
        output << xml::start( "weapon-system" )
                << xml::attribute( "launcher", GetCrossedElement()->ptrLauncher_ )
                << xml::attribute( "munition", GetCrossedElement()->ptrAmmunition_ )
            << xml::end;
    }
}

//-----------------------------------------------------------------------------
// Name: ActiveProtectionsInfos::ActiveProtectionsInfos
// Created: FDS 10-02-24
//-----------------------------------------------------------------------------
ADN_Equipments_Data::ActiveProtectionsInfos::ActiveProtectionsInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsInfos(), 0, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActiveProtectionsInfos::ActiveProtectionsInfos
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_Equipments_Data::ActiveProtectionsInfos::ActiveProtectionsInfos( ADN_ActiveProtections_Data::ActiveProtectionsInfos& activeProtections )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsInfos(), &activeProtections, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActiveProtectionsInfos::CreateCopy
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_Equipments_Data::ActiveProtectionsInfos* ADN_Equipments_Data::ActiveProtectionsInfos::CreateCopy()
{
    ActiveProtectionsInfos* pCopy = new ActiveProtectionsInfos();
    pCopy->SetCrossedElement( GetCrossedElement() );
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ActiveProtectionsInfos::ReadArchive
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ActiveProtectionsInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::WriteArchive
// Created: LDC 2010-01-12
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ActiveProtectionsInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "protection" )
            << xml::attribute( "name", *this )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfos::CategoryInfos
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipments_Data::CategoryInfos::CategoryInfos( ADN_Resources_Data::ResourceInfos& parentDotation )
    : ADN_CrossedRef( parentDotation.categories_, 0, true )
    , ptrDotation_( &parentDotation )
    , rNormalizedConsumption_( 0 )
    , rLogLowThreshold_( 0 )
    , rLogHighThreshold_( 0 )
    , rNbr_ ( 0 )
{
    // NOGHING
}

// -----------------------------------------------------------------------------
// Name: CategoryInfos::CreateCopy
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipments_Data::CategoryInfos* ADN_Equipments_Data::CategoryInfos::CreateCopy()
{
    CategoryInfos* pCopy = new CategoryInfos( *ptrDotation_.GetData() );
    pCopy->SetCrossedElement( GetCrossedElement() );
    pCopy->ptrDotation_ = ptrDotation_.GetData();
    pCopy->rNormalizedConsumption_ = rNormalizedConsumption_.GetData();
    pCopy->rNbr_ = rNbr_.GetData();
    pCopy->rLogLowThreshold_ = rLogLowThreshold_.GetData();
    pCopy->rLogHighThreshold_ = rLogHighThreshold_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfos::ReadArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::CategoryInfos::ReadArchive( xml::xistream& input, const std::string& parentName )
{
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "capacity", rNbr_ )
          >> xml::attribute( "low-threshold", rLogLowThreshold_ )
          >> xml::attribute( "high-threshold", rLogHighThreshold_ )
          >> xml::optional >> xml::attribute( "normalized-consumption", rNormalizedConsumption_ );
    if( rLogLowThreshold_.GetData() > rLogHighThreshold_.GetData() )
        ADN_ConsistencyChecker::AddLoadingError( eRepartitionError, parentName, eEquipments, -1,
            tools::translate( "ADN_Equipments_Data", "Invalid thresholds, high \'%1\' must be superior than low \'%2\' and will be set equal to low." )
                .arg( rLogHighThreshold_.GetData() ).arg( rLogLowThreshold_.GetData() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfos::WriteArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::CategoryInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "resource" )
             << xml::attribute( "name", *this )
             << xml::attribute( "capacity", rNbr_ )
             << xml::attribute( "low-threshold", rLogLowThreshold_ )
             << xml::attribute( "high-threshold", rLogHighThreshold_ )
             << xml::attribute( "normalized-consumption", rNormalizedConsumption_ )
           << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ResourceInfos::ResourceInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Equipments_Data::ResourceInfos::ResourceInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::CreateCopy
// Created: APE 2004-11-29
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ResourceInfos::CopyFrom( ADN_Equipments_Data::ResourceInfos& src )
{
    for( auto it = src.categories_.begin(); it != src.categories_.end(); ++it )
        categories_.AddItem( ( *it )->CreateCopy() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::ResourceInfos::ReadCategory
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ResourceInfos::ReadCategory( xml::xistream& input, const std::string& parentName )
{
    auto family = ENT_Tr::ConvertToDotationType( input.attribute< std::string >( "name" ), ENT_Tr::eToSim );
    ADN_Resources_Data::ResourceInfos& dotation = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( family );

    input >> xml::list( "resource", *this, &ADN_Equipments_Data::ResourceInfos::ReadDotation, dotation, parentName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::ResourceInfos::ReadDotation
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ResourceInfos::ReadDotation( xml::xistream& input, ADN_Resources_Data::ResourceInfos& dotation, const std::string& parentName )
{
    std::unique_ptr< CategoryInfos > pInfo( new CategoryInfos( dotation ) );
    pInfo->ReadArchive( input, parentName );
    if( pInfo->GetCrossedElement() )
        categories_.AddItem( pInfo.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, parentName, eEquipments, -1, tools::translate( "ADN_Equipments_Data", "Supplies" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ResourceInfos::ReadArchive( xml::xistream& input, const std::string& parentName )
{
    input >> xml::list( "category", *this, &ADN_Equipments_Data::ResourceInfos::ReadCategory, parentName );
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ResourceInfos::WriteArchive( xml::xostream& output ) const
{
    for( uint n = sword::DotationType_MIN; n <= sword::DotationType_MAX; ++n )
    {
        bool entered = false;
        for( auto it = categories_.begin(); it != categories_.end(); ++it )
            if( ( *it )->ptrDotation_.GetData() && ( *it )->ptrDotation_.GetData()->nType_ == static_cast< sword::DotationType >( n ) )
            {
                if( !entered )
                {
                    output << xml::start( "category" )
                            << xml::attribute( "name", ( *it )->ptrDotation_ );
                    entered = true;
                }
                ( *it )->WriteArchive( output );
            }
        if( entered )
            output << xml::end;
    }
}

//-----------------------------------------------------------------------------
// Name: ObjectInfos::ObjectInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Equipments_Data::ObjectInfos::ObjectInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(), 0, true )
    , initialBuildTime_( "-1s" )
    , initialDestructionTime_( "-1s" )
    , coeffBuildTime_( "-1s" )
    , coeffDestructionTime_( "-1s" )
    , valorizationTime_( "-1s" )
    , devalorizationTime_( "-1s" )
    , extinguishingTime_( "-1s" )
    , rCoeffCirc_( 0 )
    , rSpeedCirc_( 0 )
    , rSpeedNotCirc_( 0 )
    , bInitialBuildTime_( false )
    , bInitialDestructionTime_( false )
    , bCoeffBuildTime_( false )
    , bCoeffDestructionTime_( false )
    , bValorizationTime_( false )
    , bDevalorizationTime_( false )
    , bExtinguishingTime_( false )
    , bCoeffCircTime_( false )
    , bSpeedCirc_( false )
    , bSpeedNotCirc_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectInfos::CreateCopy
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipments_Data::ObjectInfos* ADN_Equipments_Data::ObjectInfos::CreateCopy()
{
    ObjectInfos* pCopy = new ObjectInfos();

    pCopy->SetCrossedElement( GetCrossedElement() );
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
void ADN_Equipments_Data::ObjectInfos::ReadArchive( xml::xistream& input )
{
    SetCrossedElement( ADN_Workspace::GetWorkspace().GetObjects().GetData().FindObject( input.attribute< std::string >( "type" ) ) );

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

    bInitialBuildTime_ = initialBuildTime_ != "-1s";
    bInitialDestructionTime_ = initialDestructionTime_ != "-1s";
    bCoeffBuildTime_ = coeffBuildTime_ != "-1s";
    bCoeffDestructionTime_ = coeffDestructionTime_ != "-1s";
    bValorizationTime_ = valorizationTime_ != "-1s";
    bDevalorizationTime_ = devalorizationTime_ != "-1s";
    bExtinguishingTime_ = extinguishingTime_ != "-1s";
    bCoeffCircTime_ = rCoeffCirc_ != 0;
    bSpeedCirc_ = rSpeedCirc_ != 0;
    bSpeedNotCirc_ = rSpeedNotCirc_ != 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectInfos::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ObjectInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "object" )
            << xml::attribute( "type", GetCrossedElement() ? GetCrossedElement()->strType_.GetData() : "" );
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
// Name: DisasterImpactInfos constructor
// Created: JSR 2014-04-22
// -----------------------------------------------------------------------------
ADN_Equipments_Data::DisasterImpactInfos::DisasterImpactInfos()
    : threshold_( 0 )
    , modifier_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterImpactInfos::CreateCopy
// Created: JSR 2014-04-22
// -----------------------------------------------------------------------------
ADN_Equipments_Data::DisasterImpactInfos* ADN_Equipments_Data::DisasterImpactInfos::CreateCopy()
{
    DisasterImpactInfos* copy = new DisasterImpactInfos;
    copy->threshold_ = threshold_.GetData();
    copy->modifier_ = modifier_.GetData();
    return copy;
}

// -----------------------------------------------------------------------------
// Name: DisasterImpactInfos::ReadArchive
// Created: JSR 2014-04-22
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::DisasterImpactInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "threshold", threshold_ )
          >> xml::attribute( "modifier", modifier_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterImpactInfos::WriteArchive
// Created: JSR 2014-04-22
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::DisasterImpactInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "disaster-impact" )
            << xml::attribute( "threshold", threshold_.GetData() )
            << xml::attribute( "modifier", modifier_.GetData() )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ConsumptionItem::ConsumptionItem
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
ADN_Equipments_Data::ConsumptionItem::ConsumptionItem( E_ConsumptionType nConsumptionType, const T_CategoryInfos_Vector& equipmentCategories, CategoryInfos* equipmentCategory )
    : ADN_CrossedRef( equipmentCategories, equipmentCategory, true )
    , nConsumptionType_( nConsumptionType )
    , nQuantityUsedPerHour_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsumptionItem::CreateCopy
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipments_Data::ConsumptionItem* ADN_Equipments_Data::ConsumptionItem::CreateCopy()
{
    ConsumptionItem* pCopy = new ConsumptionItem( nConsumptionType_, GetVector(), GetCrossedElement() );
    pCopy->nQuantityUsedPerHour_ = nQuantityUsedPerHour_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ConsumptionItem::ReadArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ConsumptionItem::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "value", nQuantityUsedPerHour_ );

    if( !GetCrossedElement() )
        return;
    for( auto it = GetVector().begin(); it != GetVector().end(); ++it )
    {
        if( ( *it )->GetCrossedElement() == GetCrossedElement()->GetCrossedElement() )
            return;
    }
    SetCrossedElement( 0 );
}

// -----------------------------------------------------------------------------
// Name: ConsumptionItem::WriteArchive
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ConsumptionItem::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "resource" )
             << xml::attribute( "name", *this );
    ADN_Resources_Data::CategoryInfo* infos = GetCrossedElement() ? GetCrossedElement()->GetCrossedElement() : 0;
    if( infos )
        output << xml::attribute( "category", infos->parentResource_.strName_.GetData() );
    //else
    //    output << xml::attribute( "category", "" );
    //output << xml::attribute( "category", infos ? infos->parentResource_.strName_.GetData() : "" )
    output << xml::attribute( "value", nQuantityUsedPerHour_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::ConsumptionsInfos
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
ADN_Equipments_Data::ConsumptionsInfos::ConsumptionsInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::CopyFrom
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ConsumptionsInfos::CopyFrom( ConsumptionsInfos& source )
{
    for( auto it = source.vConsumptions_.begin(); it != source.vConsumptions_.end(); ++it )
        vConsumptions_.AddItem( (*it)->CreateCopy() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::ConsumptionsInfos::ReadConsumption
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ConsumptionsInfos::ReadConsumption( xml::xistream& input, T_CategoryInfos_Vector& equipmentCategories, const std::string& parentName )
{
    std::string status;
    input >> xml::attribute( "status", status );
    E_ConsumptionType type = ADN_Tr::ConvertToConsumptionType( status );
    if( type == E_ConsumptionType( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "Equipments_Data",  "Equipment - Invalid activty '%1'" ).arg( status.c_str() ).toStdString() );
    input >> xml::list( "resource", *this, &ADN_Equipments_Data::ConsumptionsInfos::ReadDotation, type, equipmentCategories, parentName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::ConsumptionsInfos::ReadDotation
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ConsumptionsInfos::ReadDotation( xml::xistream& input, const E_ConsumptionType& type, T_CategoryInfos_Vector& equipmentCategories, const std::string& parentName )
{
    std::unique_ptr<ConsumptionItem> spNew( new ConsumptionItem( type, equipmentCategories, 0 ) );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vConsumptions_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, parentName, eEquipments, -1, tools::translate( "ADN_Automata_Data", "Consumptions" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::ReadArchive
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ConsumptionsInfos::ReadArchive( xml::xistream& input, T_CategoryInfos_Vector& equipmentCategories, const std::string& parentName )
{
    input >> xml::start( "consumptions" )
            >> xml::list( "consumption", *this, &ADN_Equipments_Data::ConsumptionsInfos::ReadConsumption, equipmentCategories, parentName )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::ConsumptionsInfos::FillMissingConsumptions
// Created: ABR 2012-12-03
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ConsumptionsInfos::FillMissingConsumptions( T_CategoryInfos_Vector& equipmentCategories )
{
    for( auto it = equipmentCategories.begin(); it != equipmentCategories.end(); ++it )
    {
        if( *it == 0 )
            continue;
        for( int i = 0; i < eNbrConsumptionType; ++i )
        {
            bool found = false;
            for( auto itConso = vConsumptions_.begin(); !found && itConso != vConsumptions_.end(); ++itConso )
            {
                if( ( *itConso )->nConsumptionType_ == i && ( *itConso )->GetCrossedElement() == ( *it ) )
                    found = true;
            }
            if( !found )
            {
                std::unique_ptr< ConsumptionItem > spNew( new ConsumptionItem( static_cast< E_ConsumptionType >( i ), equipmentCategories, *it ) );
                vConsumptions_.AddItem( spNew.release() );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ConsumptionsInfos::WriteArchive
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ConsumptionsInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "consumptions" );
    for( int nType = 0; nType < eNbrConsumptionType; ++nType )
    {
        bool entered = false;
        for( auto it = vConsumptions_.begin(); it != vConsumptions_.end(); ++it )
            if( (*it)->nConsumptionType_ == nType && (*it)->nQuantityUsedPerHour_.GetData() != 0. )
            {
                if( !entered )
                {
                    output << xml::start( "consumption" )
                            << xml::attribute( "status", ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)nType ) );
                    entered = true;
                }
                (*it)->WriteArchive( output );
            }
        if( entered )
            output << xml::end; // !consumption
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: AviationResourceQuotasInfos Constructor
// Created: JSR 2013-10-16
// -----------------------------------------------------------------------------
ADN_Equipments_Data::AviationResourceQuotasInfos::AviationResourceQuotasInfos( E_AviationRange nRange )
    : nRange_( nRange )
{
    for( int i = 0; i < eNbrAmmunitionType; ++i )
        resourceQuotas_[ i ] = ( nRange == eAviationRange_NoAmmunition ) ? 0 : 100;
    resourceQuotas_[ eNbrAmmunitionType ] = 100;
}

// -----------------------------------------------------------------------------
// Name: AviationResourceQuotasInfos::ReadArchive
// Created: JSR 2013-10-16
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::AviationResourceQuotasInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( "quota", *this, &ADN_Equipments_Data::AviationResourceQuotasInfos::ReadQuota );
}

// -----------------------------------------------------------------------------
// Name: AviationResourceQuotasInfos::ReadQuota
// Created: JSR 2013-10-18
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::AviationResourceQuotasInfos::ReadQuota( xml::xistream& input )
{
    const std::string category = input.attribute< std::string >( "category" );
    const unsigned int value = input.attribute< unsigned int >( "value" );
    try
    {
        if( category ==  ENT_Tr::ConvertFromDotationType( sword::dotation_type_fuel, ENT_Tr::eToSim ) )
            resourceQuotas_[ eNbrAmmunitionType ] = value;
        else
        {
            E_AmmunitionType type = ENT_Tr::ConvertToAmmunitionType( category );
            resourceQuotas_[ type ] = value;
        }
    }
    catch( const std::exception& /*e*/ )
    {
        throw MASA_EXCEPTION( tools::translate( "Equipments_Data", "Aviation range - Invalid category '%1'" ).arg( category.c_str() ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: AviationResourceQuotasInfos::WriteArchive
// Created: JSR 2013-10-16
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::AviationResourceQuotasInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "aviation-quota" )
            << xml::attribute( "range", ENT_Tr::ConvertFromAviationRange( nRange_ ) );
    for( int i = 0; i < eNbrAmmunitionType; ++i )
        output << xml::start( "quota" )
                << xml::attribute( "category", ENT_Tr::ConvertFromAmmunitionType( static_cast< E_AmmunitionType >( i ) ) )
                << xml::attribute( "value", resourceQuotas_[ i ] )
               << xml::end;
    output << xml::start( "quota" )
            << xml::attribute( "category", ENT_Tr::ConvertFromDotationType( sword::dotation_type_fuel, ENT_Tr::eToSim ) )
            << xml::attribute( "value", resourceQuotas_[ eNbrAmmunitionType ] )
           << xml::end;
    output << xml::end;
}

//=============================================================================
//
//=============================================================================

//-----------------------------------------------------------------------------
// Name: EquipmentInfos::EquipmentInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Equipments_Data::EquipmentInfos::EquipmentInfos()
    : nId_                           ( ADN_Equipments_Data::idManager_.GetNextId() )
    , equipmentCategory_             ( "Other" )
    , ptrArmor_                      ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos(), 0 )
    , ptrSize_                       ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos(), 0 )
    , rWeight_                       ( 100 )
    , vSpeeds_                       ( false )
    , bAviationResourcesQuotas_      ( false )
    , vAviationResourceQuotas_       ( false )
    , bTroopEmbarkingTimes_          ( false )
    , embarkingTimePerPerson_        ( "0s" )
    , disembarkingTimePerPerson_     ( "0s" )
    , bCanCarryCargo_                ( false )
    , rWeightTransportCapacity_      ( 0 )
    , embarkingTimePerTon_           ( "0s" )
    , disembarkingTimePerTon_        ( "0s" )
    , bCanTransportDestroyed_        ( false )
    , bCanCarryCrowd_                ( false )
    , nCrowdTransportCapacity_       ( 0 )
    , crowdEmbarkingTimePerPerson_   ( "0s" )
    , crowdDisembarkingTimePerPerson_( "0s" )
    , rMaxSpeed_                     ( 100 )
    , attritionBreakdowns_           ( "attrition" )
    , randomBreakdowns_              ( "random" )
    , nMaxSlope_                     ( 45 )
    , nSlopeDeceleration_            ( 0 )
    , length_                        ( 0. )
    , width_                         ( 0. )
    , frontSeparationDistance_       ( 0. )
    , safetyDistance_                ( 0. )
    , speedSafetyDistance_           ( 0. )
    , nPowerDirectFire_              ( 0 )
    , nPowerIndirectFire_            ( 0 )
    , nPowerCloseCombat_             ( 0 )
    , nPowerEngineering_             ( 0 )
{
    Initialize();
}

//-----------------------------------------------------------------------------
// Name: EquipmentInfos::EquipmentInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Equipments_Data::EquipmentInfos::EquipmentInfos( unsigned int id )
    : nId_                           ( id )
    , equipmentCategory_             ( "Other" )
    , ptrArmor_                      ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos(), 0 )
    , ptrSize_                       ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos(), 0 )
    , rWeight_                       ( 100 )
    , vSpeeds_                       ( false )
    , bAviationResourcesQuotas_      ( false )
    , vAviationResourceQuotas_       ( false )
    , bTroopEmbarkingTimes_          ( false )
    , embarkingTimePerPerson_        ( "0s" )
    , disembarkingTimePerPerson_     ( "0s" )
    , bCanCarryCargo_                ( false )
    , rWeightTransportCapacity_      ( 0 )
    , embarkingTimePerTon_           ( "0s" )
    , disembarkingTimePerTon_        ( "0s" )
    , bCanTransportDestroyed_        ( false )
    , bCanCarryCrowd_                ( false )
    , nCrowdTransportCapacity_       ( 0 )
    , crowdEmbarkingTimePerPerson_   ( "0s" )
    , crowdDisembarkingTimePerPerson_( "0s" )
    , rMaxSpeed_                     ( 100 )
    , attritionBreakdowns_           ( "attrition" )
    , randomBreakdowns_              ( "random" )
    , nMaxSlope_                     ( 45 )
    , nSlopeDeceleration_            ( 0 )
    , length_                        ( 0. )
    , width_                         ( 0. )
    , frontSeparationDistance_       ( 0. )
    , safetyDistance_                ( 0. )
    , speedSafetyDistance_           ( 0. )
    , nPowerDirectFire_              ( 0 )
    , nPowerIndirectFire_            ( 0 )
    , nPowerCloseCombat_             ( 0 )
    , nPowerEngineering_             ( 0 )
{
    Initialize();
    ADN_Equipments_Data::idManager_.Lock( id );
}

//-----------------------------------------------------------------------------
// Name: EquipmentInfos::EquipmentInfos
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_Equipments_Data::EquipmentInfos::~EquipmentInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::EquipmentInfos::Initialize
// Created: ABR 2012-09-18
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::Initialize()
{
    BindExistenceTo( &ptrArmor_ );
    BindExistenceTo( &ptrSize_ );

    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eEquipments, "equipments" ) );
    strAdditionalComments_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eEquipments, "comment" ) );
    strNativeCountry_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eEquipments, "native-country" ) );
    strInformationOrigin_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eEquipments, "information-origin" ) );
    equipmentCategory_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eEquipments, "equipment-category" ) );

    // initialize speeds
    for( int iTerrain = 0; iTerrain < eNbrLocation; ++iTerrain )
        vSpeeds_.AddItem( new SpeedInfos( static_cast< E_Location >( iTerrain ) ) );

    for( int iRange = 0; iRange < eNbrAviationRange; ++iRange )
        vAviationResourceQuotas_.AddItem( new AviationResourceQuotasInfos( static_cast< E_AviationRange >( iRange ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::FillMissingConsumptions
// Created: ABR 2012-12-03
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::FillMissingConsumptions()
{
    consumptions_.FillMissingConsumptions( resources_.categories_ );
}

// -----------------------------------------------------------------------------
// Name: EquipmentInfos::CreateCopy
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Equipments_Data::EquipmentInfos* ADN_Equipments_Data::EquipmentInfos::CreateCopy()
{
    ADN_Equipments_Data::EquipmentInfos* pCopy = new ADN_Equipments_Data::EquipmentInfos();

    pCopy->strAdditionalComments_ = strAdditionalComments_.GetData();
    pCopy->ptrArmor_ = ptrArmor_.GetData();
    pCopy->ptrSize_ = ptrSize_.GetData();
    pCopy->rWeight_ = rWeight_.GetData();
    pCopy->rMaxSpeed_ = rMaxSpeed_.GetData();
    pCopy->strCodeEMAT6_ = strCodeEMAT6_.GetData();
    pCopy->strCodeEMAT8_ = strCodeEMAT8_.GetData();
    pCopy->strCodeLFRIL_ = strCodeLFRIL_.GetData();
    pCopy->strCodeNNO_ = strCodeNNO_.GetData();
    pCopy->strNativeCountry_ = strNativeCountry_.GetData();
    pCopy->strStartingCountry_ = strStartingCountry_.GetData();
    pCopy->strStartingDate_ = strStartingDate_.GetData();
    pCopy->strInformationOrigin_ = strInformationOrigin_.GetData();
    pCopy->equipmentCategory_ = equipmentCategory_.GetData();

    for( int iTerrain = 0; iTerrain < eNbrLocation; ++iTerrain )
    {
        pCopy->vSpeeds_[ iTerrain ]->rSpeed_        = vSpeeds_[ iTerrain ]->rSpeed_.GetData();
        pCopy->vSpeeds_[ iTerrain ]->nConstruction_ = vSpeeds_[ iTerrain ]->nConstruction_.GetData();
    }

    pCopy->bAviationResourcesQuotas_ = bAviationResourcesQuotas_.GetData();
    for( int iRange = 0; iRange < eNbrAviationRange; ++iRange )
        for( int i = 0; i < eNbrAmmunitionType + 1; ++i )
            pCopy->vAviationResourceQuotas_[ iRange ]->resourceQuotas_[ i ] = vAviationResourceQuotas_[ iRange ]->resourceQuotas_[ i ].GetData();

    for( auto itWeapon = vWeapons_.begin(); itWeapon != vWeapons_.end(); ++itWeapon )
    {
        WeaponInfos* pNew = (*itWeapon)->CreateCopy();
        pCopy->vWeapons_.AddItem( pNew );
    }

    for( auto itActiveProtections = vActiveProtections_.begin(); itActiveProtections != vActiveProtections_.end(); ++itActiveProtections )
    {
        ActiveProtectionsInfos* pNew = (*itActiveProtections)->CreateCopy();
        pCopy->vActiveProtections_.AddItem( pNew );
    }

    for( auto itSensor = vSensors_.begin(); itSensor != vSensors_.end(); ++itSensor )
    {
        SensorInfos* pNew = (*itSensor)->CreateCopy();
        pCopy->vSensors_.AddItem( pNew );
    }

    for( auto itRadar = vRadars_.begin(); itRadar != vRadars_.end(); ++itRadar )
    {
        RadarInfos* pNew = (*itRadar)->CreateCopy();
        pCopy->vRadars_.AddItem( pNew );
    }

    for( auto itObject = vObjects_.begin(); itObject != vObjects_.end(); ++itObject )
    {
        ObjectInfos* pNew = (*itObject)->CreateCopy();
        pCopy->vObjects_.AddItem( pNew );
    }

    for( auto itDisaster = vDisasterImpacts_.begin(); itDisaster != vDisasterImpacts_.end(); ++itDisaster )
    {
        DisasterImpactInfos* pNew = (*itDisaster)->CreateCopy();
        pCopy->vDisasterImpacts_.AddItem( pNew );
    }

    pCopy->resources_.CopyFrom( resources_ );
    pCopy->consumptions_.CopyFrom( consumptions_ );

    pCopy->bTroopEmbarkingTimes_ = bTroopEmbarkingTimes_.GetData();
    pCopy->embarkingTimePerPerson_ = embarkingTimePerPerson_.GetData();
    pCopy->disembarkingTimePerPerson_ = disembarkingTimePerPerson_.GetData();
    pCopy->bCanCarryCargo_ = bCanCarryCargo_.GetData();
    pCopy->rWeightTransportCapacity_ = rWeightTransportCapacity_.GetData();
    pCopy->embarkingTimePerTon_ = embarkingTimePerTon_.GetData();
    pCopy->disembarkingTimePerTon_ = disembarkingTimePerTon_.GetData();
    pCopy->bCanTransportDestroyed_ = bCanTransportDestroyed_.GetData();
    pCopy->bCanCarryCrowd_ = bCanCarryCrowd_.GetData();
    pCopy->nCrowdTransportCapacity_ = nCrowdTransportCapacity_.GetData();
    pCopy->crowdEmbarkingTimePerPerson_ = crowdEmbarkingTimePerPerson_.GetData();
    pCopy->crowdDisembarkingTimePerPerson_ = crowdDisembarkingTimePerPerson_.GetData();

    pCopy->logInfos_.CopyFrom( logInfos_ );

    pCopy->attritionBreakdowns_.CopyFrom( attritionBreakdowns_ );
    pCopy->randomBreakdowns_.CopyFrom( randomBreakdowns_ );

    pCopy->nMaxSlope_ = nMaxSlope_.GetData();
    pCopy->nSlopeDeceleration_ = nSlopeDeceleration_.GetData();
    pCopy->length_ = length_.GetData();
    pCopy->width_ = width_.GetData();
    pCopy->frontSeparationDistance_ = frontSeparationDistance_.GetData();
    pCopy->safetyDistance_ = safetyDistance_.GetData();
    pCopy->speedSafetyDistance_ = speedSafetyDistance_.GetData();

    pCopy->nPowerDirectFire_    = nPowerDirectFire_.GetData();
    pCopy->nPowerIndirectFire_  = nPowerIndirectFire_.GetData();
    pCopy->nPowerCloseCombat_   = nPowerCloseCombat_.GetData();
    pCopy->nPowerEngineering_   = nPowerEngineering_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::EquipmentInfos::ReadSpeed
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::ReadSpeed( xml::xistream& input )
{
    const std::string strLocation = input.attribute< std::string >( "terrain" );
    try
    {
        E_Location nLocation = ENT_Tr::ConvertToLocation( strLocation );
        vSpeeds_.at( nLocation )->ReadArchive( input );
    }
    catch( const std::exception& /*e*/ )
    {
        throw MASA_EXCEPTION( tools::translate( "Equipments_Data", "Equipment - Invalid terrain type '%1'" ).arg( strLocation.c_str() ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::EquipmentInfos::ReadAviationQuota
// Created: JSR 2013-10-16
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::ReadAviationQuota( xml::xistream& input )
{
    const std::string strRange = input.attribute< std::string >( "range" );
    try
    {
        E_AviationRange nRange = ENT_Tr::ConvertToAviationRange( strRange );
        vAviationResourceQuotas_.at( nRange )->ReadArchive( input );
    }
    catch( const std::exception& /*e*/ )
    {
        throw MASA_EXCEPTION( tools::translate( "Equipments_Data", "Aviation range - Invalid range type '%1'" ).arg( strRange.c_str() ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::EquipmentInfos::ReadSensor
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::ReadSensor( xml::xistream& input )
{
    std::unique_ptr<SensorInfos> spNew( new SensorInfos() );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vSensors_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eEquipments, -1, tools::translate( "ADN_Automata_Data", "Sensors" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::EquipmentInfos::ReadRadar
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::ReadRadar( xml::xistream& input )
{
    std::unique_ptr<RadarInfos> spNew( new RadarInfos() );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vRadars_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eEquipments, -1, tools::translate( "ADN_Automata_Data", "Radars" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::EquipmentInfos::ReadWeapon
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::ReadWeapon( xml::xistream& input )
{
    std::unique_ptr<WeaponInfos> spNew( new WeaponInfos() );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vWeapons_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eEquipments, -1, tools::translate( "ADN_Automata_Data", "Weapons" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::EquipmentInfos::ReadActiveProtection
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::ReadActiveProtection( xml::xistream& input )
{
    std::unique_ptr<ActiveProtectionsInfos> spNew( new ActiveProtectionsInfos() );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vActiveProtections_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eEquipments, -1, tools::translate( "ADN_Automata_Data", "Active Protections" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::EquipmentInfos::ReadObject
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::ReadObject( xml::xistream& input )
{
    std::unique_ptr< ObjectInfos > spNew( new ObjectInfos() );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vObjects_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eEquipments, -1, tools::translate( "ADN_Automata_Data", "Objects" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::ReadDisasterImpact
// Created: JSR 2014-04-23
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::ReadDisasterImpact( xml::xistream& input )
{
    std::unique_ptr< DisasterImpactInfos > spNew( new DisasterImpactInfos() );
    spNew->ReadArchive( input );
    vDisasterImpacts_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: EquipmentInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "comment", strAdditionalComments_ )
          >> xml::optional >> xml::start( "operational-information" )
            >> xml::optional >> xml::attribute( "native-country", strNativeCountry_ )
            >> xml::optional >> xml::attribute( "information-origin", strInformationOrigin_ )
          >> xml::end
          >> xml::optional >> xml::content( "equipment-category", equipmentCategory_ );

    strCodeEMAT6_ = strName_.GetData();
    strCodeEMAT8_ = strName_.GetData();
    strCodeLFRIL_ = strName_.GetData();
    strCodeNNO_ = strName_.GetData();
    input >> xml::optional >> xml::attribute( "codeEMAT6", strCodeEMAT6_ )
          >> xml::optional >> xml::attribute( "codeEMAT8", strCodeEMAT8_ )
          >> xml::optional >> xml::attribute( "codeLFRIL", strCodeLFRIL_ )
          >> xml::optional >> xml::attribute( "codeNNO",   strCodeNNO_   );

    input >> xml::attribute( "protection", ptrArmor_ )
          >> xml::attribute( "size", ptrSize_ )
          >> xml::attribute( "weight", rWeight_ );

    input >> xml::start( "speeds" )
            >> xml::attribute( "max", rMaxSpeed_ )
            >> xml::list( "speed", *this, &ADN_Equipments_Data::EquipmentInfos::ReadSpeed )
          >> xml::end;

    input >> xml::start( "composition" );
    resources_.ReadArchive( input, strName_.GetData() );
    input >> xml::end;

    input >> xml::start( "sensors" )
            >> xml::list( "sensor", *this, &ADN_Equipments_Data::EquipmentInfos::ReadSensor )
          >> xml::end;

    input >> xml::optional >> xml::start( "radars" )
            >> xml::list( "radar", *this, &ADN_Equipments_Data::EquipmentInfos::ReadRadar )
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
                >> xml::optional >> xml::attribute( "can-transport-destroyed", bCanTransportDestroyed_ )
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

    consumptions_.ReadArchive( input, resources_.categories_, strName_.GetData() );
    FillMissingConsumptions();
    bAviationResourcesQuotas_ = input.has_child( "aviation-quotas" );
    if( bAviationResourcesQuotas_.GetData() )
        input >> xml::start( "aviation-quotas" )
                >> xml::list( "aviation-quota", *this, &ADN_Equipments_Data::EquipmentInfos::ReadAviationQuota )
              >> xml::end;

    input >> xml::start( "weapon-systems" )
            >> xml::list( "weapon-system", *this, &ADN_Equipments_Data::EquipmentInfos::ReadWeapon )
          >> xml::end
          >> xml::optional >> xml::start ( "active-protections" )
            >> xml::list( "protection", *this, &ADN_Equipments_Data::EquipmentInfos::ReadActiveProtection )
          >> xml::end
          >> xml::start( "objects" )
            >> xml::list( "object", *this, &ADN_Equipments_Data::EquipmentInfos::ReadObject )
          >> xml::end;
    if( input.has_child( "power-indicators" ) )
    {
        input >> xml::start( "power-indicators" )
                >> xml::attribute( "direct-fire", nPowerDirectFire_ )
                >> xml::attribute( "indirect-fire", nPowerIndirectFire_ )
                >> xml::attribute( "close-combat", nPowerCloseCombat_ )
                >> xml::attribute( "engineering", nPowerEngineering_ )
              >> xml::end;
    }

    logInfos_.ReadArchive( input );

    randomBreakdowns_.ReadArchive( input, strName_.GetData() );
    attritionBreakdowns_.ReadArchive( input, strName_.GetData() );

    if( input.has_attribute( "max-slope" ) )
    {
        double slope = input.attribute< double >( "max-slope" );
        nMaxSlope_ = static_cast< int >( atan( slope )* 180 / MT_PI + 0.5 );
    }
    if( input.has_attribute( "slope-deceleration" ) )
        nSlopeDeceleration_ = static_cast< int >( input.attribute< double >( "slope-deceleration" ) * 100 + 0.5 );

    input >> xml::optional >> xml::attribute( "length", length_ );
    input >> xml::optional >> xml::attribute( "width", width_ );
    input >> xml::optional >> xml::attribute( "front-separation-distance", frontSeparationDistance_ );
    input >> xml::optional >> xml::attribute( "safety-distance", safetyDistance_ );
    input >> xml::optional >> xml::attribute( "speed-safety-distance", speedSafetyDistance_ );

    input >> xml::optional >> xml::start( "operational-information" )
            >> xml::optional >> xml::attribute( "starting-country", strStartingCountry_ )
            >> xml::optional >> xml::attribute( "starting-date", strStartingDate_ )
          >> xml::end;

    input >> xml::optional >> xml::start( "disaster-impacts" )
            >> xml::list( "disaster-impact", *this, &ADN_Equipments_Data::EquipmentInfos::ReadDisasterImpact )
        >> xml::end;

    for( auto it = vWeapons_.begin(); it != vWeapons_.end(); ++it )
    {
        std::string name( (*it)->strName_.GetData() );
        bool ok = false;
        for( auto itc = resources_.categories_.begin(); itc != resources_.categories_.end(); ++itc )
        {
            if( !(*itc)->GetCrossedElement() )
                continue;
            std::string resourcename = (*itc)->GetCrossedElement()->strName_.GetData();
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
// Name: ADN_Equipments_Data::CheckDatabaseValidity
// Created: PHC 2011-01-20
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker )
{
    ptrSize_.CheckValidity( checker, strName_.GetData(), eEquipments, -1, tools::translate( "ADN_Equipments_Data", "Volume").toStdString() );
    if( attritionBreakdowns_.vBreakdowns_.empty() || randomBreakdowns_.vBreakdowns_.empty() )
    {
        ptrArmor_.CheckValidity( checker, strName_.GetData(), eEquipments, -1, tools::translate( "ADN_Equipments_Data", "Armor-Plating").toStdString() );
        if( ptrArmor_.GetData() && ptrArmor_.GetData()->nType_.GetData() == eProtectionType_Material )
            checker.AddError( eMissingBreakdown, strName_.GetData(), eEquipments );
    }
    attritionBreakdowns_.CheckValidity( checker, strName_.GetData() );
    randomBreakdowns_.CheckValidity( checker, strName_.GetData() );
    if( !logInfos_.IsRepairTypeValid() )
        checker.AddError( eMissingRepairType, strName_.GetData(), eEquipments );
    for( auto it = resources_.categories_.begin(); it != resources_.categories_.end(); ++it )
        if( (*it)->rLogLowThreshold_.GetData() > (*it)->rLogHighThreshold_.GetData() )
            checker.AddError( eRepartitionError, strName_.GetData(), eEquipments, -1,
                tools::translate( "ADN_Equipments_Data", "Invalid thresholds, high \'%1\' must be superior than low \'%2\'." )
                    .arg( (*it)->rLogHighThreshold_.GetData() ).arg( (*it)->rLogLowThreshold_.GetData() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::BreakdownGroupInfos::CheckValidity
// Created: LDC 2014-03-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::BreakdownGroupInfos::CheckValidity( ADN_ConsistencyChecker& checker, const std::string& composante ) const
{
    if( vBreakdowns_.empty() )
        return;
    double rSum = 0.0;
    for( auto it = vBreakdowns_.begin(); it != vBreakdowns_.end() ; ++it )
        rSum += ( *it )->rPercentage_.GetData();
    if( rSum != 100.0 )
        checker.AddError( eBadBreakdownSum, composante, eEquipments );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::CheckDatabaseValidity
// Created: PHC 2011-01-20
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
        (*it)->CheckDatabaseValidity( checker );
}

// -----------------------------------------------------------------------------
// Name: EquipmentInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "equipment" )
               << xml::attribute( "comment", strAdditionalComments_ )
               << xml::attribute( "name", *this )
               << xml::attribute( "id", nId_ )
               << xml::attribute( "codeEMAT6", strCodeEMAT6_ )
               << xml::attribute( "codeEMAT8", strCodeEMAT8_ )
               << xml::attribute( "codeLFRIL", strCodeLFRIL_ )
               << xml::attribute( "codeNNO",   strCodeNNO_   )
               << xml::attribute( "protection", ptrArmor_ )
               << xml::attribute( "size", ptrSize_ )
               << xml::attribute( "weight", rWeight_ );

    output << xml::start( "speeds" )
            << xml::attribute( "max", rMaxSpeed_ );
    for( auto itSpeed = vSpeeds_.begin(); itSpeed != vSpeeds_.end(); ++itSpeed )
        (*itSpeed)->WriteArchive( output );
    output << xml::end;

    output << xml::start( "composition" );
    resources_.WriteArchive( output );
    output << xml::end;

    output << xml::start( "sensors" );
    for( auto itSensor = vSensors_.begin(); itSensor != vSensors_.end(); ++itSensor )
        (*itSensor)->WriteArchive( output );
    output << xml::end;

    output << xml::start( "radars" );
    for( auto itRadar = vRadars_.begin(); itRadar != vRadars_.end(); ++itRadar )
        (*itRadar)->WriteArchive( output );
    output << xml::end;

    output << xml::start( "transports" );
    if( bTroopEmbarkingTimes_.GetData() )
        output << xml::start( "crew" )
                << xml::attribute( "man-boarding-time", embarkingTimePerPerson_ )
                << xml::attribute( "man-unloading-time", disembarkingTimePerPerson_ )
            << xml::end;
    if( bCanCarryCargo_.GetData() )
    {
        output << xml::start( "unit" )
                << xml::attribute( "capacity", rWeightTransportCapacity_ )
                << xml::attribute( "ton-loading-time", embarkingTimePerTon_ )
                << xml::attribute( "ton-unloading-time", disembarkingTimePerTon_ );
        if( bCanTransportDestroyed_.GetData() )
            output << xml::attribute( "can-transport-destroyed", true );
        output << xml::end;
    }
    if( bCanCarryCrowd_.GetData() )
        output << xml::start( "crowd" )
                << xml::attribute( "capacity", nCrowdTransportCapacity_ )
                << xml::attribute( "man-boarding-time", crowdEmbarkingTimePerPerson_ )
                << xml::attribute( "man-unloading-time", crowdDisembarkingTimePerPerson_ )
               << xml::end;
    output << xml::end;

    consumptions_.WriteArchive( output );
    if( bAviationResourcesQuotas_.GetData() )
    {
        output << xml::start( "aviation-quotas" );
            for( auto itQuota = vAviationResourceQuotas_.begin(); itQuota != vAviationResourceQuotas_.end(); ++itQuota )
                ( *itQuota )->WriteArchive( output );
        output << xml::end;
    }

    output << xml::start( "weapon-systems" );
    for( auto itWeapon = vWeapons_.begin(); itWeapon != vWeapons_.end(); ++itWeapon )
        (*itWeapon)->WriteArchive( output );
    output << xml::end;

    if( ! vActiveProtections_.empty() )
    {
        output << xml::start( "active-protections" );
        for( auto itActiveProtections = vActiveProtections_.begin(); itActiveProtections != vActiveProtections_.end(); ++itActiveProtections )
            (*itActiveProtections)->WriteArchive( output );
        output << xml::end;
    }

    if( nPowerDirectFire_ != 0 || nPowerIndirectFire_ != 0 || nPowerCloseCombat_ != 0 || nPowerEngineering_ != 0 )
    {
        output << xml::start( "power-indicators" )
                 << xml::attribute( "direct-fire", nPowerDirectFire_ )
                 << xml::attribute( "indirect-fire", nPowerIndirectFire_ )
                 << xml::attribute( "close-combat", nPowerCloseCombat_ )
                 << xml::attribute( "engineering", nPowerEngineering_ )
               << xml::end;
    }

    output << xml::start( "objects" );
    for( auto itObject = vObjects_.begin(); itObject != vObjects_.end(); ++itObject )
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
    double maxSlope = nMaxSlope_ == 90 ? std::numeric_limits< double >::max() : tan( nMaxSlope_.GetData() * MT_PI / 180 );
    output << xml::attribute( "max-slope", maxSlope )
           << xml::attribute( "slope-deceleration", nSlopeDeceleration_.GetData() * 0.01 );

    if( length_.GetData() )
        output << xml::attribute( "length", length_ );
    if( width_.GetData() )
        output << xml::attribute( "width", width_ );
    if( frontSeparationDistance_.GetData() )
        output << xml::attribute( "front-separation-distance", frontSeparationDistance_ );
    if( safetyDistance_.GetData() )
        output << xml::attribute( "safety-distance", safetyDistance_ );
    if( speedSafetyDistance_.GetData() )
        output << xml::attribute( "speed-safety-distance", speedSafetyDistance_ );

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

    if( !vDisasterImpacts_.empty() )
    {
        output << xml::start( "disaster-impacts" );
        for( auto itDisaster= vDisasterImpacts_.begin(); itDisaster!= vDisasterImpacts_.end(); ++itDisaster )
            (*itDisaster)->WriteArchive( output );
        output << xml::end;
    }

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: EquipmentInfos::CheckValidity
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::EquipmentInfos::CheckValidity()
{
    ADN_RefWithLocalizedName::CheckValidity();
    CheckTypeValidity( strAdditionalComments_ );
    CheckTypeValidity( strNativeCountry_ );
    CheckTypeValidity( strInformationOrigin_ );
    CheckTypeValidity( equipmentCategory_ );
}

//=============================================================================
//
//=============================================================================

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_Data constructor
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
ADN_Equipments_Data::ADN_Equipments_Data()
    : ADN_Data_ABC( eEquipments )
    , vEquipments_()
{
    vEquipments_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_Data destructor
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
ADN_Equipments_Data::~ADN_Equipments_Data()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Munitions_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Equipments_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szComponents_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::ReadElement
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ReadElement( xml::xistream& input )
{
    std::unique_ptr<EquipmentInfos> spNew( new EquipmentInfos( input.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( input );
    vEquipments_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::ReadArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "equipments" )
            >> xml::list( "equipment", *this, &ADN_Equipments_Data::ReadElement )
          >> xml::end;
    vEquipments_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::WriteArchive
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipments_Data::WriteArchive( xml::xostream& output ) const
{
    if( vEquipments_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "equipments" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Equipments" );
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::GetEquipmentsThatUse
// Created: LDC 2012-05-03
// -----------------------------------------------------------------------------
QStringList ADN_Equipments_Data::GetEquipmentsThatUse( ADN_Radars_Data::RadarInfos& radar )
{
    QStringList result;
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
    {
        EquipmentInfos* pComp = *it;
        for( auto it2 = pComp->vRadars_.begin(); it2 != pComp->vRadars_.end(); ++it2 )
            if( ( *it2 )->GetCrossedElement() == &radar )
            {
                result << pComp->strName_.GetData().c_str();
                break;
            }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::GetEquipmentsThatUse
// Created: APE 2005-04-22
// -----------------------------------------------------------------------------
QStringList ADN_Equipments_Data::GetEquipmentsThatUse( ADN_Sensors_Data::SensorInfos& sensor )
{
    QStringList result;
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
    {
        EquipmentInfos* pComp = *it;
        for( auto it2 = pComp->vSensors_.begin(); it2 != pComp->vSensors_.end(); ++it2 )
            if( ( *it2 )->GetCrossedElement() == &sensor )
            {
                result << pComp->strName_.GetData().c_str();
                break;
            }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::GetEquipmentsThatUse
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
QStringList ADN_Equipments_Data::GetEquipmentsThatUse( ADN_Weapons_Data_WeaponInfos& weapon )
{
    QStringList result;
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
    {
        EquipmentInfos* pComp = *it;
        for( auto it2 = pComp->vWeapons_.begin(); it2 != pComp->vWeapons_.end(); ++it2 )
            if( (*it2)->GetCrossedElement() == &weapon )
            {
                result << pComp->strName_.GetData().c_str();
                break;
            }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::GetEquipmentsThatUse
// Created: LGY 2012-05-29
// -----------------------------------------------------------------------------
QStringList ADN_Equipments_Data::GetEquipmentsThatUse( ADN_Breakdowns_Data::BreakdownInfo& breakdown )
{
    QStringList result;
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
    {
        EquipmentInfos* pComp = *it;
        if( pComp->attritionBreakdowns_.Contains( breakdown ) || pComp->randomBreakdowns_.Contains( breakdown ) )
            result << pComp->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::GetEquipmentsThatUse
// Created: LGY 2012-05-29
// -----------------------------------------------------------------------------
QStringList ADN_Equipments_Data::GetEquipmentsThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
    {
        EquipmentInfos* pComp = *it;
        for( auto itObject = pComp->vObjects_.begin(); itObject != pComp->vObjects_.end(); ++itObject )
        {
            ADN_Objects_Data_ObjectInfos* infos = (*itObject)->GetCrossedElement();
            if( infos && infos->strName_.GetData() == object.strName_.GetData() )
                result << pComp->strName_.GetData().c_str();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::GetEquipmentsThatUse
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
QStringList ADN_Equipments_Data::GetEquipmentsThatUse( ADN_Armors_Data::ArmorInfos& armor )
{
    QStringList result;
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
    {
        EquipmentInfos* pComp = *it;
        if( pComp->ptrArmor_.GetData() && pComp->ptrArmor_.GetData()->strName_.GetData() == armor.strName_.GetData() )
                result << pComp->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::GetEquipmentsThatUse
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
QStringList ADN_Equipments_Data::GetEquipmentsThatUse( ADN_Volumes_Data::VolumeInfos& size )
{
    QStringList result;
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
    {
        EquipmentInfos* pComp = *it;
        if( pComp->ptrSize_.GetData() && pComp->ptrSize_.GetData()->strName_.GetData() == size.strName_.GetData() )
            result << pComp->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::GetEquipmentsThatUse
// Created: ABR 2012-08-01
// -----------------------------------------------------------------------------
QStringList ADN_Equipments_Data::GetEquipmentsThatUse( ADN_ActiveProtections_Data::ActiveProtectionsInfos& activeProtection )
{
    QStringList result;
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
    {
        EquipmentInfos* pComp = *it;
        for( auto itActive = pComp->vActiveProtections_.begin(); itActive != pComp->vActiveProtections_.end(); ++itActive )
        {
            ADN_ActiveProtections_Data::ActiveProtectionsInfos* infos = ( *itActive )->GetCrossedElement();
            if( infos && infos->strName_.GetData() == activeProtection.strName_.GetData() )
                result << pComp->strName_.GetData().c_str();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::GetEquipmentsThatUse
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_Equipments_Data::GetEquipmentsThatUse( ADN_Resources_Data::CategoryInfo& category )
{
    QStringList result;
    for( auto it = vEquipments_.begin(); it != vEquipments_.end(); ++it )
    {
        EquipmentInfos* pComp = *it;
        for( auto itResource = pComp->resources_.categories_.begin(); itResource != pComp->resources_.categories_.end(); ++itResource )
        {
            ADN_Resources_Data::CategoryInfo* infos = ( *itResource )->GetCrossedElement();
            if( infos && infos->strName_.GetData() == category.strName_.GetData() )
                result << pComp->strName_.GetData().c_str();
        }
    }
    return result;
}
