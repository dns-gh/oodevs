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
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLine constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterPhaseLine::MissionParameterPhaseLine( xml::xistream& xis, const kernel::OrderParameter& type, const std::string& functions )
    : MissionParameter_ABC( type )
    , points_( 0 )
    , functions_( functions )
{
    xis >> xml::start( "Line" );
    points_.reset( new PointList( xis ) );
    xis >> xml::end();
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
void MissionParameterPhaseLine::Serialize( ASN1T_MissionParameter& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLine::Clean
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPhaseLine::Clean( ASN1T_MissionParameter& ) const
{
    // NOTHING
}

namespace
{
    ASN1T_EnumLimaType ToPhaseLineType( const std::string& function )
    {
        if( function == "LD" )
            return EnumLimaType::ligne_debouche;
        if( function == "LCA" )
            return EnumLimaType::ligne_changement_attitude;
        if( function == "LC" )
            return EnumLimaType::ligne_coordination;
        if( function == "LI" )
            return EnumLimaType::ligne_interdire;
        if( function == "LO" )
            return EnumLimaType::ligne_objectif;
        if( function == "LCAR" )
            return EnumLimaType::ligne_coup_arret;
        if( function == "LR" )
            return EnumLimaType::ligne_recueil;
        if( function == "LDM" )
            return EnumLimaType::ligne_debut_mission;
        if( function == "LFM" )
            return EnumLimaType::ligne_fin_mission;
        if( function == "LIA" )
            return EnumLimaType::ligne_identification_accueil;
        return ASN1T_EnumLimaType( -1 );
    }

    void SerializeFunctions( ASN1T__SeqOfEnumLimaType& asn, const std::string& functions )
    {
        asn.n = 1;
        asn.elem = new ASN1T_EnumLimaType[asn.n];
        asn.elem[0] = ToPhaseLineType( functions );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLine::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPhaseLine::Serialize( ASN1T_LimaOrder& asn ) const
{
    SerializeFunctions( asn.fonctions, functions_ );
    points_->Serialize( asn.lima.coordinates );
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLine::Clean
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPhaseLine::Clean( ASN1T_LimaOrder& asn ) const
{
    delete[] asn.fonctions.elem;
}
