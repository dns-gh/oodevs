// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterPhaseLine.h"
#include "PointList.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 1 )
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;
using namespace plugins::bml;

using namespace sword;

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLine constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterPhaseLine::MissionParameterPhaseLine( xml::xistream& xis, const kernel::OrderParameter& type, const std::string& functions )
    : MissionParameter_ABC( type )
    , points_( 0 )
    , functions_( functions )
{
    xis >> xml::start( NS( "Line", "cbml" ) );
    points_.reset( new PointList( xis ) );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLine destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterPhaseLine::~MissionParameterPhaseLine()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLine::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPhaseLine::Serialize( MissionParameter& ) const
{
    // NOTHING
}

namespace
{
    LimaOrder_Function ToPhaseLineType( const std::string& function )
    {
        if( function == "LD" )
            return LimaOrder_Function_ligne_debouche;
        if( function == "LCA" )
            return LimaOrder_Function_ligne_changement_attitude;
        if( function == "LC" )
            return LimaOrder_Function_ligne_coordination;
        if( function == "LI" )
            return LimaOrder_Function_ligne_interdire;
        if( function == "LO" )
            return LimaOrder_Function_ligne_objectif;
        if( function == "LCAR" )
            return LimaOrder_Function_ligne_coup_arret;
        if( function == "LR" )
            return LimaOrder_Function_ligne_recueil;
        if( function == "LDM" )
            return LimaOrder_Function_ligne_debut_mission;
        if( function == "LFM" )
            return LimaOrder_Function_ligne_fin_mission;
        if( function == "LIA" )
            return LimaOrder_Function_ligne_identification_accueil;
        return LimaOrder_Function( -1 );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLine::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPhaseLine::Serialize( LimaOrder& message ) const
{
    std::vector< std::string > functions;
    boost::algorithm::split( functions, functions_, boost::algorithm::is_any_of( "," ) );
    for( std::vector< std::string >::const_iterator it = functions.begin(); it != functions.end(); ++it )
        message.add_fonctions( ToPhaseLineType( *it ) );
    message.mutable_lima()->mutable_location()->set_type( Location_Geometry_line );
    points_->Serialize( *message.mutable_lima()->mutable_location()->mutable_coordinates() );
    message.mutable_horaire()->set_data( bpt::to_iso_string( bpt::from_time_t( 0 ) ).c_str() );
}
