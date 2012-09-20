// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#ifndef orbat_generator_Automate_h
#define orbat_generator_Automate_h

#include "Pion.h"
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

namespace xml
{
    class xistream;
    class xostream;
}

namespace orbat_generator
{

// =============================================================================
/** @class  Automate
*/
// Created: RCD 2011-03-02
// =============================================================================

class Automate : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Automate( const std::string type, std::vector< boost::shared_ptr< Pion > > pions, const std::string& odbType );
    virtual ~Automate();
    //@}

public:
    //! @name Operations
    //@{
    void InsertIntoOrbat( xml::xostream& orbat, const IdNameGenerator& idNameGen, unsigned int knowledgeGroupId );
    //@}

private:
    //! @name Member data
    //@{
    bool engaged_;
    const std::string type_;
    std::vector< boost::shared_ptr< Pion > > pions_;
    //@}
};

}

#endif // orbat_generator_Automate_h
