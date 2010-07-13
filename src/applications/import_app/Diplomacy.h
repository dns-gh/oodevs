// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Diplomacy_h_
#define __Diplomacy_h_

#include "Mapping.h"
#include <xeumeuleu/xml.hpp>

// =============================================================================
/** @class  Diplomacy
    @brief  Diplomacy
*/
// Created: LDC 2010-07-07
// =============================================================================
class Diplomacy
{

public:
    //! @name Constructors/Destructor
    //@{
             Diplomacy( const std::string& target, const std::string& type );
    virtual ~Diplomacy();
    //@}

    //! @name Operations
    //@{
    void Write( xml::xostream& xos, const Mapping& mapping ) const;
    //@}

    //! @name Copy/Assignment
    //@{
    Diplomacy( const Diplomacy& );            //!< Copy constructor
    Diplomacy& operator=( const Diplomacy& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string target_;
    std::string type_;
    //@}
};

#endif // __Diplomacy_h_
