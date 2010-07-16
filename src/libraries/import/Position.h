// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Position_h_
#define __Position_h_

#include <xeumeuleu/xml.hpp>

// =============================================================================
/** @class  Position
    @brief  Position
*/
// Created: LDC 2010-07-08
// =============================================================================
class Position
{
public:
    //! @name Constructors/Destructor
    //@{
             Position();
    explicit Position( xml::xistream& xis );
    virtual ~Position();
    //@}

    //! @name Operations
    //@{
    friend xml::xostream& operator<<( xml::xostream& xos, const Position& position );
    //@}

private:
    //! @name Member data
    //@{
    float latitude_;
    float longitude_;
    //@}
};

#endif // __Position_h_
