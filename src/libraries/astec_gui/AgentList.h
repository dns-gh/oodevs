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

class Controllers;
class Publisher_ABC;
class ItemFactory_ABC;

// =============================================================================
/** @class  AgentList
    @brief  Agent list
*/
// Created: AGE 2006-08-10
// =============================================================================
class AgentList : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentList( Controllers& controllers, Publisher_ABC& publisher, ItemFactory_ABC& factory );
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
