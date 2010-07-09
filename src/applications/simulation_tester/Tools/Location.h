// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-10 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Location_h_
#define __Location_h_

#include "Types.h"
#include "Position.h"

namespace TEST {

    class Location;

//! @name Types
//@{
typedef std::vector< Location* >                 T_LocationVector;
typedef std::vector< const Location* >           CT_LocationVector;
typedef T_LocationVector::iterator               IT_LocationVector;
typedef T_LocationVector::const_iterator         CIT_LocationVector;
typedef T_LocationVector::const_reverse_iterator CRIT_LocationVector;
//@}


// =============================================================================
/** @class  Location
    @brief  Location
    @par    Using example
    @code
    Location;
    @endcode
*/
// Created: SBO 2005-08-10
// =============================================================================
class Location
{
public:
    //! @name Constructors/Destructor
    //@{
             Location();
    virtual ~Location();
    //@}

    //! @name Accessors
    //@{
    EnumLocationType GetType          () const;
    const T_PositionVector&    GetPositionVector() const;
    //@}

    //! @name Test Parameters
    //@{
    static Location&         GetTestParam_Location    ( const Position& pos,
                                                        EnumLocationType eType = ( EnumLocationType )-1 );
    static T_LocationVector& GetTestParam_LocationList( const Position& pos, unsigned int nLocation,
                                                        EnumLocationType eType = ( EnumLocationType )-1 );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Location( const Location& );            //!< Copy constructor
    Location& operator=( const Location& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T_PositionVector           points_;
    EnumLocationType eType_;
    //@}
};

} // end namespace TEST

#include "Location.inl"

#endif // __Location_h_
