// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentListView_h_
#define __AgentListView_h_

#include "clients_gui/AgentListView.h"

// =============================================================================
/** @class  AgentListView
    @brief  AgentListView
*/
// Created: SBO 2006-08-29
// =============================================================================
class AgentListView : public gui::AgentListView
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Agent_ABC& agent, gui::ValuedListItem* item );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentListView( const AgentListView& );            //!< Copy constructor
    AgentListView& operator=( const AgentListView& ); //!< Assignement operator
    //@}
};

#endif // __AgentListView_h_
