// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamCommunications_h_
#define __TeamCommunications_h_

#include "EntityCommunications.h"
#include "clients_kernel/Serializable_ABC.h"

// =============================================================================
/** @class  TeamCommunications
    @brief  TeamCommunications
*/
// Created: SBO 2006-09-25
// =============================================================================
class TeamCommunications : public EntityCommunications
                         , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamCommunications( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~TeamCommunications();
    //@}

private:
    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}
};

#endif // __TeamCommunications_h_
