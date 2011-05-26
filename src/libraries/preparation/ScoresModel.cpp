// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ScoresModel.h"
#include "ModelChecker_ABC.h"
#include "Score_ABC.h"
#include "ScoreFactory_ABC.h"
#include "TeamsModel.h"
#include "Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter_ABC.h"
#include <boost/algorithm/string/replace.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ScoresModel constructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
ScoresModel::ScoresModel( const ScoreFactory_ABC& factory, const TeamsModel& teams, const tools::Resolver_ABC< kernel::DotationType >& dotations, const tools::Resolver_ABC< kernel::EquipmentType >& equipments )
    : factory_   ( factory )
    , teams_     ( teams )
    , dotations_ ( dotations )
    , equipments_( equipments )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoresModel destructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
ScoresModel::~ScoresModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Purge
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoresModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Load
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void ScoresModel::Load( const tools::Loader_ABC& fileLoader, const std::string& file )
{
    fileLoader.LoadFile( file, boost::bind( &ScoresModel::Read, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Read
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void ScoresModel::Read( xml::xistream& xis )
{
    xis >> xml::start( "scores" )
            >> xml::list( "score", *this, &ScoresModel::ReadScore )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::CheckValidity
// Created: SBO 2009-05-26
// -----------------------------------------------------------------------------
bool ScoresModel::CheckValidity( ModelChecker_ABC& checker, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    try
    {
        xml::xostringstream xos;
        Serialize( xos, schemaWriter );
    }
    catch( std::exception& e )
    {
        return checker.Reject( tools::translate( "ScoresModel", "Score definitions contain errors:\nReason: %1." ).arg( e.what() ) );
    }
    return checker.Validate();
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Serialize
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void ScoresModel::Serialize( const std::string& file, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    xml::xofstream xos( file );
    Serialize( xos, schemaWriter );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Serialize
// Created: SBO 2009-05-26
// -----------------------------------------------------------------------------
void ScoresModel::Serialize( xml::xostream& xos, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    xos << xml::start( "scores" );
    schemaWriter.WriteExerciseSchema( xos, "scores" );
    BOOST_FOREACH( const T_Elements::value_type factor, elements_ )
        factor.second->Serialize( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::GenerateScoresFromTemplate
// Created: JSR 2011-02-08
// -----------------------------------------------------------------------------
void ScoresModel::GenerateScoresFromTemplate( const tools::Loader_ABC& fileLoader )
{
    fileLoader.LoadPhysicalFile( "scores", boost::bind( &ScoresModel::ReadTemplate, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::ReadTemplate
// Created: JSR 2011-02-08
// -----------------------------------------------------------------------------
void ScoresModel::ReadTemplate( xml::xistream& xis )
{
    xis >> xml::start( "scores" )
            >> xml::list( "score", *this, &ScoresModel::ReadScore )
            >> xml::list( "foreach", *this, &ScoresModel::ReadForeach );
}

namespace
{
    void AddUnitToList( std::string& unitList, const kernel::Entity_ABC& entity )
    {
        if( const kernel::Agent_ABC* agent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        {
            if( !unitList.empty() )
                unitList += ',';
            unitList += boost::lexical_cast< std::string >( agent->GetId() );
        }
        else if( const kernel::Hierarchies* h = entity.Retrieve< kernel::TacticalHierarchies >() )
        {
            tools::Iterator< const kernel::Entity_ABC& > children = h->CreateSubordinateIterator();
            while( children.HasMoreElements() )
                AddUnitToList( unitList, children.NextElement() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::ReadForeach
// Created: JSR 2011-02-08
// -----------------------------------------------------------------------------
void ScoresModel::ReadForeach( xml::xistream& xis )
{
    std::string source;
    std::string variable;
    std::string partiesReplacementString;
    std::string ammunitionReplacementString;
    std::string equipmentReplacementString;
    xis >> xml::attribute( "source", source )
        >> xml::attribute( "variable", variable );

    if( source == "party" )
        partiesReplacementString = variable;
    if( source == "ammunition" )
        ammunitionReplacementString = variable;
    if( source == "equipment" )
        equipmentReplacementString = variable;
    while( xis.has_child( "foreach" ) )
    {
        xis >> xml::start( "foreach" )
            >> xml::attribute( "source", source )
            >> xml::attribute( "variable", variable );
        if( source == "party" )
            partiesReplacementString = variable;
        if( source == "ammunition" )
            ammunitionReplacementString = variable;
        if( source == "equipment" )
            equipmentReplacementString = variable;
    }
    if( xis.has_child( "score" ) )
    {
        std::string name;
        xis >> xml::start( "score" )
            >> xml::attribute( "name", name );
        boost::shared_ptr< xml::xibufferstream > pXis( new xml::xibufferstream( xis ) );
        xis >> xml::end;
        xml::xostringstream xos;
        xos << xml::start( "score" )
            << *pXis
            << xml::end;
        const std::string content = xos.str();
        if( !ammunitionReplacementString.empty() )
            IterateAmmunitions( content, ammunitionReplacementString, name, partiesReplacementString );
        else if( !equipmentReplacementString.empty() )
            IterateEquipments( content, equipmentReplacementString, name, partiesReplacementString );
        else if( !partiesReplacementString.empty() )
            IterateParties( content, partiesReplacementString, name );
        else
            CreateScoreFromString( content );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::IterateAmmunitions
// Created: JSR 2011-02-09
// -----------------------------------------------------------------------------
void ScoresModel::IterateAmmunitions( const std::string& content, const std::string& toReplace, const std::string& name, const std::string& toReplaceParties )
{
    std::map< const std::string, std::string > ammunitionMap;
    tools::Iterator< const kernel::DotationType& > it = dotations_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::DotationType& dotation = it.NextElement();
        if( dotation.IsAmmunition() && !dotation.GetType().empty() )
            ammunitionMap[ dotation.GetType() ] += boost::lexical_cast< std::string >( dotation.GetId() ) + ",";
    }
    for( std::map< const std::string, std::string >::iterator it = ammunitionMap.begin(); it != ammunitionMap.end(); ++it )
    {
        it->second.erase( it->second.length() - 1 );
        std::string newName = name + " (" + it->first + ")";
        std::string newContent = content;
        boost::algorithm::replace_all( newContent, name, newName);
        boost::algorithm::replace_all( newContent, toReplace, it->second );
        if( toReplaceParties.empty() )
            CreateScoreFromString( newContent );
        else
            IterateParties( newContent, toReplaceParties, newName );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::IterateEquipments
// Created: JSR 2011-02-09
// -----------------------------------------------------------------------------
void ScoresModel::IterateEquipments( const std::string& content, const std::string& toReplace, const std::string& name, const std::string& toReplaceParties )
{
    std::map< const std::string, std::string > equipmentMap;
    tools::Iterator< const kernel::EquipmentType& > it = equipments_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::EquipmentType& equipment = it.NextElement();
        equipmentMap[ equipment.GetProtection() ] += boost::lexical_cast< std::string >( equipment.GetId() ) + ",";
    }
    for( std::map< const std::string, std::string >::iterator it = equipmentMap.begin(); it != equipmentMap.end(); ++it )
    {
        it->second.erase( it->second.length() - 1 );
        std::string newName = name + " (" + it->first + ")";
        std::string newContent = content;
        boost::algorithm::replace_all( newContent, name, newName);
        boost::algorithm::replace_all( newContent, toReplace, it->second );
        if( toReplaceParties.empty() )
            CreateScoreFromString( newContent );
        else
            IterateParties( newContent, toReplaceParties, newName );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::IterateParties
// Created: JSR 2011-02-09
// -----------------------------------------------------------------------------
void ScoresModel::IterateParties( const std::string& content, const std::string& toReplace, const std::string& name )
{
    tools::Iterator< const kernel::Entity_ABC& > it = teams_.CreateEntityIterator();
    while( it.HasMoreElements() )
    {
        std::string unitList;
        const kernel::Entity_ABC& entity = it.NextElement();
        AddUnitToList( unitList, entity );
        if( !unitList.empty() )
        {
            std::string newContent = content;
            std::string newName = name + " (" + entity.GetName().ascii() + ")";
            boost::algorithm::replace_all( newContent, name, newName);
            boost::algorithm::replace_all( newContent, toReplace, unitList );
            CreateScoreFromString( newContent );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::CreateScoreFromString
// Created: JSR 2011-02-09
// -----------------------------------------------------------------------------
void ScoresModel::CreateScoreFromString( const std::string& buffer )
{
    std::string scoreName;
    xml::xistringstream stringStream( buffer );
    stringStream >> xml::start( "score" )
                 >> xml::attribute( "name", scoreName );
    if( Score_ABC* element = Find( scoreName.c_str() ) )
    {
        Remove( scoreName.c_str() );
        delete element;
    }
    ReadScore( stringStream );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::ReadScore
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void ScoresModel::ReadScore( xml::xistream& xis )
{
    Score_ABC* score = factory_.CreateScore( xis );
    Register( score->GetName(), *score );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Create
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoresModel::Create( const QString& name )
{
    Score_ABC* score = factory_.CreateScore( name );
    Register( score->GetName(), *score );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Delete
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoresModel::Delete( const Score_ABC& score )
{
    const QString name = score.GetName();
    if( Score_ABC* element = Find( name ) )
    {
        Remove( name );
        delete element;
    }
}
