// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_Mission.h"
#include "ADN_Missions_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_ConsistencyChecker.h"
#include "ADN_WorkspaceElement.h"
#include <tools/Helpers.h>
#include <tools/Loader_ABC.h>
#include <boost/regex.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <queue>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

// =============================================================================
// Missions
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::Mission
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
ADN_Missions_Mission::ADN_Missions_Mission()
    : ADN_Missions_ABC()
{
    assert( false ); // $$$$ ABR 2013-08-23: useless constructor, needed by ADN_Wizard...
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::Mission
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
ADN_Missions_Mission::ADN_Missions_Mission( E_MissionType type )
    : ADN_Missions_ABC( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Mission::ADN_Missions_Mission( E_MissionType type, unsigned int id )
    : ADN_Missions_ABC( type, id )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::~Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Mission::~ADN_Missions_Mission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::CreateCopy
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Mission* ADN_Missions_Mission::CreateCopy()
{
    ADN_Missions_Mission* newMission     = new ADN_Missions_Mission( type_ );
    newMission->diaBehavior_             = diaBehavior_.GetData();
    newMission->cdtDiaBehavior_          = cdtDiaBehavior_.GetData();
    newMission->mrtDiaBehavior_          = mrtDiaBehavior_.GetData();
    newMission->missionSheetPath_        = missionSheetPath_.GetData();
    newMission->symbol_                  = symbol_.GetData();
    newMission->parameters_.reserve( parameters_.size() );
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        ADN_Missions_Parameter* newParam = (*it)->CreateCopy();
        newMission->parameters_.AddItem( newParam );
    }
    newMission->descriptionContext_ = descriptionContext_.GetData();
    newMission->descriptionBehavior_ = descriptionBehavior_.GetData();
    newMission->descriptionSpecific_ = descriptionSpecific_.GetData();
    newMission->descriptionComment_ = descriptionComment_.GetData();
    newMission->descriptionMissionEnd_ = descriptionMissionEnd_.GetData();
    newMission->attachments_ = attachments_;
    return newMission;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::ReadArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadArchive( xml::xistream& input )
{
    std::string missionSheetDesc, symbol;
    ADN_Missions_ABC::ReadArchive( input );
    input >> xml::optional >> xml::attribute( "symbol", symbol )
          >> xml::optional >> xml::attribute( "dia-behavior", diaBehavior_ )
          >> xml::optional >> xml::attribute( "cdt-dia-behavior", cdtDiaBehavior_ )
          >> xml::optional >> xml::attribute( "mrt-dia-behavior", mrtDiaBehavior_ )
          >> xml::optional >> xml::attribute( "package", strPackage_ )
          >> xml::optional >> xml::attribute( "package", strPackage_ )
          >> xml::optional >> xml::start( "description" )
            >> xml::optional >> xml::attribute( "doctrine", doctrine_ )
            >> xml::optional >> xml::attribute( "usage", usage_ )
          >> xml ::end
          >> xml::list( "parameter", boost::bind( &ADN_Missions_ABC::ReadParameter, this, _1 ) );
    const std::string code = symbol.empty() ? " - " : symbol;
    ADN_Drawings_Data& drawings = ADN_Workspace::GetWorkspace().GetDrawings().GetData();
    symbol_.SetVector( drawings.GetCategoryDrawings( "tasks" ) );
    symbol_.SetData( drawings.GetDrawing( code ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::FixConsistency
// Created: ABR 2013-10-10
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::FixConsistency()
{
    ADN_Missions_ABC::FixConsistency();
    bool isAutomat = type_ == eMissionType_Automat;
    const QString typeName = type_ == eMissionType_Pawn ? "Pion" : ( isAutomat ? "Automate" : "Population" );
    if( !isAutomat )
    {
        if( diaBehavior_.GetData().empty() )
            diaBehavior_ = QString( "MIS_%1_%2" ).arg( typeName ).arg( diaType_.GetData().c_str() ).toStdString();
    }
    else
    {
        if( cdtDiaBehavior_.GetData().empty() )
            cdtDiaBehavior_ = QString( "MIS_%1_CDT_%2" ).arg( typeName ).arg( diaType_.GetData().c_str() ).toStdString();
        if( mrtDiaBehavior_.GetData().empty() )
            mrtDiaBehavior_ = QString( "MIS_%1_MRT_%2" ).arg( typeName ).arg( diaType_.GetData().c_str() ).toStdString();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "mission" );
    ADN_Missions_ABC::WriteArchive( output );
    const std::string code = ( symbol_.GetData() ) ? symbol_.GetData()->GetCode() : "";
    if( code != "" && code != " - " )
        output << xml::attribute( "symbol", code );
    if( ! strPackage_.GetData().empty() )
        output << xml::attribute( "package", strPackage_);
    if( type_ != eMissionType_Automat )
        output << xml::attribute( "dia-behavior", diaBehavior_ );
    else
        output << xml::attribute( "mrt-dia-behavior", mrtDiaBehavior_ )
               << xml::attribute( "cdt-dia-behavior", cdtDiaBehavior_ );
    for( unsigned int i = 0; i < parameters_.size(); ++i )
        parameters_[i]->WriteArchive( output );
    output << xml::end;
}
