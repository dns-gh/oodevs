// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#ifndef orbat_generator_Formation_h
#define orbat_generator_Formation_h

#include "Automate.h"
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
/** @class  Formation
*/
// Created: RCD 2011-03-02
// =============================================================================

class Formation : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Formation( std::string type );
    virtual ~Formation();
    //@}

public:
    //! @name Operations
    //@{
    void Formation::InsertIntoOrbat( xml::xostream& orbat, const IdNameGenerator& idNameGen );
    void Formation::AddAutomate( boost::shared_ptr< Automate > automate );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< Automate > > automates_;
    std::string type_;
    unsigned int current_;
    //@}
};

}

#endif // orbat_generator_Formation_h
