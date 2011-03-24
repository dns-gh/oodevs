// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#ifndef orbat_generator_Pion_h
#define orbat_generator_Pion_h

#include "IdNameGenerator.h"
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
/** @class  Pion
*/
// Created: RCD 2011-03-02
// =============================================================================

class Pion : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Pion( bool posteDeCommandement, std::string type, std::string posi );
    virtual ~Pion();
    //@}

public:
    //! @name Operations
    //@{
    void Pion::InsertIntoOrbat( xml::xostream& orbat, IdNameGenerator& idNameGen );
    //@}

private:
    //! @name Helpers
    //@{    
    //@}

private:
    //! @name Member data
    //@{    
    std::string type_;
    std::string posi_;
    const bool pc_;
    //@}
};

}

#endif // orbat_generator_Pion_h
