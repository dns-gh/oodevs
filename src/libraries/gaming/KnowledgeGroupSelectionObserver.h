// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupSelectionObserver_h_
#define __KnowledgeGroupSelectionObserver_h_

#include "clients_kernel/SelectionObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class KnowledgeGroup_ABC;
    class Controllers;
}

// =============================================================================
/** @class  KnowledgeGroupSelectionObserver
    @brief  KnowledgeGroupSelectionObserver
*/
// Created: AGE 2006-02-24
// =============================================================================
class KnowledgeGroupSelectionObserver : public kernel::SelectionObserver_ABC
                                      , public kernel::SelectionObserver_Base< kernel::KnowledgeGroup_ABC >
                                      , public kernel::SelectionObserver_Base< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupSelectionObserver();
    virtual ~KnowledgeGroupSelectionObserver();
    //@}

    //! @name Operations
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::KnowledgeGroup_ABC& element );
    virtual void Select( const kernel::Agent_ABC& element );
    //@}

protected:
    //! @name Operations
    //@{    
    virtual void Select( const kernel::KnowledgeGroup_ABC* ) = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroupSelectionObserver( const KnowledgeGroupSelectionObserver& );            //!< Copy constructor
    KnowledgeGroupSelectionObserver& operator=( const KnowledgeGroupSelectionObserver& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::KnowledgeGroup_ABC* selected_;
    //@}
};

#endif // __KnowledgeGroupSelectionObserver_h_
