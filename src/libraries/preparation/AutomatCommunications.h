// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatCommunications_h_
#define __AutomatCommunications_h_

#include "EntityCommunications.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class KnowledgeGroup_ABC;
}

namespace xml
{
    class xistream;
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
             AutomatCommunications( xml::xistream& xis, kernel::Controller& controller, kernel::Entity_ABC& holder, const kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groups );
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
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    static kernel::KnowledgeGroup_ABC* ReadKnowledgeGroup( xml::xistream& xis, const kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groups );
    //@}
};

#endif // __AutomatCommunications_h_
