// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Publisher_h_
#define __Publisher_h_

#include "Publisher_ABC.h"

namespace frontend
{
    class Master;

// =============================================================================
/** @class  Publisher
    @brief  Publisher
*/
// Created: SBO 2007-01-25
// =============================================================================
class Publisher : public Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Publisher( Master& master );
    virtual ~Publisher();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const ASN1T_MsgsInMaster& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Publisher( const Publisher& );            //!< Copy constructor
    Publisher& operator=( const Publisher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    Master& master_;
    //@}
};

}

#endif // __Publisher_h_
