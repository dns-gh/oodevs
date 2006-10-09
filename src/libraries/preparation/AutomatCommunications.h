// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatCommunications_h_
#define __AutomatCommunications_h_

#include "EntityCommunications.h"
#include "clients_kernel/Serializable_ABC.h"

namespace xml
{
    class xostream;
}

// =============================================================================
/** @class  AutomatCommunications
    @brief  AutomatCommunications
*/
// Created: SBO 2006-09-25
// =============================================================================
class AutomatCommunications : public EntityCommunications
                            , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatCommunications( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~AutomatCommunications();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AutomatCommunications( const AutomatCommunications& );            //!< Copy constructor
    AutomatCommunications& operator=( const AutomatCommunications& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}
};

#endif // __AutomatCommunications_h_
