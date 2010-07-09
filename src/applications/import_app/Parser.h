// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Parser_h_
#define __Parser_h_

#include <string>
#include <vector>
#include <xeumeuleu/xml.h>

#include "Diplomacy.h"
#include "Mapping.h"
#include "Mission.h"
#include "Side.h"

// =============================================================================
/** @class  Parser
    @brief  Parser
*/
// Created: LDC 2010-07-06
// =============================================================================
class Parser
{

public:
    //! @name Constructors/Destructor
    //@{
             Parser( const std::string& inputFile, const std::string& rootDir, const std::string& outputFile );
    virtual ~Parser();
    //@}

    //! @name Operations
    //@{
    void Generate();
    //@}

private:
    //! @name Helpers
    //@{
    void WriteWeather();
    void WriteOrbat();
    void WriteDiplomacies();
    void WriteSides();
    void ReadRelations();
    void ReadRelation( xml::xistream& xis );
    void ReadSides();
    void ReadSide( xml::xistream& xis );
    void ReadPlan();
    void ReadNextPlan( xml::xistream& xis );
    void PlaceEntity( xml::xistream& xis );
    void ReadPlanData( xml::xistream& xis );
    void ReadPlanDatum( xml::xistream& xis );
    void ReadTacticals( xml::xistream& xis );
    void ReadTactical( xml::xistream& xis );
    void ReadTacticalPointList( xml::xistream& xis, std::vector< std::vector< Position > >& positions );
    void ReadTacticalPoint( xml::xistream& xis, std::vector< Position >& positions );
    void ReadMissions( xml::xistream& xis );
    void ReadMission( xml::xistream& xis );

    void WriteUnitInOrd( xml::xistream& xis, xml::xostream& xos, const std::string& timeName, const std::string& date );
    void WriteMissionInOrd( xml::xistream& xis, xml::xostream& xos, const std::string& timeName, const std::string& date );
    //@}

    //! @name Copy/Assignment
    //@{
    Parser( const Parser& );            //!< Copy constructor
    Parser& operator=( const Parser& ); //!< Assignment operator
    //@}

private:
    std::string outDir_;
    xml::xifstream xis_;
    xml::xofstream xos_;
    xml::xofstream xosWeather_;
    std::string dateScenario_;
    unsigned int plan_;

    Mapping mapping_;

    std::map< std::string, std::vector< Diplomacy > > diplomacies_;
    std::map< std::string, Side > sides_;
    std::map< std::string, std::vector< std::vector< Position > > > tacticals_;
    std::map< std::string, Mission > missions_;
};

#endif // __Parser_h_
