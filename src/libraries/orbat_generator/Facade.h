// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#ifndef orbat_generator_Facade_h
#define orbat_generator_Facade_h

#include "Party.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

namespace orbat_generator
{

// =============================================================================
/** @class  Facade
*/
// Created: RCD 2011-03-02
// =============================================================================

class Facade : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Facade( xml::xistream& xis, bfs::path base );
    virtual ~Facade();
    //@}

public:
    //! @name Operations
    //@{
    void Facade::CreateOrbat( xml::xostream& xos );
    std::string Facade::ComputeSignature( std::istream& is );
    //@}

private:
    //! @name Helpers
    //@{
    void Facade::RetrieveData( xml::xistream& xis, unsigned int& partyQty, unsigned int& formationQty, unsigned int& automateQty, unsigned int& unitQty, unsigned int& crowdQty, std::vector< std::string >& formationsRecord );
    void Facade::FillCrowds( xml::xistream& xis, unsigned int& crowdQty );
    void Facade::ListExcluded( xml::xistream& xis );
    void Facade::FillParty( xml::xistream& xis, xml::xistream& physXis );
    void Facade::FillFormation( xml::xistream& xis, xml::xistream& physXis );
    void Facade::FillAutomate( xml::xistream& xis, unsigned int& automateQty, unsigned int& pionQty, const std::string& odbType );
    void Facade::FillPion( xml::xistream& xis, unsigned int& pionsParAutomate );
    void Facade::ListFormations( xml::xistream& xis, std::vector< std::string >& formationsRecord, std::string& formationPattern );
    void Facade::AddCrowds( xml::xostream& orbat );
    //@}

private:
    //! @name Member data
    //@{
    std::string defaultPoint_;
    std::vector< std::string > excludes_;
    std::vector< std::string > crowds_;
    IdNameGenerator idNameGen_;
    std::vector< boost::shared_ptr< Formation > > formations_;
    std::vector< boost::shared_ptr< Party > > parties_;
    std::vector< boost::shared_ptr< Automate > > automates_;
    std::vector< boost::shared_ptr< Pion > > pions_;
    //@}
};

}

#endif // orbat_generator_Facade_h
