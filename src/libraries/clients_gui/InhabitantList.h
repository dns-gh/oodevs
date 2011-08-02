// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantList_h_
#define __InhabitantList_h_

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  InhabitantList
    @brief  Population list
*/
// Created: SLG 2010-11-30
// =============================================================================
class InhabitantList : public Q3VBox
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantList( kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~InhabitantList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantList( const InhabitantList& );            //!< Copy constructor
    InhabitantList& operator=( const InhabitantList& ); //!< Assignment operator
    //@}
};

}

#endif // __InhabitantList_h_
