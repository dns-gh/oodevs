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
             Facade();
             Facade( xml::xistream& xis, const bfs::path& base );
    virtual ~Facade();
    //@}

public:
    //! @name Operations
    //@{
    void CreateOrbat( xml::xostream& xos );
    std::string ComputeSignature( std::istream& is ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void RetrieveData( xml::xisubstream xis, unsigned int& automateQuantity, unsigned int& unitQuantity, unsigned int& crowdQuantity );
    void ReadCrowds( xml::xisubstream xis, unsigned int crowdQuantity );
    void FillCrowds( xml::xistream& xis );
    void ListExcluded( xml::xistream& xis );
    void FillParty( xml::xistream& xis, xml::xistream& physXis );
    void FillFormation( xml::xistream& xis, xml::xistream& physXis );
    void ReadAutomates( xml::xisubstream xis, const std::string& type, unsigned int automateQuantity, unsigned int unitQuantity );
    void FillAutomate( xml::xistream& xis, unsigned int& automateQuantity, unsigned int& pionQuantity, const std::string& odbType );
    void FillPion( xml::xistream& xis, unsigned int& pionsParAutomate );
    void ListFormations( xml::xistream& xis, std::vector< std::string >& formationsRecord );
    void AddCrowds( xml::xostream& orbat );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< std::string > excludes_;
    std::vector< std::string > crowds_;
    const IdNameGenerator idNameGen_;
    std::vector< boost::shared_ptr< Formation > > formations_;
    std::vector< boost::shared_ptr< Party > > parties_;
    std::vector< boost::shared_ptr< Automate > > automates_;
    std::vector< boost::shared_ptr< Pion > > pions_;
    //@}
};

}

#endif // orbat_generator_Facade_h
