// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupHierarchy_h_
#define __KnowledgeGroupHierarchy_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class KnowledgeGroup_ABC;
    class CommunicationHierarchies;
    class Controller;
}

// =============================================================================
/** @class  KnowledgeGroupHierarchy
    @brief  KnowledgeGroupHierarchy
    // $$$$ AGE 2006-10-04: non au modèle qui observe le modèle !
*/
// Created: SBO 2006-09-27
// =============================================================================
class KnowledgeGroupHierarchy : public kernel::Extension_ABC
                              , public kernel::Serializable_ABC
                              , public kernel::Observer_ABC
                              , public kernel::ElementObserver_ABC< kernel::CommunicationHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit KnowledgeGroupHierarchy( kernel::Controller& controller );
    virtual ~KnowledgeGroupHierarchy();
    //@}

    //! @name Operations
    //@{
    const kernel::KnowledgeGroup_ABC* GetKnowledgeGroup() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroupHierarchy( const KnowledgeGroupHierarchy& );            //!< Copy constructor
    KnowledgeGroupHierarchy& operator=( const KnowledgeGroupHierarchy& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoSerialize( xml::xostream& xos ) const;
    virtual void NotifyUpdated( const kernel::CommunicationHierarchies& hierarchies );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::KnowledgeGroup_ABC* group_;
    //@}
};

#endif // __KnowledgeGroupHierarchy_h_
