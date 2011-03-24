// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#ifndef orbat_generator_IdNameGenerator_h
#define orbat_generator_IdNameGenerator_h

#include <boost/noncopyable.hpp>
#include <xeumeuleu/xml.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

namespace orbat_generator
{
// =============================================================================
/** @class  IdNameGenerator
*/
// Created: RCD 2011-03-04
// =============================================================================

class IdNameGenerator : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             IdNameGenerator::IdNameGenerator();
    virtual ~IdNameGenerator();
    //@}

public:
    //! @name Operations
    //@{
    void IdNameGenerator::Init( std::string pattern, std::string partyPattern, std::string formationPattern, std::string automatePattern, std::string pionPattern, std::string crowdPattern );
    unsigned int IdNameGenerator::ComputePionId();
    std::string IdNameGenerator::ComputePionName();
    unsigned int IdNameGenerator::ComputePartyId();
    std::string IdNameGenerator::ComputePartyName();
    unsigned int IdNameGenerator::ComputeFormationId();
    std::string IdNameGenerator::ComputeFormationName();
    unsigned int IdNameGenerator::ComputeAutomateId();
    std::string IdNameGenerator::ComputeAutomateName();
    std::string IdNameGenerator::ComputeCrowdName();
    //@}

private:
    //! @name Helpers
    //@{
    void IdNameGenerator::Rewind( std::string& name );
    void IdNameGenerator::Increment( std::string& name );
    void IdNameGenerator::RegReplace( std::string& name, std::string& toReplace, std::string replaceBy );
    //@}

private:
    //! @name Member data
    //@{    
    std::string  crowdName_;
    std::string  pionName_;
    std::string  formationName_;
    std::string  partyName_;
    std::string  automateName_;
    std::string  pattern_;
    unsigned int partyId_;
    unsigned int formationId_;
    unsigned int automateId_;
    unsigned int pionId_;
    //@}
};

}

#endif // orbat_generator_Pion_h
