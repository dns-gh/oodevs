// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __CapacityContainer_ABC_h_
#define __CapacityContainer_ABC_h_

class ObjectCapacity_ABC;

#include <boost/ptr_container/ptr_vector.hpp>

// =============================================================================
/** @class  CapacityContainer_ABC
*/
// Created: JCR 2008-05-26
// =============================================================================
class CapacityContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            CapacityContainer_ABC() {}
    virtual ~CapacityContainer_ABC() {}
    //@}

    //! @name Method
    //@{
    template <typename Capacity> void AddCapacity( Capacity* capacity );
    //@}

protected:
    //! @name 
    //@{
    typedef boost::ptr_vector< ObjectCapacity_ABC >   T_Capacities;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CapacityContainer_ABC( const CapacityContainer_ABC& );            //!< Copy constructor
    CapacityContainer_ABC& operator=( const CapacityContainer_ABC& ); //!< Assignment operator
    //@}
};

#endif // __CapacityContainer_ABC_h_
