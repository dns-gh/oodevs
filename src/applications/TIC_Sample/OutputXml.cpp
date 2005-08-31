// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-19 $
// $Archive: /MVW_v10/Build/SDK/TIC_Sample/src/OutputXml.cpp $
// $Author: Sbo $
// $Modtime: 20/07/05 18:10 $
// $Revision: 24 $
// $Workfile: OutputXml.cpp $
//
// *****************************************************************************

#include "Types.h"

#include "OutputXml.h"
#include "TIC_Export/Workspace.h"
#include "TIC_Export/Automat.h"
#include "TIC_Export/Pawn.h"
#include "TIC_Export/Platform.h"
#include "TIC_Export/Communication.h"
#include "TIC_Export/CommunicationLink.h"

using namespace TICExport;

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputXml
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
OutputXml::OutputXml( const std::string& strFileName )
    : Callback_ABC()
    , strFilename_( strFileName )
    , pWorkspace_ ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: OutputXml::~OutputXml
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
OutputXml::~OutputXml()
{
}


// -----------------------------------------------------------------------------
// Name: OutputXml::SetWorkspace
// Created: NLD 2005-06-08
// -----------------------------------------------------------------------------
void OutputXml::SetWorkspace( const Workspace& workspace )
{
    pWorkspace_ = &workspace;
}

// -----------------------------------------------------------------------------
// Name: OutputXml::OnReceiveEndTick
// Created: NLD 2005-06-08
// -----------------------------------------------------------------------------
void OutputXml::OnReceiveEndTick()
{
    Save();
}

