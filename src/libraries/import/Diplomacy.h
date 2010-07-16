// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef import_Diplomacy_h
#define import_Diplomacy_h

#include <xeumeuleu/xml.hpp>

class Mapping;

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
    explicit Diplomacy( xml::xisubstream xis, const Mapping& mapping );
    virtual ~Diplomacy();
    //@}

    //! @name Operators
    //@{
    friend xml::xostream& operator<<( xml::xostream& xos, const Diplomacy& diplomacy );
    //@}

private:
    //! @name Member data
    //@{
    std::string target_;
    std::string type_;
    const Mapping* mapping_;
    //@}
};

#endif // import_Diplomacy_h
