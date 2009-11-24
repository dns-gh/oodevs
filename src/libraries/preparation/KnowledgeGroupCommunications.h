// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgeGroupCommunications_h_
#define __KnowledgeGroupCommunications_h_

#include "EntityCommunications.h"
#include "clients_kernel/Serializable_ABC.h"

namespace xml
{
    class xostream;
}

// =============================================================================
/** @class  KnowledgeGroupCommunications
    @brief  KnowledgeGroupCommunications
*/
// Created: FHD 2009-11-20
// =============================================================================
class KnowledgeGroupCommunications : public EntityCommunications
                                   , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupCommunications( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~KnowledgeGroupCommunications();
    //@}

    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroupCommunications( const KnowledgeGroupCommunications& );            //!< Copy constructor
    KnowledgeGroupCommunications& operator=( const KnowledgeGroupCommunications& ); //!< Assignment operator
    //@}

};

#endif // __KnowledgeGroupCommunications_h_
