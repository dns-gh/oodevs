// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _AgentAffinitiesItem_h
#define _AgentAffinitiesItem_h

#include "AffinitiesDialog.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

// =============================================================================
/** @class  AgentAffinitiesDialog
    @brief  Agent Affinities dialog
*/
// Created: LGY 2011-03-14
// =============================================================================
class AgentAffinitiesDialog : public AffinitiesDialog
                            , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentAffinitiesDialog( QWidget* parent, kernel::Controllers& controllers );
    virtual ~AgentAffinitiesDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // _AgentAffinitiesItem_h
