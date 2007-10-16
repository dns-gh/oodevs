// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamSelectionObserver_h_
#define __TeamSelectionObserver_h_

#include "SelectionObserver_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Team_ABC;

// =============================================================================
/** @class  TeamSelectionObserver
    @brief  TeamSelectionObserver
*/
// Created: AGE 2006-02-24
// =============================================================================
class TeamSelectionObserver : public SelectionObserver_ABC
                            , public SelectionObserver_Base< Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamSelectionObserver();
    virtual ~TeamSelectionObserver();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamSelectionObserver( const TeamSelectionObserver& );            //!< Copy constructor
    TeamSelectionObserver& operator=( const TeamSelectionObserver& ); //!< Assignement operator
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const Entity_ABC& element );
    virtual void Select( const Team_ABC* ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    const Team_ABC* selected_;
    //@}
};

}

#endif // __TeamSelectionObserver_h_
