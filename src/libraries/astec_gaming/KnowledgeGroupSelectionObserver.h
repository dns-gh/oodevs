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

#include "SelectionObserver_ABC.h"

class Agent_ABC;
class KnowledgeGroup;
class Controllers;

// =============================================================================
/** @class  KnowledgeGroupSelectionObserver
    @brief  KnowledgeGroupSelectionObserver
*/
// Created: AGE 2006-02-24
// =============================================================================
class KnowledgeGroupSelectionObserver
    : public SelectionObserver_ABC
    , public SelectionObserver_Base< KnowledgeGroup >
    , public SelectionObserver_Base< Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupSelectionObserver();
    virtual ~KnowledgeGroupSelectionObserver();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroupSelectionObserver( const KnowledgeGroupSelectionObserver& );            //!< Copy constructor
    KnowledgeGroupSelectionObserver& operator=( const KnowledgeGroupSelectionObserver& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const KnowledgeGroup& element );
    virtual void Select( const Agent_ABC& element );
    virtual void Select( const KnowledgeGroup* ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    const KnowledgeGroup* selected_;
    //@}
};

#endif // __KnowledgeGroupSelectionObserver_h_
