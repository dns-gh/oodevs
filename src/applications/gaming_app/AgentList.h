// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentList_h_
#define __AgentList_h_

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  AgentList
    @brief  AgentList
*/
// Created: AGE 2006-08-29
// =============================================================================
class AgentList : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentList( kernel::Controllers& controllers, Publisher_ABC& publisher, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~AgentList();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentList( const AgentList& );            //!< Copy constructor
    AgentList& operator=( const AgentList& ); //!< Assignement operator
    //@}
};

#endif // __AgentList_h_
