// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "SymbolEditor.h"
#include "ScopeEditor.h"

#include "AgentType.h"
#include "AutomatType.h"
#include "dispatcher/Side.h"
#include "dispatcher/ModelWalker.h"
#include "dispatcher/Model.h"
#include "dispatcher/ModelVisitors.h"

//#include <algorithm>
//#include "MT/MT_Tools/MT_StringHelpers.h"

using namespace dispatcher;
using namespace crossbow;


// -----------------------------------------------------------------------------
// Name: SymbolEditor::SymbolEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
SymbolEditor::SymbolEditor( ScopeEditor& scope, const Model& model )
    : scope_ ( scope )
    , model_ ( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::~SymbolEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
SymbolEditor::~SymbolEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgLimitCreation& msg )
{
    std::string symbol( "G*GP********--X" );
    dispatcher::ModelWalker walker( model_ );

    {
        dispatcher::SideModelVisitor visitor( walker );
        walker.WalkTo( visitor, msg.tactical_line );
        UpdateAffiliation( symbol, visitor.GetSide() );
    }
    {
        dispatcher::FormationModelVisitor visitor( walker );
        walker.WalkTo( visitor, msg.tactical_line );
        UpdateEchelon( symbol, visitor.GetEchelon() );
    }
    UpdateFunction( symbol, "GLB---" );
    scope_.Write( spFeature, CComBSTR( L"Symbol_ID" ), BuildSymbol( symbol ).c_str() );  // $$$$ JCR 2007-05-15: TODO - Level
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgLimaCreation& msg )
{
    std::string symbol( "G*GP********--X" );
    dispatcher::ModelWalker walker( model_ );

    {
        dispatcher::SideModelVisitor visitor( walker );
        walker.WalkTo( visitor, msg.tactical_line );
        UpdateAffiliation( symbol, visitor.GetSide() );
    }
    UpdateFunction( symbol, "GLP---" );
    // Fire support, coordination line UpdateFunction( symbol, "OLF---" );
    // line of departure UpdateFunction( symbol, "OLT---" );
    // line of contact UpdateFunction( symbol, "OLC---" );
    scope_.Write( spFeature, CComBSTR( L"Symbol_ID" ), BuildSymbol( symbol ).c_str() );  // $$$$ JCR 2007-05-15: TODO - Level
}

