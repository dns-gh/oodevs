// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef import_Parser_h
#define import_Parser_h

#include "Diplomacy.h"
#include "Warnings.h"
#include "Mapping.h"
#include "Mission.h"
#include "Side.h"
#include <xeumeuleu/xml.hpp>
#include <fstream>
#include <string>
#include <vector>

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
             Parser( const std::string& inputFile, const std::string& rootDir, const std::string& exerciseFile, const std::string& outputDir );
    virtual ~Parser();
    //@}

    //! @name Operations
    //@{
    void Generate();
    //@}

private:
    //! @name Helpers
    //@{
    void WriteWeather( xml::xisubstream xis );
    void WriteOrbat();
    void ReadRelation( xml::xistream& xis );
    void ReadSide( xml::xistream& xis );
    void ReadNextPlan( xml::xistream& xis );
    void PlaceEntity( xml::xistream& xis );
    void ReadPlanDatum( xml::xistream& xis );
    void ReadTactical( xml::xistream& xis );
    void ReadTacticalPointList( xml::xistream& xis, std::vector< std::vector< Position > >& positions );
    void ReadTacticalPoint( xml::xistream& xis, std::vector< Position >& positions );
    void ReadMission( xml::xistream& xis );

    void WriteUnitInOrd( xml::xistream& xis, xml::xosubstream xos, const std::string& timeName, const std::string& date );
    void WriteMissionInOrd( xml::xistream& xis, xml::xostream& xos, const std::string& timeName, const std::string& date );
    //@}

    //! @name Copy/Assignment
    //@{
    Parser( const Parser& );            //!< Copy constructor
    Parser& operator=( const Parser& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string outputDir_;
    xml::xifstream xis_;
    Warnings warnings_;
    Mapping mapping_;
    unsigned int plan_;
    std::map< std::string, std::vector< Diplomacy > > diplomacies_;
    std::map< std::string, Side > sides_;
    std::map< std::string, std::vector< std::vector< Position > > > tacticals_;
    std::map< std::string, Mission > missions_;
    //@}
};

#endif // import_Parser_h