//-----------------------------------------------------------------------------
// Name: OutputXml::Save
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void OutputXml::Save()
{
    assert( pWorkspace_ );

    archive_.open( strFilename_.c_str(), std::ios_base::out | std::ios_base::trunc );
 
    if( !archive_.is_open() )
    {
        std::cerr << "Cannot open file for output: " << strFilename_ << std::cerr;
        return;
    }

    archive_ << "<Tic xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" <<
                " xsi:noNamespaceSchemaLocation=\"tic.xsd\">" << std::endl;

    std::string strCurrentTime = pWorkspace_->GetSimIsoTime();
    archive_ << "  <HeureSIM>" << strCurrentTime <<
                "</HeureSIM>"  << std::endl;

    uint nElementCount = pWorkspace_->GetAutomatCount();

    archive_ << "  <Automates>" << std::endl;
    for( uint i = 0; i < nElementCount; i++ )
        OutputAutomat( pWorkspace_->GetAutomatAt( i ) );
    archive_ << "  </Automates>" << std::endl;

    archive_ << "</Tic>" << std::endl;

    archive_.close();

    std::cout << "Export done: " << strCurrentTime << std::endl;
}

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputAutomat
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void OutputXml::OutputAutomat( const Automat& automat )
{
    const std::string strIndent = "    ";
    archive_ << strIndent << "<Automate id=\"" << automat.GetId() << "\">" << std::endl;
    archive_ << strIndent << "  <Camp>" << automat.GetTeam() << "</Camp>" << std::endl;
    archive_ << strIndent << "  <GroupeConnaissance>" << automat.GetKnowledgeGroup() << "</GroupeConnaissance>" << std::endl;

    OutputPawn( automat.GetPc() );

    uint nNbrPawns = automat.GetPawnCount();
    if( nNbrPawns > 0 )
    {
        archive_ << strIndent << "  <Pions>" << std::endl;
        for( uint i = 0; i < nNbrPawns; i++ )
        {
            const Pawn& pawn = automat.GetPawnAt( i );
            if( pawn.IsPc() )
                continue;
            OutputPawn( pawn );
        }
        archive_ << strIndent << "  </Pions>" << std::endl;
    }
    archive_ << strIndent << "</Automate>" << std::endl;
}

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputPawn
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void OutputXml::OutputPawn( const Pawn& pawn )
{
    const std::string strIndent = "      ";

    archive_ << strIndent << "<" << (pawn.IsPc() ? "PC" : "Pion" );
    archive_ << " id=\"" << pawn.GetId() << "\"";
    archive_ << " type=\"agrege\">" << std::endl;

    archive_ << strIndent << "  <Nature>" << std::endl;
    archive_ << strIndent << "    <Nom>" << pawn.GetNatureName() << "</Nom>" << std::endl;
    archive_ << strIndent << "    <Niveau>" << pawn.GetNatureLevel() << "</Niveau>" << std::endl;
    archive_ << strIndent << "    <Arme>" << pawn.GetNatureWeapon() << "</Arme>" << std::endl;
    archive_ << strIndent << "    <Specialisation>" << pawn.GetNatureSpecialization() << "</Specialisation>" << std::endl;
    archive_ << strIndent << "    <Categorie>" << pawn.GetNatureCategory() << "</Categorie>" << std::endl;
    archive_ << strIndent << "    <Qualificatif>" << pawn.GetNatureQualifier() << "</Qualificatif>" << std::endl;
    archive_ << strIndent << "    <TypeMobilite>" << pawn.GetNatureMobility() << "</TypeMobilite>" << std::endl;
    archive_ << strIndent << "    <CapaciteMission>" << pawn.GetNatureCapacity() << "</CapaciteMission>" << std::endl;
    archive_ << strIndent << "  </Nature>" << std::endl;

    archive_ << strIndent << "  <Direction>" << pawn.GetDirection() << "</Direction> " << std::endl;
    archive_ << strIndent << "  <Vitesse>" << pawn.GetSpeed() << "</Vitesse> " << std::endl;
    archive_ << strIndent << "  <Position>" << std::endl;
    archive_ << strIndent << "    <Latitude>" << pawn.GetLatitude() << "</Latitude> " << std::endl;
    archive_ << strIndent << "    <Longitude>" << pawn.GetLongitude() << "</Longitude> " << std::endl;
    archive_ << strIndent << "    <Hauteur>" << pawn.GetHeight() << "</Hauteur> " << std::endl;
    archive_ << strIndent << "  </Position>" << std::endl;

    std::string strState;
    switch( pawn.GetState() )
    {
    case Pawn::eStateOperational:
        strState = "Operationnel";
        break;
    case Pawn::eStateFixable:
        strState = "Reparable";
        break;
    case Pawn::eStateDestroyed:
        strState = "Detruit";
        break;
    default:
        break;
    }

    archive_ << strIndent << "  <Etat>" << strState << "</Etat>" << std::endl;

    uint nNbrPlatforms = pawn.GetPlatformCount();
    if( nNbrPlatforms > 0 )
    {
        archive_ << strIndent << "  <Plateformes>" << std::endl;
        for( uint i = 0; i < nNbrPlatforms; i++ )
            OutputPlatform( pawn.GetPlatformAt( i ) );
        archive_ << strIndent << "  </Plateformes>" << std::endl;
    }

    archive_ << strIndent << "</" << (pawn.IsPc() ? "PC" : "Pion" ) << ">" << std::endl;
}

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputPlatform
// Created: SBO 2005-07-13
//-----------------------------------------------------------------------------
void OutputXml::OutputPlatform( const Platform& platform )
{
    const std::string strIndent = "          ";

    archive_ << strIndent << "<Plateforme id=\"" << platform.GetId() << "\">" << std::endl;
    archive_ << strIndent << "  <Type>"       << platform.GetTypeName()       << "</Type>"       << std::endl;
    archive_ << strIndent << "  <CodeEMAT6>"  << platform.GetTypeEMAT6()      << "</CodeEMAT6>"  << std::endl;
    archive_ << strIndent << "  <CodeEMAT8>"  << platform.GetTypeEMAT8()      << "</CodeEMAT8>"  << std::endl;
    archive_ << strIndent << "  <CodeLFRIL>"  << platform.GetTypeLFRIL()      << "</CodeLFRIL>"  << std::endl;
    archive_ << strIndent << "  <CodeNNO>"    << platform.GetTypeNNO()        << "</CodeNNO>"    << std::endl;
    archive_ << strIndent << "  <Protection>" << platform.GetTypeProtection() << "</Protection>" << std::endl;
    archive_ << strIndent << "  <Volume>"     << platform.GetTypeVolume()     << "</Volume>"     << std::endl;
    archive_ << strIndent << "  <Direction>"  << platform.GetDirection()      << "</Direction>"  << std::endl;
    archive_ << strIndent << "  <Vitesse>"    << platform.GetSpeed()          << "</Vitesse>"    << std::endl;
    archive_ << strIndent << "  <Position>"    << std::endl;
    archive_ << strIndent << "    <Latitude>"  << platform.GetLatitude()      << "</Latitude>"  << std::endl;
    archive_ << strIndent << "    <Longitude>" << platform.GetLongitude()     << "</Longitude>" << std::endl;
    archive_ << strIndent << "    <Hauteur>"   << platform.GetHeight()        << "</Hauteur>"   << std::endl;
    archive_ << strIndent << "  </Position>"   << std::endl;
    archive_ << strIndent << "  <NatureEquipement type=\"" << platform.GetDevice()            << "\">"              << std::endl;
    archive_ << strIndent << "    <Categorie>"             << platform.GetDeviceCategory()    << "</Categorie>"     << std::endl;
    archive_ << strIndent << "    <SousCategorie>"         << platform.GetDeviceSubCategory() << "</SousCategorie>" << std::endl;
    archive_ << strIndent << "  </NatureEquipement>"       << std::endl;
    archive_ << strIndent << "</Plateforme>"               << std::endl;
}

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputCommunicationLink
// Created: SBO 2005-07-13
//-----------------------------------------------------------------------------
void OutputXml::OutputCommunicationLink( const CommunicationLink& link )
{
    const std::string strIndent = "    ";
    archive_ << strIndent << "<Liaison "
             << " automate=\"" << link.GetAutomatId() << "\" "
             << " pion=\""     << link.GetPawnId   () << "\">";
    
    uint nComNbr = link.GetCommunicationCount( CommunicationLink::eSourcePawn );
    archive_ << strIndent << "  <PionVersAutomate>"  << std::endl;
    for( uint i = 0; i < nComNbr; ++i )
        OutputCommunication( link.GetCommunicationAt( CommunicationLink::eSourcePawn, i ) );
    archive_ << strIndent << "  </PionVersAutomate>" << std::endl;

    nComNbr = link.GetCommunicationCount( CommunicationLink::eSourceAutomat );
    archive_ << strIndent << "  <AutomateVersPion>"  << std::endl;
    for( uint i = 0; i < nComNbr; ++i )
        OutputCommunication( link.GetCommunicationAt( CommunicationLink::eSourceAutomat, i ) );
    archive_ << strIndent << "  </AutomateVersPion>" << std::endl;

    archive_ << strIndent << "</Liaison>" << std::endl;
}

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputCommunication
// Created: SBO 2005-07-13
//-----------------------------------------------------------------------------
void OutputXml::OutputCommunication( const Communication& communication )
{
    if( communication.GetArity() <= 0 )
        return;

    const std::string strIndent = "        ";
    archive_ << strIndent << "<Communication "
             << " type=\""     << communication.GetType   () << "\""
             << " soustype=\"" << communication.GetSubType() << "\""
             << " nombre=\""   << communication.GetArity  () << "\"";
             //<< " sujet=\""    << communication.GetSubject() << "\"";

    if( communication.IsReport() )
        archive_ << " message=\"" << communication.GetMessage() << "\"";
    archive_ << "/>" << std::endl;
}