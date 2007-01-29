// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PhysicalModel_h_
#define __PhysicalModel_h_

namespace kernel
{
    class Controller;
}

namespace frontend
{

// =============================================================================
/** @class  PhysicalModel
    @brief  PhysicalModel
*/
// Created: SBO 2007-01-29
// =============================================================================
class PhysicalModel
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PhysicalModel( kernel::Controller& controller );
    virtual ~PhysicalModel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PhysicalModel( const PhysicalModel& );            //!< Copy constructor
    PhysicalModel& operator=( const PhysicalModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

}

#endif // __PhysicalModel_h_
