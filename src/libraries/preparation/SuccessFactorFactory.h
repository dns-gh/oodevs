// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorFactory_h_
#define __SuccessFactorFactory_h_

#include "SuccessFactorFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

class Model;
class SuccessFactorActionTypes;

// =============================================================================
/** @class  SuccessFactorFactory
    @brief  SuccessFactorFactory
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorFactory : public SuccessFactorFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorFactory( kernel::Controllers& controllers, const Model& model, const SuccessFactorActionTypes& actionTypes );
    virtual ~SuccessFactorFactory();
    //@}

    //! @name Operations
    //@{
    virtual SuccessFactor* Create( xml::xistream& xis ) const;
    virtual SuccessFactor* Create( const QString& name ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorFactory( const SuccessFactorFactory& );            //!< Copy constructor
    SuccessFactorFactory& operator=( const SuccessFactorFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Model& model_;
    const SuccessFactorActionTypes& actionTypes_;
    //@}
};

#endif // __SuccessFactorFactory_h_
