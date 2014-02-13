// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_Objects_Data.h"
#include "ADN_ConsistencyChecker.h"
#include "ADN_WorkspaceElement.h"

namespace
{
    static const std::string locations[ 4 ] = { "polygon", "point", "line", "circle" };
    const std::string baseType = "T_Object_";
    unsigned int typeId = 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data_ObjectInfos::ADN_Objects_Data_ObjectInfos()
    : pointSize_ ( 1 )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eObjects, "objects" ) );
    ADN_Drawings_Data& drawingsData = ADN_Workspace::GetWorkspace().GetDrawings().GetData();
    for( int i = 0; i < 4; ++i )
    {
        geometries_[ i ] = false;
        symbols_[ i ].SetVector( drawingsData.GetGeometryDrawings( locations[ i ], "graphics" ) );
    }
    InitializeCapacities();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::~ADN_Objects_Data_ObjectInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Objects_Data_ObjectInfos::~ADN_Objects_Data_ObjectInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::InitializeCapacities
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::InitializeCapacities()
{
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Activable::TAG ].reset(                new ADN_Objects_Data::ADN_CapacityInfos_Activable() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Attrition::TAG ].reset(                new ADN_Objects_Data::ADN_CapacityInfos_Attrition() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_UrbanDestruction::TAG ].reset(         new ADN_Objects_Data::ADN_CapacityInfos_UrbanDestruction() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Avoidable::TAG ].reset(                new ADN_Objects_Data::ADN_CapacityInfos_Avoidable() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Bridging::TAG ].reset(                 new ADN_Objects_Data::ADN_CapacityInfos_Bridging() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Bypassable::TAG ].reset(               new ADN_Objects_Data::ADN_CapacityInfos_Bypassable() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Constructor::TAG ].reset(              new ADN_Objects_Data::ADN_CapacityInfos_Constructor() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Contamination::TAG ].reset(            new ADN_Objects_Data::ADN_CapacityInfos_Contamination() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Decontamination::TAG ].reset(          new ADN_Objects_Data::ADN_CapacityInfos_Decontamination() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Delay::TAG ].reset(                    new ADN_Objects_Data::ADN_CapacityInfos_Delay() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Detection::TAG ].reset(                new ADN_Objects_Data::ADN_CapacityInfos_Detection() ); // LTO
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_InteractionHeight::TAG ].reset(        new ADN_Objects_Data::ADN_CapacityInfos_InteractionHeight() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_InteractWithSide::TAG ].reset(         new ADN_Objects_Data::ADN_CapacityInfos_InteractWithSide() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Interference::TAG ].reset(             new ADN_Objects_Data::ADN_CapacityInfos_Interference() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Intoxication::TAG ].reset(             new ADN_Objects_Data::ADN_CapacityInfos_Intoxication() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Logistic::TAG ].reset(                 new ADN_Objects_Data::ADN_CapacityInfos_Logistic() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Mobility::TAG ].reset(                 new ADN_Objects_Data::ADN_CapacityInfos_Mobility() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Trafficability::TAG ].reset(           new ADN_Objects_Data::ADN_CapacityInfos_Trafficability() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Population::TAG ].reset(               new ADN_Objects_Data::ADN_CapacityInfos_Population() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Propagation::TAG ].reset(              new ADN_Objects_Data::ADN_CapacityInfos_Propagation() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Protection::TAG ].reset(               new ADN_Objects_Data::ADN_CapacityInfos_Protection() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Supply::TAG ].reset(                   new ADN_Objects_Data::ADN_CapacityInfos_Supply() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_SupplyRoute::TAG ].reset(              new ADN_Objects_Data::ADN_CapacityInfos_SupplyRoute() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_TerrainHeuristic::TAG ].reset(         new ADN_Objects_Data::ADN_CapacityInfos_TerrainHeuristic() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_TimeLimited::TAG ].reset(              new ADN_Objects_Data::ADN_CapacityInfos_TimeLimited() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Workable::TAG ].reset(                 new ADN_Objects_Data::ADN_CapacityInfos_Workable() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Spawn::TAG ].reset(                    new ADN_Objects_Data::ADN_CapacityInfos_Spawn() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Structural::TAG ].reset(               new ADN_Objects_Data::ADN_CapacityInfos_Structural() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::TAG ].reset(         new ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Perception::TAG ].reset(               new ADN_Objects_Data::ADN_CapacityInfos_Perception() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Scattering::TAG ].reset(               new ADN_Objects_Data::ADN_CapacityInfos_Scattering() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::TAG ].reset(  new ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Burn::TAG ].reset(                     new ADN_Objects_Data::ADN_CapacityInfos_Burn() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Flood::TAG ].reset(                    new ADN_Objects_Data::ADN_CapacityInfos_Flood() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Universal::TAG ].reset(                new ADN_Objects_Data::ADN_CapacityInfos_Universal() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Stock::TAG ].reset(                    new ADN_Objects_Data::ADN_CapacityInfos_Stock() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_ResourceNetwork::TAG ].reset(          new ADN_Objects_Data::ADN_CapacityInfos_ResourceNetwork() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Lodging::TAG ].reset(                  new ADN_Objects_Data::ADN_CapacityInfos_Lodging() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_AltitudeModifier::TAG ].reset(         new ADN_Objects_Data::ADN_CapacityInfos_AltitudeModifier() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_UndergroundNetwork::TAG ].reset(       new ADN_Objects_Data::ADN_CapacityInfos_UndergroundNetwork() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_FireForbidden::TAG ].reset(            new ADN_Objects_Data::ADN_CapacityInfos_FireForbidden() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Border::TAG ].reset(                   new ADN_Objects_Data::ADN_CapacityInfos_Border() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Disaster::TAG ].reset(                 new ADN_Objects_Data::ADN_CapacityInfos_Disaster() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadCapacityArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::ReadCapacityArchive( const std::string& type, xml::xistream& xis )
{
    auto it = capacities_.find( type );
    if( it != capacities_.end() )
        it->second->ReadArchive( xis );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::ReadGeometry
// Created: JSR 2012-02-13
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::ReadGeometry( xml::xistream& xis )
{
    const std::string code = xis.attribute< std::string >( "symbol" );
    const std::string type = xis.attribute< std::string >( "type" );
    for( int i = 0; i < 4; ++i )
    {
        if( locations[ i ] == type )
        {
            ADN_Drawings_Data& drawingsData = ADN_Workspace::GetWorkspace().GetDrawings().GetData();
            symbols_[ i ].SetData( drawingsData.GetDrawing( code ) );
            geometries_[ i ] = true;
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::GenerateNextType
// Created: ABR 2013-09-24
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data_ObjectInfos::GenerateNextType()
{
    return baseType + QString::number( ++typeId ).toStdString();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::ReadArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::ReadArchive( xml::xistream& xis )
{
    ADN_Drawings_Data& drawingsData = ADN_Workspace::GetWorkspace().GetDrawings().GetData();
    for( int i = 0; i < 4; ++i )
        symbols_[ i ].SetVector( drawingsData.GetGeometryDrawings( locations[ i ], "graphics" ) );

    xis >> xml::attribute( "name", *this )
        >> xml::attribute( "type", strType_ )
        >> xml::optional >> xml::attribute( "point-size", pointSize_ )
        >> xml::optional >> xml::attribute( "description", description_ )
        >> xml::start( "geometries" )
        >> xml::list( "geometry", *this, &ADN_Objects_Data_ObjectInfos::ReadGeometry )
        >> xml::end
        >> xml::list( *this, &ADN_Objects_Data_ObjectInfos::ReadCapacityArchive );
    if( strType_.GetData().substr( 0, baseType.size() ) == baseType )
    {
        QString strId = strType_.GetData().substr( baseType.size() ).c_str();
        unsigned int id = strId.toUInt();
        if( id && id > typeId )
            typeId = id;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::FixConsistency
// Created: ABR 2013-10-10
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::FixConsistency()
{
    if( strType_.GetData().empty() )
        strType_ = GenerateNextType();
    for( auto it = capacities_.begin(); it != capacities_.end(); ++it )
        it->second->FixConsistency();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::WriteArchive( xml::xostream& xos ) const
{
    xos << xml::start( "object" )
        << xml::attribute( "name", *this );
    // distance effect > 0 for geometry point
    if( pointSize_.GetData() > 0 && geometries_[ 1 ].GetData() )
        xos << xml::attribute( "point-size", pointSize_.GetData() );
    xos << xml::attribute( "type", strType_ );
    if( !description_.GetData().empty() )
        xos << xml::attribute( "description", description_ );

    xos << xml::start( "geometries" );
    for( int i = 0; i < 4; ++i )
    {
        if( geometries_[ i ].GetData() )
        {
            const std::string code = ( symbols_[ i ].GetData() ) ? symbols_[ i ].GetData()->GetCode() : "";
            xos << xml::start( "geometry" );
            xos << xml::attribute( "type", locations[ i ] );
            xos << xml::attribute( "symbol", code );
            xos << xml::end;
        }
    }
    xos << xml::end;

    for( auto it = capacities_.begin(); capacities_.end() != it; ++it )
        if( it->second->bPresent_.GetData() )
        {
            xos << xml::start( it->first );
            it->second->WriteArchive( xos );
            xos << xml::end;
        }
        xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::CheckDatabaseValidity
// Created: JSR 2012-02-16
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( auto it = capacities_.begin(); capacities_.end() != it; ++it )
        if( it->second->bPresent_.GetData() )
            it->second->CheckDatabaseValidity( checker, strName_ );

    for( int i = 0; i < 4; ++i )
        if( geometries_[ i ].GetData() )
            return;
    checker.AddError( eMissingGeometry, strName_.GetData(), eObjects );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::CheckValidity
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::CheckValidity()
{
    // $$$$ ABR 2013-01-15: TODO: Add a tree hierarchy between adn_ref, so we can apply CheckValidity on every child
    ADN_Ref_ABC::CheckValidity();
    ADN_ErrorStatus status = GetErrorStatus();
    strName_.CheckValidity();
    strType_.CheckValidity();
    status = std::max< ADN_ErrorStatus >( status, strName_.GetErrorStatus() );
    status = std::max< ADN_ErrorStatus >( status, strType_.GetErrorStatus() );
    SetErrorStatus( status );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::GetAllGeometries
// Created: JSR 2012-02-14
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data_ObjectInfos::GetAllGeometries() const
{
    std::string ret;
    for( int i = 0; i < 4; ++i )
    {
        if( geometries_[ i ].GetData() )
        {
            if( !ret.empty() )
                ret += " ";
            ret += locations[ i ];
        }
    }
    return ret;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::CreateCopy
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
ADN_Objects_Data_ObjectInfos* ADN_Objects_Data_ObjectInfos::CreateCopy()
{
    xml::xostringstream xos;
    try
    {
        FixConsistency();
        WriteArchive( xos );
    }
    catch( std::exception& e )
    {
        const QString errorMessage = tools::translate( "ADN_Objects_Data_ObjectInfos", "The object you attempt to copy is invalid: %1." ).arg( tools::GetExceptionMsg( e ).c_str() );
        QMessageBox::critical( qApp ? qApp->activeWindow() : 0, tools::translate( "ADN_Objects_Data_ObjectInfos", "Error" ), errorMessage );
        return 0;
    }
    ADN_Objects_Data_ObjectInfos* pCopy = new ADN_Objects_Data_ObjectInfos;
    xml::xistringstream xis( xos.str() );
    xis >> xml::start( "object" );
    pCopy->ReadArchive( xis );

    // Load spawn capacity
    ADN_Objects_Data::ADN_CapacityInfos_Spawn* spawn = static_cast< ADN_Objects_Data::ADN_CapacityInfos_Spawn* >( pCopy->capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Spawn::TAG ].get() );
    spawn->Load( pCopy->strName_.GetData() );

    return pCopy;
}
