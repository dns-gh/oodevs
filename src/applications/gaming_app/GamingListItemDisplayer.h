// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GamingListItemDisplayer_h_
#define __GamingListItemDisplayer_h_

#include "clients_gui/ListItemDisplayer.h"

class AgentKnowledge_ABC;
class PopulationKnowledge_ABC;
class ObjectKnowledge_ABC;

// =============================================================================
/** @class  GamingListItemDisplayer
    @brief  GamingListItemDisplayer
*/
// Created: SBO 2007-03-28
// =============================================================================
class GamingListItemDisplayer : public gui::ListItemDisplayer
                              , public tools::Caller< AgentKnowledge_ABC >
                              , public tools::Caller< PopulationKnowledge_ABC >
                              , public tools::Caller< ObjectKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             GamingListItemDisplayer();
    virtual ~GamingListItemDisplayer();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GamingListItemDisplayer( const GamingListItemDisplayer& );            //!< Copy constructor
    GamingListItemDisplayer& operator=( const GamingListItemDisplayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Call( const AgentKnowledge_ABC& value );
    virtual void Call( const PopulationKnowledge_ABC& value );
    virtual void Call( const ObjectKnowledge_ABC& value );
    //@}
};

#endif // __GamingListItemDisplayer_h_
