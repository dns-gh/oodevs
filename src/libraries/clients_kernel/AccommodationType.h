// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AccommodationType_h_
#define __AccommodationType_h_

namespace kernel
{
// =============================================================================
/** @class  AccommodationType
    @brief  AccommodationType
*/
// Created: LDC 2011-03-25
// =============================================================================
class AccommodationType
{
public:
    //! @name Constructors/Destructor
    //@{
             AccommodationType( const std::string& role, double maxCapacity, double nominalCapacity );
    virtual ~AccommodationType();
    //@}

    //! @name Operations
    //@{
    const std::string& GetRole() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AccommodationType( const AccommodationType& );            //!< Copy constructor
    AccommodationType& operator=( const AccommodationType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string role_;
    //@}
};
}

#endif // __AccommodationType_h_
