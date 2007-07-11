// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-26 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __GenObject_h_
#define __GenObject_h_

#include "Types.h"

namespace TEST {

    class Location;
    class GenObject;
    class Testable_Entity;

//! @name Types
//@{
typedef std::vector< GenObject* >                 T_GenObjectVector;
typedef std::vector< const GenObject* >           CT_GenObjectVector;
typedef T_GenObjectVector::iterator               IT_GenObjectVector;
typedef T_GenObjectVector::const_iterator         CIT_GenObjectVector;
typedef T_GenObjectVector::const_reverse_iterator CRIT_GenObjectVector;
//@}

// =============================================================================
/** @class  GenObject
    @brief  GenObject
    @par    Using example
    @code
    GenObject;
    @endcode
*/
// Created: SBO 2005-08-26
// =============================================================================
class GenObject
{

public:
    //! @name Constructors/Destructor
    //@{
             GenObject();
    virtual ~GenObject();
    //@}

    //! @name Accessors
    //@{
    const Location&                      GetLocation       () const;
    ASN1T_EnumObjectType                 GetType           () const;
    ASN1T_EnumMissionGenSousTypeObstacle GetObstacleSubType() const;
    uint                                 GetDensity        () const;
    //@}

    //! @name Test Parameters
    //@{
    static GenObject&         GetTestParam_GenObject    ( const Testable_Entity& target );
    static T_GenObjectVector& GetTestParam_GenObjectList( const Testable_Entity& target, uint nGenObject );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GenObject( const GenObject& );            //!< Copy constructor
    GenObject& operator=( const GenObject& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Location*                            pLocation_;
    ASN1T_EnumObjectType                 eType_;
    ASN1T_EnumMissionGenSousTypeObstacle eObstacleSubType_;
    uint                                 nDensity_;
    //@}
};

} // end namespace TEST

#include "GenObject.inl"

#endif // __GenObject_h_
