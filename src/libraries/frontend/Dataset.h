// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dataset_h_
#define __Dataset_h_

#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controller;
}

namespace frontend
{
    class PhysicalModel;

// =============================================================================
/** @class  Dataset
    @brief  Dataset
*/
// Created: SBO 2007-01-29
// =============================================================================
class Dataset : public kernel::StringResolver< PhysicalModel >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Dataset( kernel::Controller& controller );
    virtual ~Dataset();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Dataset( const Dataset& );            //!< Copy constructor
    Dataset& operator=( const Dataset& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    //@}
};

}

#endif // __Dataset_h_
