// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Models_ModelInfos.h"
#include "moc_ADN_Models_ModelInfos.cpp"
#include "ADN_Missions_Data.h"
#include "ADN_Models_MissionInfos.h"
#include "ADN_Models_OrderInfos.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_Models_ModelInfos constructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Models_ModelInfos::ADN_Models_ModelInfos()
    : isMasalife_( false )
    , type_( eNbrEntityType )
{
    assert( false ); // $$$$ ABR 2013-08-23: useless constructor, needed by ADN_Wizard...
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_ModelInfos constructor
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
ADN_Models_ModelInfos::ADN_Models_ModelInfos( E_EntityType type )
    : isMasalife_( false )
    , type_( type )
{
    connect( &vFragOrders_, SIGNAL( ItemAdded( void* ) ), this, SLOT( OnMainFragOrderAdded( void* ) ) );
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eModels, ADN_Tools::MakePluralFromEntityType( type ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_ModelInfos destructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_Models_ModelInfos::~ADN_Models_ModelInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AddFragOrder
// Created: HBD 2010-09-01
// -----------------------------------------------------------------------------
void ADN_Models_ModelInfos::AddFragOrder( ADN_Missions_FragOrder* fragorder, const std::string& name )
{
    std::auto_ptr< ADN_Models_OrderInfos > spNew( new ADN_Models_OrderInfos( fragorder, name ) );
    vFragOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: RemoveFragOder
// Created: HBD 2010-09-06
// -----------------------------------------------------------------------------
void ADN_Models_ModelInfos::RemoveFragOder( const std::string& order )
{
    for( auto it = vFragOrders_.begin(); it != vFragOrders_.end(); ++it )
        if( (*it)->strName_ == order)
        {
            vFragOrders_.RemItem( *it );
            break;
        }
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_ModelInfos::CreateCopy
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_ModelInfos* ADN_Models_ModelInfos::CreateCopy()
{
    ADN_Models_ModelInfos* pNewInfo = new ADN_Models_ModelInfos( type_ );
    pNewInfo->strDiaType_ = strDiaType_.GetData();
    pNewInfo->strFile_ = strFile_.GetData();
    pNewInfo->isMasalife_ = isMasalife_.GetData();

    pNewInfo->vMissions_.reserve( vMissions_.size() );
    for( auto it = vMissions_.begin(); it != vMissions_.end(); ++it )
        pNewInfo->vMissions_.AddItem( (*it)->CreateCopy() );
    pNewInfo->vFragOrders_.reserve( vFragOrders_.size() );
    for( auto it = vFragOrders_.begin(); it != vFragOrders_.end(); ++it )
        pNewInfo->vFragOrders_.AddItem( (*it)->CreateCopy() );
    return pNewInfo;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_ModelInfos::ReadMission
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_ModelInfos::ReadMission( xml::xistream& input )
{
    std::auto_ptr< ADN_Models_MissionInfos > spNew( new ADN_Models_MissionInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( ADN_Tools::ConvertEntityTypeToMissionType( type_ ) ) ) );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vMissions_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eModels, -1, tools::translate( "ADN_Models_ModelInfos", "Missions" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_ModelInfos::ReadOrder
// Created: HBD 2010-08-31
// -----------------------------------------------------------------------------
void ADN_Models_ModelInfos::ReadOrder( xml::xistream& input )
{
    std::auto_ptr< ADN_Models_OrderInfos > spNew( new ADN_Models_OrderInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder ) ) );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vFragOrders_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eModels, -1, tools::translate( "ADN_Models_ModelInfos", "Frag orders" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_ModelInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_ModelInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this )
        >> xml::attribute( "dia-type", strDiaType_ )
        >> xml::attribute( "file", strFile_ )
        >> xml::attribute( "masalife", isMasalife_ )
        >> xml::start( "missions" )
        >> xml::list( "mission", *this, &ADN_Models_ModelInfos::ReadMission )
        >> xml::end
        >> xml::optional
        >> xml::start( "fragorders" )
        >> xml::list( "fragorder", *this, &ADN_Models_ModelInfos::ReadOrder )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_ModelInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_ModelInfos::WriteArchive( const std::string& type, xml::xostream& output )
{
    output << xml::start( type )
        <<  xml::attribute( "name", *this )
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

// -----------------------------------------------------------------------------
// Name: ADN_Models_ModelInfos::OnMainFragOrderAdded
// Created: ABR 2014-04-10
// -----------------------------------------------------------------------------
void ADN_Models_ModelInfos::OnMainFragOrderAdded( void* data )
{
    auto order = static_cast< ADN_Models_OrderInfos* >( data );
    if( !order )
        return;
    for( auto itMission = vMissions_.begin(); itMission != vMissions_.end(); ++itMission )
    {
        auto& mission = **itMission;
        ADN_Models_OrderInfos* missionOrder = 0;
        for( auto it = mission.vOrders_.begin(); it != mission.vOrders_.end(); ++it )
            if( order->GetCrossedElement() == ( *it )->GetCrossedElement() )
            {
                missionOrder = *it;
                break;
            }
        mission.vOrders_.RemItem( missionOrder );
    }   
}
