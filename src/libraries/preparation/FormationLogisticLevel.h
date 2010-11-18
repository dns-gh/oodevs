// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FormationLogisticLevel_h_
#define __FormationLogisticLevel_h_

#include "tools/Resolver.h"

namespace kernel
{
    class LogisticLevel;
}

// =============================================================================
/** @class  FormationLogisticLevel
    @brief  FormationLogisticLevel
*/
// Created: HBD 2010-11-15
// =============================================================================
class FormationLogisticLevel : public tools::Resolver< kernel::LogisticLevel, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             FormationLogisticLevel();
    virtual ~FormationLogisticLevel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormationLogisticLevel( const FormationLogisticLevel& );            //!< Copy constructor
    FormationLogisticLevel& operator=( const FormationLogisticLevel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __FormationLogisticLevel_h_
