// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Visitor_ABC_h_
#define __Visitor_ABC_h_

namespace tools {

// =============================================================================
/** @class  Visitor_ABC
    @brief  Visitor_ABC
*/
// Created: LDC 2011-01-04
// =============================================================================
template< typename T > class Visitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Visitor_ABC() {}
    virtual ~Visitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const T& ) = 0;
    //@}
};

}

#endif // __Visitor_ABC_h_
