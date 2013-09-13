// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Models_Data.h"

#include "ADN_Workspace.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Models_Gui.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_enums.h"
#include "ENT/ENT_Tr.h"
#include "clients_kernel/XmlTranslations.h"

// =============================================================================
// OrderInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: OrderInfos::OrderInfos
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
ADN_Models_Data::OrderInfos::OrderInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder ), 0, true )
{
    // NOTHING
}

ADN_Models_Data::OrderInfos::OrderInfos( ADN_Missions_FragOrder* fragorder, const std::string& name )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder ), fragorder, true )
{
    strName_ = name;
}

// -----------------------------------------------------------------------------
// Name: OrderInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::OrderInfos::ReadArchive( xml::xistream& input )
{
    ADN_CrossedRef< ADN_Missions_ABC >::ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: OrderInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::OrderInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "fragorder" );
    ADN_CrossedRef< ADN_Missions_ABC >::WriteArchive( output );
    output<< xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::OrderInfos::CreateCopy
// Created: HBD 2010-08-31
// -----------------------------------------------------------------------------
ADN_Models_Data::OrderInfos* ADN_Models_Data::OrderInfos::CreateCopy()
{
    OrderInfos* result = new OrderInfos();
    result->SetCrossedElement( GetCrossedElement() );
    result->strName_ = strName_.GetData();
    return result;
}

