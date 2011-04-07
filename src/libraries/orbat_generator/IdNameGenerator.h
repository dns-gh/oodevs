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

namespace xml
{
    class xisubstream;
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
    explicit IdNameGenerator( xml::xisubstream xis );
    virtual ~IdNameGenerator();
    //@}

public:
    //! @name Operations
    //@{
    unsigned int ComputePionId() const;
    std::string ComputePionName() const;
    unsigned int ComputePartyId() const;
    std::string ComputePartyName() const;
    unsigned int ComputeFormationId() const;
    std::string ComputeFormationName() const;
    unsigned int ComputeAutomateId() const;
    std::string ComputeAutomateName() const;
    std::string ComputeCrowdName() const;
    void RewindAll() const;
    //@}

private:
    //! @name Helpers
    //@{
    void Rewind( std::string& name ) const;
    void Increment( std::string& name ) const;
    void RegReplace( std::string& name, std::string& toReplace, std::string replaceBy ) const;
    //@}

private:
    //! @name Member data
    //@{
    mutable std::string  crowdName_;
    mutable std::string  pionName_;
    mutable std::string  formationName_;
    mutable std::string  partyName_;
    mutable std::string  automateName_;
    mutable std::string  pattern_;
    mutable unsigned int partyId_;
    mutable unsigned int formationId_;
    mutable unsigned int automateId_;
    mutable unsigned int pionId_;
    //@}
};

}

#endif // orbat_generator_Pion_h
