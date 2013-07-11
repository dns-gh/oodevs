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

#include "tools/SelectionObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Agent_ABC;
    class KnowledgeGroup_ABC;
    class Controllers;
    class Automat_ABC;
    class Population_ABC;
    class Entity_ABC;
}

// =============================================================================
/** @class  KnowledgeGroupSelectionObserver
    @brief  KnowledgeGroupSelectionObserver
*/
// Created: AGE 2006-02-24
// =============================================================================
class KnowledgeGroupSelectionObserver : public tools::SelectionObserver_ABC
                                      , public tools::SelectionObserver_Base< kernel::KnowledgeGroup_ABC >
                                      , public tools::SelectionObserver_Base< kernel::Agent_ABC >
                                      , public tools::SelectionObserver_Base< kernel::Automat_ABC >
                                      , public tools::SelectionObserver_Base< kernel::Population_ABC >
                                      , private boost::noncopyable
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
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void Select( const kernel::Population_ABC& element );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Select( const kernel::KnowledgeGroup_ABC* ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* selected_;
    //@}
};

#endif // __KnowledgeGroupSelectionObserver_h_
