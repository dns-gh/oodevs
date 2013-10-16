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

#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Team_ABC;

// =============================================================================
/** @class  TeamSelectionObserver
    @brief  TeamSelectionObserver
*/
// Created: AGE 2006-02-24
// =============================================================================
class TeamSelectionObserver : public tools::SelectionObserver_Base< Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             TeamSelectionObserver();
    virtual ~TeamSelectionObserver();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TeamSelectionObserver( const TeamSelectionObserver& );            //!< Copy constructor
    TeamSelectionObserver& operator=( const TeamSelectionObserver& ); //!< Assignment operator
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const Team_ABC& team );
    virtual void Select( const Team_ABC* team ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    const Team_ABC* selected_;
    //@}
};

}

#endif // __TeamSelectionObserver_h_
