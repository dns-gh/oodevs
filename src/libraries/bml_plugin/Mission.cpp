// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Mission.h"
#include "MissionParameterFactory.h"
#include "MissionParameterPhaseLine.h"
#include "MissionParameterPhaseLines.h"
#include "dispatcher/Model.h"
#include "dispatcher/Automat.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Network_Def.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/OrderParameter.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Mission::Mission( xml::xistream& xis, const dispatcher::Model& model )
    : model_( model )
    , type_( ResolveMission( xis ) )
    , taskee_( 0 )
    , factory_( new MissionParameterFactory() )
{
    ReadTaskeeWho( xis );
    ReadParameters( xis );
}

// -----------------------------------------------------------------------------
// Name: Mission destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Mission::~Mission()
{
    // NOTHING
}

namespace
{
    std::string MissionNameFromCode( const std::string& code )
    {
        if( code == "ATTACK" )
            return "ABC GTIA Attaquer";
//        if( code == "" )
//            return "ABC GTIA DonnerCoupArret";
//        if( code == "" )
//            return "ABC GTIA Freiner";
        throw std::runtime_error( __FUNCTION__ ": Unsupported mission" );
    }

    const kernel::MissionType& GetMissionByName( const kernel::Resolver_ABC< kernel::MissionType >& missions, const std::string& name )
    {
        kernel::Iterator< const kernel::MissionType& > it( missions.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const kernel::MissionType& type = it.NextElement();
            if( type.GetName() == name )
                return type;
        }
        throw std::runtime_error( __FUNCTION__ ": Unable to resolve Mission" );
    }
}

// -----------------------------------------------------------------------------
// Name: Mission::ResolveMission
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
const kernel::MissionType& Mission::ResolveMission( xml::xistream& xis )
{
    std::string code;
    xis >> xml::start( "What" )
            >> xml::start( "C_BML_WhatWhen" )
                >> xml::start( "WhatWhenInstance" )
                    >> xml::start( "ActionTask" )
                        >> xml::content( "jc3iedm:ActivityCode", code )
                    >> xml::end()
                >> xml::end()
            >> xml::end()
        >> xml::end();
    return GetMissionByName( model_.GetMissionTypes(), MissionNameFromCode( code ) );
}

namespace
{
    struct NameFinder
    {
        explicit NameFinder( const std::string& name ) : name_( name ), result_( 0 ) {}
        void operator()( const dispatcher::Automat& entity )
        {
            if( entity.strName_ == name_ )
                result_ = &entity;
        }
        std::string name_;
        const dispatcher::Automat* result_;
    };
}

// -----------------------------------------------------------------------------
// Name: Mission::ReadTaskeeWho
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::ReadTaskeeWho( xml::xistream& xis )
{
    std::string name;
    xis >> xml::start( "TaskeeWho" )
            >> xml::start( "C_BML_Who" )
                >> xml::start( "WhoRef" )
                    >> xml::start( "UnitRef" )
                        >> xml::content( "jc3iedm::OID", name )
                    >> xml::end()
                >> xml::end()
            >> xml::end()
        >> xml::end();
    NameFinder finder( name );
    model_.GetAutomats().Apply( finder );
    taskee_ = finder.result_;
    if( !taskee_ )
        throw std::runtime_error( __FUNCTION__ ": Unable to resolve TaskeeWho" );
}

// -----------------------------------------------------------------------------
// Name: Mission::ReadParameters
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::ReadParameters( xml::xistream& xis )
{
    xis >> xml::start( "TaskControlMeasures" )
            >> xml::list( "TaskControlMeasure", *this, &Mission::ReadParameter )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Mission::ReadParameter
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::ReadParameter( xml::xistream& xis )
{
    MissionParameter_ABC* parameter = factory_->CreateParameter( xis, type_ );
    if( !parameter )
        throw std::runtime_error( __FUNCTION__ ": Error creating mission parameter" );
    AddParameter( *parameter );
}

// -----------------------------------------------------------------------------
// Name: Mission::AddParameter
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::AddParameter( MissionParameter_ABC& parameter )
{
    if( parameter.GetType().GetName() == "phaselinelist" )
    {
        MissionParameterPhaseLines* lines = 0;
        for( T_Parameters::iterator it = parameters_.begin(); it != parameters_.end(); ++it )
            if( &(*it)->GetType() == &parameter.GetType() )
            {
                lines = static_cast< MissionParameterPhaseLines* >( *it );
                break;
            }
        if( !lines )
            lines = new MissionParameterPhaseLines( parameter.GetType() );
        lines->AddLine( static_cast< MissionParameterPhaseLine& >( parameter ) );
    }
    else
        parameters_.push_back( &parameter );
}

// -----------------------------------------------------------------------------
// Name: Mission::Send
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::Send( dispatcher::SimulationPublisher_ABC& publisher ) const
{
    dispatcher::AsnMsgClientToSimAutomatOrder asn;
    asn().oid = taskee_->GetID();
    asn().mission = type_.GetId();
    asn().formation = EnumAutomatOrderFormation::deux_echelons;
    asn().parametres.n = type_.Count();
    if( asn().parametres.n > 0 )
        asn().parametres.elem = new ASN1T_MissionParameter[ asn().parametres.n ];
    Serialize( asn().parametres );
    asn.Send( publisher );
    Clean( asn().parametres );
}

namespace
{
    int GetParameterIndex( const kernel::MissionType& mission, const kernel::OrderParameter& param )
    {
        kernel::Iterator< const kernel::OrderParameter& > it( mission.CreateIterator() );
        for( unsigned int i = 0; it.HasMoreElements(); ++i )
        {
            const kernel::OrderParameter& parameter = it.NextElement();
            if( &parameter == &param )
                return i;
        }
        return -1;
    }
}

// -----------------------------------------------------------------------------
// Name: Mission::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::Serialize( ASN1T_MissionParameters& asn ) const
{
    for( unsigned int i = 0; i < asn.n; ++i )
        asn.elem[i].null_value = 1;
    for( T_Parameters::const_iterator it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        const unsigned int index = GetParameterIndex( type_, (*it)->GetType() );
        if( index >= 0 )
            (*it)->Serialize( asn.elem[index] );
    }
}

// -----------------------------------------------------------------------------
// Name: Mission::Clean
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::Clean( ASN1T_MissionParameters& asn ) const
{
    for( T_Parameters::const_iterator it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        const unsigned int index = GetParameterIndex( type_, (*it)->GetType() );
        if( index >= 0 )
            (*it)->Clean( asn.elem[index] );
    }
    delete[] asn.elem;
}