//struct EXTERN EnumObjectType {
//   enum Root {
//      bouchon_mines = 0,
//      zone_minee_lineaire = 1,
//      zone_minee_par_dispersion = 2,
//      fosse_anti_char = 3,
//      abattis = 4,
//      barricade = 5,
//      eboulement = 6,
//      destruction_route = 7,
//      destruction_pont = 8,
//      pont_flottant_continu = 9,
//      poste_tir = 10,
//      zone_protegee = 11,
//      zone_implantation_canon = 12,
//      zone_implantation_cobra = 13,
//      zone_implantation_lrm = 14,
//      site_franchissement = 15,
//      nuage_nbc = 16,
//      plot_ravitaillement = 17,
//      site_decontamination = 18,
//      zone_brouillage_brod = 19,
//      rota = 20,
//      zone_nbc = 21,
//      zone_brouillage_bromure = 22,
//      aire_poser = 23,
//      piste = 24,
//      plateforme = 25,
//      zone_mobilite_amelioree = 26,
//      zone_poser_helicoptere = 27,
//      aire_logistique = 28,
//      itineraire_logistique = 29,
//      camp_prisonniers = 30,
//      camp_refugies = 31,
//      poste_controle = 32,
//      terrain_largage = 33,
//      zone_interdite_mouvement = 34,
//      zone_interdite_tir = 35,
//      zone_implantation_mortier = 36,
//      pont_flottant_discontinu = 37,
//      installation = 38
//   } ;
//} ;

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgObjectCreation& msg )
{
    dispatcher::ModelWalker walker( model_ );
    std::string symbol( "G*MP********--X" );

    switch ( msg.type )
    {
    case EnumObjectType::abattis:                   UpdateFunction( symbol, "OS----" ); break;
    case EnumObjectType::bouchon_mines:             UpdateFunction( symbol, "OMC---" ); break;
    case EnumObjectType::zone_minee_lineaire:
        symbol.assign( "G*AP********--X" );         UpdateFunction( symbol, "OFS---" ); break;

    case EnumObjectType::destruction_pont:
    case EnumObjectType::destruction_route:         UpdateFunction( symbol, "ORA---" ); break;

    case EnumObjectType::plot_ravitaillement:
    case EnumObjectType::zone_implantation_canon:
    case EnumObjectType::zone_implantation_cobra:
    case EnumObjectType::zone_implantation_lrm:
    case EnumObjectType::zone_implantation_mortier:
        symbol.assign( "G*GP********--X" );         UpdateFunction( symbol, "GPP---" ); break;
    }

    {
        dispatcher::SideModelVisitor visitor( walker );
        walker.WalkTo( visitor, msg );
        UpdateAffiliation( symbol, visitor.GetSide() );
    }
    // B14: pp 337
    // Abattis                      : UpdateFunction( symbol, "OS----" );
    // Antitank under construction  : UpdateFunction( symbol, "OADU--" );
    // Antitank complete            : UpdateFunction( symbol, "OADC--" );
    // Antitank mined complete      : UpdateFunction( symbol, "OAR---" );
    // Antitank wall                : UpdateFunction( symbol, "OAW---" );
    // mine                         : UpdateFunction( symbol, "OMU---" );
    // Antitank mine                : UpdateFunction( symbol, "OMT---" );
    // mine cluster                 : UpdateFunction( symbol, "OMC---" );
    // mine field                   : UpdateFunction( symbol, "OFS---" ); // with A instead of M in [3] for planned
    // bridge planned               : UpdateFunction( symbol, "ORP---" );
    // bridge                       : UpdateFunction( symbol, "ORS---" );
    // bridge destruction           : UpdateFunction( symbol, "ORA---" );
    // bridge destruction complete  : UpdateFunction( symbol, "ORC---" );

    // crossing site                : UpdateFunction( symbol, "BCA---" );
    // nuclear, bio, chemic         : UpdateFunction( symbol, "NR----" );
    // nuclear, bio, chemic         : UpdateFunction( symbol, "NB----" );
    // nuclear, bio, chemic         : UpdateFunction( symbol, "NC----" );

    // Indirect fire, impact point  : UpdateFunction( symbol, "GPWI--" ); // symbol( "G*GP********--X" );
    // Indirect fire, action point  : UpdateFunction( symbol, "GPP---" ); // symbol( "G*GP********--X" );
    scope_.Write( spFeature, CComBSTR( L"Symbol_ID" ), BuildSymbol( symbol ).c_str() );
}

