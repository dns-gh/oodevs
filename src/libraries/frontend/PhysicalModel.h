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

#include "Messages.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controller;
}

namespace frontend
{
    class Dataset;

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
             PhysicalModel( const ASN1T_MsgPhysicalModelCreation& message, kernel::Controller& controller
                          , const kernel::StringResolver< frontend::Dataset >& datasetResolver );
    virtual ~PhysicalModel();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    const Dataset& GetDataset() const;
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
    kernel::Controller& controller_;
    QString name_;
    frontend::Dataset& dataset_;
    //@}
};

}

#endif // __PhysicalModel_h_
