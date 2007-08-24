// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lima_h_
#define __Lima_h_

#include "TacticalLine_ABC.h"
#include "Entity_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  Lima
    @brief  Lima
*/
// Created: SBO 2006-11-15
// =============================================================================
class Lima : public TacticalLine_ABC, public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Lima( const Model& model, const ASN1T_MsgLimaCreation& message );
    virtual ~Lima();
    //@}

    //! @name Operations
    //@{
    void Update      ( const ASN1T_MsgLimaCreation& message );
    void Update      ( const ASN1T_MsgLimaUpdate& message );

    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Lima( const Lima& );            //!< Copy constructor
    Lima& operator=( const Lima& ); //!< Assignement operator
    //@}
};

}

#endif // __Lima_h_