namespace
{
    ASN1T_EnumNatureLevel   ConvertToNatureLevel( const std::string& nature )
    {
        if( nature == "o" )    { return EnumNatureLevel::o; }
        if( nature == "oo" )   { return EnumNatureLevel::oo; }
        if( nature == "ooo" )  { return EnumNatureLevel::ooo; }
        if( nature == "i" )    { return EnumNatureLevel::i; }
        if( nature == "ii" )   { return EnumNatureLevel::ii; }
        if( nature == "iii" )  { return EnumNatureLevel::iii; }
        if( nature == "x" )    { return EnumNatureLevel::x; }
        if( nature == "xx" )   { return EnumNatureLevel::xx; }
        if( nature == "xxx" )  { return EnumNatureLevel::xxx; }
        if( nature == "xxxx" ) { return EnumNatureLevel::xxxx; }
        return EnumNatureLevel::none;
    }
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-06-12
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IRowBufferPtr spRow, const ASN1T_MsgFormationCreation& asn )
{
    std::string         symbol( "S*GPU*******---" );

    {
        dispatcher::ModelWalker         walker( model_ );
        dispatcher::SideModelVisitor    visitor( walker );
        walker.WalkTo( visitor, asn );
        UpdateAffiliation( symbol, visitor.GetSide() );
    }
    UpdateEchelon( symbol, asn.niveau, false );
    scope_.Write( spRow, CComBSTR( L"Symbol_ID" ), BuildSymbol( symbol ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgUnitCreation& asn, const kernel::Resolver_ABC< kernel::AgentType >& resolver )
{
    const kernel::AgentType&    aType( resolver.Get( asn.type_pion ) );
    std::string                 symbol( ExtractCode( aType.GetSymbol() ) );
    dispatcher::ModelWalker     walker( model_ );

    symbol.append( 15 - symbol.size(), '*' );
    {
        dispatcher::SideModelVisitor    visitor( walker );
        walker.WalkTo( visitor, asn );
        UpdateAffiliation( symbol, visitor.GetSide() );
    }
    UpdateEchelon( symbol, ConvertToNatureLevel( ExtractCode( aType.GetLevelSymbol() ) ), asn.pc ? true : false );
    scope_.Write( spFeature, CComBSTR( L"Symbol_ID" ), BuildSymbol( symbol ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-06-12
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IRowBufferPtr spRow, const ASN1T_MsgAutomatCreation& asn, const kernel::Resolver_ABC< kernel::AutomatType >& resolver )
{
    const kernel::AutomatType&  aType( resolver.Get( asn.type_automate ) );
    std::string                 symbol( ExtractCode( aType.GetSymbol() ), 0, 7 );

    symbol.append( 15 - symbol.size(), '*' );
    {
        dispatcher::ModelWalker         walker( model_ );
        dispatcher::SideModelVisitor    visitor( walker );
        walker.WalkTo( visitor, asn );
        UpdateAffiliation( symbol, visitor.GetSide() );
    }
    if( aType.GetTypePC() )
    {
        ASN1T_EnumNatureLevel level = ConvertToNatureLevel( ExtractCode( aType.GetTypePC()->GetLevelSymbol() ) );
        UpdateEchelon( symbol, (ASN1T_EnumNatureLevel)((unsigned)level + 1), false );
    }
    scope_.Write( spRow, CComBSTR( L"Symbol_ID" ), BuildSymbol( symbol ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgUnitKnowledgeCreation& /*asn*/ )
{
    std::string         symbol( "SUZP********---" );

    scope_.Write( spFeature, CComBSTR( L"Symbol_ID" ), BuildSymbol( symbol ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::MakeKnowledgeSymbol
// Created: JCR 2007-06-11
// -----------------------------------------------------------------------------
const kernel::AgentType& SymbolEditor::RetrieveAgentType( const ASN1T_MsgUnitKnowledgeUpdate& asn, const kernel::Resolver_ABC< kernel::AgentType >& resolver )
{
    dispatcher::ModelWalker                 walker( model_ );
    dispatcher::UnitKnowledgeModelVisitor   visitor( walker );

    walker.WalkTo( visitor, asn );
    return resolver.Get( visitor.GetType() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgUnitKnowledgeUpdate& asn, const kernel::Resolver_ABC< kernel::AgentType >& resolver )
{
    std::string         symbol;

    symbol.append( 15 - symbol.size(), '*' );
    switch ( asn.max_identification_level )
    {
    case EnumUnitIdentificationLevel::identifiee:
        UpdateOnIdentification( symbol, asn, RetrieveAgentType( asn, resolver ) );
        break;
    case EnumUnitIdentificationLevel::reconnue:
        UpdateOnRecognition( symbol, asn, RetrieveAgentType( asn, resolver ) );
        break;
    case EnumUnitIdentificationLevel::detectee:
        UpdateOnDetection( symbol );
        break;
    case EnumUnitIdentificationLevel::signale:
        return;  // $$$$ JCR 2007-06-11:  should not be there ..
    }
    if( asn.identification_level == EnumUnitIdentificationLevel::signale )
        symbol[ E_Field::status ] = 'A'; // Present = 'P' - Planned or Anticipated = 'A'
    scope_.Write( spFeature, CComBSTR( L"Symbol_ID" ), BuildSymbol( symbol ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::UpdateIdentified
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::UpdateOnIdentification( std::string& symbol, const ASN1T_MsgUnitKnowledgeUpdate& asn, const kernel::AgentType& unit )
{
    dispatcher::ModelWalker walker( model_ );
    std::string stExtracted( ExtractCode( unit.GetSymbol() ) );

    symbol.replace( 0, stExtracted.size(), stExtracted );
    {
        dispatcher::SideModelVisitor    visitor( walker );
        walker.Visit( visitor, model_.GetSides().Get( asn.camp ) );
        UpdateAffiliation( symbol, visitor.GetSide() );
    }
    if( asn.m.nature_pcPresent )
        UpdateEchelon( symbol, ConvertToNatureLevel( ExtractCode( unit.GetLevelSymbol() ) ), asn.nature_pc ? true : false );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::UpdateOnRecognition
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::UpdateOnRecognition( std::string& symbol, const ASN1T_MsgUnitKnowledgeUpdate& asn, const kernel::AgentType& unit )
{
    dispatcher::ModelWalker         walker( model_ );

    symbol.replace( 0, 6, ExtractCode( unit.GetSymbol() ), 0, 6 );
    {
        dispatcher::SideModelVisitor    visitor( walker );
        walker.Visit( visitor, model_.GetSides().Get( asn.camp ) );
        UpdateAffiliation( symbol, visitor.GetSide() );
    }
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::UpdateDetected
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::UpdateOnDetection( std::string& symbol )
{
    symbol.assign( "SPZP********---" ); // Unknown Pending
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::ExtractCode
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
std::string SymbolEditor::ExtractCode( const std::string& symbol ) const
{
    const std::string::size_type    npos = symbol.find_first_of( '/' );
    std::string                     extracted( symbol, npos + 1, symbol.size() - npos );
    return extracted;
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::BuildSymbol
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
std::string SymbolEditor::BuildSymbol( const std::string& symbol ) const
{
    std::string result( symbol.size(), '*' );
    std::transform( symbol.begin(), symbol.end(), result.begin(), toupper );
    std::replace( result.begin(), result.end(), '*', '-' );
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::UpdateAffiliation
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
void SymbolEditor::UpdateAffiliation( std::string& symbol, ASN1T_EnumDiplomacy diplomatie ) const
{
    const char cDiplomatie[ 4 ] = { 'U' /*Unkown*/, 'F' /*Friend*/, 'H' /*Hostile*/, 'N' /*Neutral*/};

    if( symbol[ E_Field::affiliation ] == '*' )
        symbol[ E_Field::affiliation ] = cDiplomatie[ ( unsigned )diplomatie ];
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::UpdateFunction
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
void SymbolEditor::UpdateFunction( std::string& symbol, const std::string& type ) const
{
    if( symbol[ E_Field::function ] == '*' )
    {
        std::string stCode( type, 0, 6 );
        symbol.replace( E_Field::function, stCode.size(), stCode );
    }
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::GetLevel
// Created: JCR 2007-06-11
// -----------------------------------------------------------------------------
std::string SymbolEditor::GetLevel( ASN1T_EnumNatureLevel level )
{
    std::string stCode( "-" );

    switch( level )
    {
    case EnumNatureLevel::none:   stCode = "-"; break;
    case EnumNatureLevel::o:      stCode = "A"; break;
    case EnumNatureLevel::oo:     stCode = "C"; break;
    case EnumNatureLevel::ooo:    stCode = "D"; break;
    case EnumNatureLevel::i:      stCode = "E"; break;
    case EnumNatureLevel::ii:     stCode = "F"; break;
    case EnumNatureLevel::iii:    stCode = "G"; break;
    case EnumNatureLevel::x:      stCode = "H"; break;
    case EnumNatureLevel::xx:     stCode = "I"; break;
    case EnumNatureLevel::xxx:    stCode = "J"; break;
    case EnumNatureLevel::xxxx:   stCode = "K"; break;
    }
    return stCode;
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::UpdateEchelon
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
void SymbolEditor::UpdateEchelon( std::string& symbol, ASN1T_EnumNatureLevel level, bool pc ) const
{
    if( symbol[ E_Field::echelon ] == '*' )
    {
        std::string stCode( "-" + GetLevel( level ) );
        if( pc )
            stCode[ 0 ] = 'A';
        symbol.replace( E_Field::echelon, stCode.size(), stCode );
    }
}
