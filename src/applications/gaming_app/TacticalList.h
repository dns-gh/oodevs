// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalList_h_
#define __TacticalList_h_

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;
}

// =============================================================================
/** @class  TacticalList
    @brief  TacticalList
*/
// Created: AGE 2006-10-20
// =============================================================================
class TacticalList : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalList( kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons );
    virtual ~TacticalList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalList( const TacticalList& );            //!< Copy constructor
    TacticalList& operator=( const TacticalList& ); //!< Assignement operator
    //@}
};

#endif // __TacticalList_h_
