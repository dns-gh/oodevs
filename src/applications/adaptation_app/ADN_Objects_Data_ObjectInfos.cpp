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

//-----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::ADN_Objects_Data_ObjectInfos
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Objects_Data_ObjectInfos::ADN_Objects_Data_ObjectInfos( const std::string& type )
    : ADN_Ref_ABC()
    , strType_   ( type )
    , geometries_( "polygon" )
    , pointSize_ ( 0. )
{
    symbol_.SetParentNode( *this );
    geometries_.SetParentNode( *this );
    description_.SetParentNode( *this );
    InitializeCapacities();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data_ObjectInfos::ADN_Objects_Data_ObjectInfos()
    : ADN_Ref_ABC()
    , strType_   ()
    , geometries_( "polygon" )
    , pointSize_ ( 0. )
{
    symbol_.SetParentNode( *this );
    geometries_.SetParentNode( *this );
    geometries_.SetParentNode( *this );
    ADN_Drawings_Data& drawingsData = ADN_Workspace::GetWorkspace().GetDrawings().GetData();
    symbol_.SetVector( drawingsData.GetGeometryDrawings( geometries_.GetData() ) );
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
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Extinguishable::TAG ].reset(           new ADN_Objects_Data::ADN_CapacityInfos_Extinguishable() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Medical::TAG ].reset(                  new ADN_Objects_Data::ADN_CapacityInfos_Medical() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_InteractionHeight::TAG ].reset(        new ADN_Objects_Data::ADN_CapacityInfos_InteractionHeight() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_InteractWithEnemy::TAG ].reset(        new ADN_Objects_Data::ADN_CapacityInfos_InteractWithEnemy() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Interference::TAG ].reset(             new ADN_Objects_Data::ADN_CapacityInfos_Interference() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Intoxication::TAG ].reset(             new ADN_Objects_Data::ADN_CapacityInfos_Intoxication() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Logistic::TAG ].reset(                 new ADN_Objects_Data::ADN_CapacityInfos_Logistic() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Mobility::TAG ].reset(                 new ADN_Objects_Data::ADN_CapacityInfos_Mobility() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Trafficability::TAG ].reset(           new ADN_Objects_Data::ADN_CapacityInfos_Trafficability() );
    capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Occupable::TAG ].reset(                new ADN_Objects_Data::ADN_CapacityInfos_Occupable() );
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
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::GetNodeName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data_ObjectInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data_ObjectInfos::GetNodeName()
{
    return std::string( "de l'objet " ) + strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadCapacityArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::ReadCapacityArchive( const std::string& type, xml::xistream& xis )
{
    IT_CapacityMap it = capacities_.find( type );
    if( it != capacities_.end() )
        it->second->ReadArchive( xis );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::ReadArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::ReadArchive( xml::xistream& xis )
{
    std::string code;
    xis >> xml::attribute( "name", strName_ )
        >> xml::attribute( "type", strType_ )
        >> xml::attribute( "geometry", geometries_ )
        >> xml::optional >> xml::attribute( "symbol", code )
        >> xml::optional >> xml::attribute( "point-size", pointSize_ )
        >> xml::optional >> xml::attribute( "description", description_ )
        >> xml::list( *this, &ADN_Objects_Data_ObjectInfos::ReadCapacityArchive );

    ADN_Drawings_Data& drawingsData = ADN_Workspace::GetWorkspace().GetDrawings().GetData();
    symbol_.SetVector( drawingsData.GetGeometryDrawings( geometries_.GetData() ) );
    symbol_.SetData( drawingsData.GetDrawing( code ), false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data_ObjectInfos::WriteArchive( xml::xostream& xos )
{
    std::string code = ( symbol_.GetData() ) ? symbol_.GetData()->GetCode() : "";
    xos << xml::start( "object" )
        << xml::attribute( "name", strName_ )
        << xml::attribute( "geometry", geometries_.GetData() )
        << xml::attribute( "symbol", code );
    if( pointSize_.GetData() )
        xos << xml::attribute( "point-size", pointSize_.GetData() );
    if( strType_ == "" )
        xos << xml::attribute( "type", strName_ );
    else
        xos << xml::attribute( "type", strType_ );
    if( !description_.GetData().empty() )
        xos << xml::attribute( "description", description_ );

    for( CIT_CapacityMap it = capacities_.begin(); capacities_.end() != it; ++it )
        if( it->second->bPresent_.GetData() )
        {
            xos << xml::start( it->first );
            it->second->WriteArchive( xos );
            xos << xml::end;
        }
        xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data_ObjectInfos::CreateCopy
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
ADN_Objects_Data_ObjectInfos* ADN_Objects_Data_ObjectInfos::CreateCopy()
{
    xml::xostringstream xos;
    WriteArchive( xos );
    ADN_Objects_Data_ObjectInfos* pCopy = new ADN_Objects_Data_ObjectInfos;
    xml::xistringstream xis( xos.str() );
    xis >> xml::start( "object" );
    pCopy->ReadArchive( xis );
    return pCopy;
}
