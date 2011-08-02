// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationList_h_
#define __PopulationList_h_

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  PopulationList
    @brief  Population list
*/
// Created: AGE 2006-08-10
// =============================================================================
class PopulationList : public Q3VBox
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationList( kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~PopulationList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationList( const PopulationList& );            //!< Copy constructor
    PopulationList& operator=( const PopulationList& ); //!< Assignment operator
    //@}
};

}

#endif // __PopulationList_h_
