// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPositions_h_
#define __ObjectPositions_h_

#include "LocationPositions.h"
#include "clients_kernel/Updatable_ABC.h"

// =============================================================================
/** @class  ObjectPositions
    @brief  ObjectPositions
*/
// Created: AGE 2006-03-22
// =============================================================================
class ObjectPositions : public LocationPositions
                      , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
                      , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectPositions( const kernel::CoordinateConverter_ABC& converter );
    virtual ~ObjectPositions();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPositions( const ObjectPositions& );            //!< Copy constructor
    ObjectPositions& operator=( const ObjectPositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& message );
    //@}
};

#endif // __ObjectPositions_h_