// =============================================================================
// MissionInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionInfos::~MissionInfos
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::MissionInfos::MissionInfos( const ADN_Missions_Data::T_Mission_Vector& missions, ADN_Missions_ABC* mission /* = 0 */ )
    : ADN_CrossedRef< ADN_Missions_ABC >( missions, mission, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::~MissionInfos
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::MissionInfos::~MissionInfos()
{
    vOrders_.Reset();
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::CreateCopy
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::MissionInfos* ADN_Models_Data::MissionInfos::CreateCopy()
{
    MissionInfos* pMission = new MissionInfos( GetVector(), GetCrossedElement() );

    pMission->vOrders_.reserve( vOrders_.size() );
    for( T_OrderInfos_Vector::iterator it = vOrders_.begin(); it != vOrders_.end(); ++it )
    {
        OrderInfos* pOrder = new OrderInfos();
        pOrder->SetCrossedElement( ( *it )->GetCrossedElement() );
        pOrder->strName_ = (*it)->strName_.GetData();
        pMission->vOrders_.AddItem( pOrder );
    }
    return pMission;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::MissionInfos::ReadFragOrder
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::ReadFragOrder( xml::xistream& input )
{
    std::auto_ptr< OrderInfos > spNew( new OrderInfos() );
    spNew->ReadArchive( input );
    vOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ );
    ADN_Missions_ABC* mission = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindMission( GetVector(), strName_.GetData() );
    if( !mission )
        throw MASA_EXCEPTION( tools::translate( "Models_Data", "Doctrine models - Invalid mission '%1'" ).arg( strName_.GetData().c_str() ).toStdString() );
    SetCrossedElement( mission );
    input >> xml::list( "fragorder", *this, &ADN_Models_Data::MissionInfos::ReadFragOrder );
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "mission" )
            << xml::attribute( "name", GetCrossedElement()->strName_ );
    for( auto it = vOrders_.begin(); it != vOrders_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// =============================================================================
//
// =============================================================================

namespace
{
    ADN_Missions_Data::T_Mission_Vector dummy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ModelInfos
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos::ModelInfos()
    : missions_ ( dummy )
    , isMasalife_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ModelInfos::ModelInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos::ModelInfos( ADN_Missions_Data::T_Mission_Vector& missions )
    : missions_( missions )
    , strDiaType_( "T_Pion" )
    , strFile_( "DEC\\For Tests\\Empty\\Files.hal" )
    , isMasalife_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::ModelInfos
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos::~ModelInfos()
{
    vMissions_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::AddFragOrder
// Created: HBD 2010-09-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::AddFragOrder( ADN_Missions_FragOrder* fragorder, const std::string& name )
{
    std::auto_ptr< OrderInfos > spNew( new OrderInfos( fragorder, name ) );
    vFragOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::RemoveFragOder
// Created: HBD 2010-09-06
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::RemoveFragOder( const std::string& order )
{
    for( auto it = vFragOrders_.begin(); it != vFragOrders_.end(); ++it )
        if( (*it)->strName_ == order)
        {
            vFragOrders_.RemItem( *it );
            break;
        }
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::CreateCopy
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos* ADN_Models_Data::ModelInfos::CreateCopy()
{
    ModelInfos* pNewInfo = new ModelInfos( missions_ );
    pNewInfo->strDiaType_ = strDiaType_.GetData();
    pNewInfo->strFile_ = strFile_.GetData();
    pNewInfo->isMasalife_ = isMasalife_.GetData();

    pNewInfo->vMissions_.reserve( vMissions_.size() );
    for( T_MissionInfos_Vector::iterator itMission = vMissions_.begin(); itMission != vMissions_.end(); ++itMission )
    {
        MissionInfos* pNewMission = (*itMission)->CreateCopy();
        pNewInfo->vMissions_.AddItem( pNewMission );
    }
    pNewInfo->vFragOrders_.reserve( vFragOrders_.size() );
    for( T_OrderInfos_Vector::iterator itOrder = vFragOrders_.begin(); itOrder != vFragOrders_.end(); ++itOrder )
    {
        OrderInfos* pNewOrder = (*itOrder)->CreateCopy();
        pNewInfo->vFragOrders_.AddItem( pNewOrder );
    }
    return pNewInfo;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ModelInfos::ReadMission
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::ReadMission( xml::xistream& input )
{
    std::auto_ptr<MissionInfos> spNew( new MissionInfos( missions_ ) );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vMissions_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eModels, -1, tools::translate( "ADN_Models_Data", "Missions" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ModelInfos::ReadOrder
// Created: HBD 2010-08-31
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::ReadOrder( xml::xistream& input )
{
    std::auto_ptr<OrderInfos> spNew( new OrderInfos() );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vFragOrders_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eModels, -1, tools::translate( "ADN_Models_Data", "Frag orders" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "dia-type", strDiaType_ )
          >> xml::attribute( "file", strFile_ )
          >> xml::attribute( "masalife", isMasalife_ )
          >> xml::start( "missions" )
            >> xml::list( "mission", *this, &ADN_Models_Data::ModelInfos::ReadMission )
        >> xml::end
        >> xml::optional
        >> xml::start( "fragorders" )
            >> xml::list( "fragorder", *this, &ADN_Models_Data::ModelInfos::ReadOrder )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::WriteArchive( const std::string& type, xml::xostream& output )
{
    output << xml::start( type )
            <<  xml::attribute( "name", strName_ )
            <<  xml::attribute( "dia-type", strDiaType_ )
            <<  xml::attribute( "file", strFile_.GetData().Normalize() )
            <<  xml::attribute( "masalife", isMasalife_ )
            <<  xml::start( "missions" );
    for( auto it = vMissions_.begin(); it != vMissions_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end
        << xml::start( "fragorders" );
    for( auto it = vFragOrders_.begin(); it != vFragOrders_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end
        << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data constructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Models_Data::ADN_Models_Data()
    : ADN_Data_ABC( eModels )
{
    vUnitModels_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
    vAutomataModels_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
    vPopulationModels_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data destructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Models_Data::~ADN_Models_Data()
{
    Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data::Reset
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
void ADN_Models_Data::Reset()
{
    vUnitModels_.Reset();
    vAutomataModels_.Reset();
    vPopulationModels_.Reset();
}

namespace
{
    class SourcePaths : boost::noncopyable
    {
    public:
         SourcePaths() {}
        ~SourcePaths() {}

        void AddSource( xml::xistream& xis )
        {
            static const tools::Path basePath( "decisional" );
            sourcePaths_.push_back( ( basePath / xis.attribute< tools::Path >( "directory" ) ).Normalize().ToUnicode() );
        }
        const std::vector< std::wstring >& GetSourcePaths() const { return sourcePaths_; }

    private:
        std::vector< std::wstring > sourcePaths_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::Initialize
// Created: JSR 2013-05-22
// -----------------------------------------------------------------------------
void ADN_Models_Data::Initialize()
{
    tools::Path p = ADN_Workspace::GetWorkspace().GetProject().GetWorkDirInfos().GetWorkingDirectory().GetData();
    const tools::Path root = p.Root();
    while( p != root && p.FileName().ToLower().ToUTF8() != "physical" )
        p = p.Parent();
    tools::Path decisionalFile = p.Parent() / "decisional/decisional.xml";
    if( decisionalFile.Exists() )
    {
        SourcePaths sourcePaths;
        tools::Xifstream xis( decisionalFile );
        xis >> xml::start( "decisional" )
                >> xml::start( "RepertoiresSources" )
                    >> xml::list( "RepertoireSources" , sourcePaths, &SourcePaths::AddSource );
        ADN_Workspace::GetWorkspace().GetModels().GetGui().SetDecisionalFilters( sourcePaths.GetSourcePaths() );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Models_Data::FilesNeeded(tools::Path::T_Paths& files) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szModels_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadUnit
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadUnit( xml::xistream& input )
{
    std::string strName = input.attribute< std::string >( "name" );
    std::auto_ptr<ModelInfos> spNew( new ModelInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_Pawn ) ) );
    spNew->ReadArchive( input );
    spNew->strName_.SetTranslation( strName, translations_->GetTranslation( "units", strName ) );
    vUnitModels_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadAutomat
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadAutomat( xml::xistream& input )
{
    std::string strName = input.attribute< std::string >( "name" );
    std::auto_ptr<ModelInfos> spNew( new ModelInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_Automat ) ) );
    spNew->ReadArchive( input );
    spNew->strName_.SetTranslation( strName, translations_->GetTranslation( "automats", strName ) );
    vAutomataModels_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadPopulation
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadPopulation( xml::xistream& input )
{
    std::string strName = input.attribute< std::string >( "name" );
    std::auto_ptr<ModelInfos> spNew( new ModelInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_Population ) ) );
    spNew->ReadArchive( input );
    spNew->strName_.SetTranslation( strName, translations_->GetTranslation( "crowd", strName ) );
    vPopulationModels_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "models" )
            >> xml::start( "units" )
                >> xml::list( "unit", *this, &ADN_Models_Data::ReadUnit )
            >> xml::end
            >> xml::start( "automats" )
                >> xml::list( "automat", *this, &ADN_Models_Data::ReadAutomat )
            >> xml::end
            >> xml::start( "crowd" )
                >> xml::list( "crowd", *this, &ADN_Models_Data::ReadPopulation )
            >> xml::end
          >> xml::end;
    vUnitModels_.CheckValidity();
    vAutomataModels_.CheckValidity();
    vPopulationModels_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::WriteArchive( xml::xostream& output )
{
    if( vUnitModels_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( tools::translate( "ADN_Models_Data", "Invalid data on tab '%1', subtab '%2'" )
                              .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( tools::translate( "ADN_Models_Data", "Unit models" ) ).toStdString() );
    if( vAutomataModels_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( tools::translate( "ADN_Models_Data", "Invalid data on tab '%1', subtab '%2'" )
                              .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( tools::translate( "ADN_Models_Data", "Automata models" ) ).toStdString() );
    if( vPopulationModels_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( tools::translate( "ADN_Models_Data", "Invalid data on tab '%1', subtab '%2'" )
                              .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( tools::translate( "ADN_Models_Data", "Crowds models" ) ).toStdString() );

    output << xml::start( "models" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Models" );
    output  << xml::start( "units" );
    for( auto  it1 = vUnitModels_.begin(); it1 != vUnitModels_.end(); ++it1 )
        ( *it1 )->WriteArchive( "unit", output );
    output << xml::end
            << xml::start( "automats" );
    for( auto it2 = vAutomataModels_.begin(); it2 != vAutomataModels_.end(); ++it2 )
        ( *it2 )->WriteArchive( "automat", output );
    output << xml::end
            << xml::start( "crowd" );
    for( auto it2 = vPopulationModels_.begin(); it2 != vPopulationModels_.end(); ++it2 )
        ( *it2 )->WriteArchive( "crowd", output );
    output << xml::end
          << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetModelsThatUse
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
QStringList ADN_Models_Data::GetModelsThatUse( E_EntityType type, ADN_Missions_Mission& mission )
{
    QStringList result;
    T_ModelInfos_Vector* currentVector = 0;
    if( type == eEntityType_Pawn )
        currentVector = &vUnitModels_;
    else if( type == eEntityType_Automat )
        currentVector = &vAutomataModels_;
    else
        currentVector = &vPopulationModels_;
    for( auto it = currentVector->begin(); it != currentVector->end(); ++it )
    {
        ModelInfos* pModel = *it;
        if( !pModel )
            continue;
        for( auto missionIt = pModel->vMissions_.begin(); missionIt != pModel->vMissions_.end(); ++missionIt )
            if( ( *missionIt )->GetCrossedElement() == &mission )
            {
                result << pModel->strName_.GetData().c_str();
                break;
            }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetModelsThatUse
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_Models_Data::GetModelsThatUse( E_EntityType type, ADN_Missions_FragOrder& fragOrder )
{
    QStringList result;
    T_ModelInfos_Vector* currentVector = 0;
    if( type == eEntityType_Pawn )
        currentVector = &vUnitModels_;
    else if( type == eEntityType_Automat )
        currentVector = &vAutomataModels_;
    else
        currentVector = &vPopulationModels_;
    for( auto it = currentVector->begin(); it != currentVector->end(); ++it )
    {
        bool added = false;
        for( auto itOrder = ( *it )->vFragOrders_.begin(); !added && itOrder != ( *it )->vFragOrders_.end(); ++itOrder )
        {
            if( ( *itOrder )->strName_.GetData() == fragOrder.strName_.GetData() )
            {
                added = true;
                result << ( *it )->strName_.GetData().c_str();
            }
        }
        for( auto itMission = ( *it )->vMissions_.begin(); !added && itMission != ( *it )->vMissions_.end(); ++itMission )
        {
            for( auto itOrder = ( *itMission )->vOrders_.begin(); !added && itOrder != ( *itMission )->vOrders_.end(); ++itOrder )
            {
                if( ( *itOrder )->strName_.GetData() == fragOrder.strName_.GetData() )
                {
                    added = true;
                    result << ( *it )->strName_.GetData().c_str();
                }
            }
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::CheckDatabaseValidity
// Created: JSR 2013-04-11
// -----------------------------------------------------------------------------
void ADN_Models_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( auto it = vUnitModels_.begin(); it != vUnitModels_.end(); ++it )
        ( *it )->CheckValidity( checker, ( *it )->strName_.GetData(), eModels, eEntityType_Pawn );
    for( auto it = vAutomataModels_.begin(); it != vAutomataModels_.end(); ++it )
        ( *it )->CheckValidity( checker, ( *it )->strName_.GetData(), eModels, eEntityType_Automat );
    for( auto it = vPopulationModels_.begin(); it != vPopulationModels_.end(); ++it )
        ( *it )->CheckValidity( checker, ( *it )->strName_.GetData(), eModels, eEntityType_Population );
}
