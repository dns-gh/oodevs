// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Zone_ABC_h_
#define __Zone_ABC_h_

class Position;

// =============================================================================
/** @class  Zone_ABC
    @brief  Zone_ABC
*/
// Created: AGE 2007-10-09
// =============================================================================
class Zone_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Zone_ABC() {}
    virtual ~Zone_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Contains( const Position& pos ) const = 0;
    //@}
};

#endif // __Zone_ABC_h_
