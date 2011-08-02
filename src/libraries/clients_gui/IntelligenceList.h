// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligenceList_h_
#define __IntelligenceList_h_

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;

// =============================================================================
/** @class  IntelligenceList
    @brief  IntelligenceList
*/
// Created: SBO 2007-10-19
// =============================================================================
class IntelligenceList : public Q3VBox
{
public:
    //! @name Constructors/Destructor
    //@{
             IntelligenceList( kernel::Controllers& controllers, ItemFactory_ABC& factory, EntitySymbols& icons, const kernel::Profile_ABC& profile );
    virtual ~IntelligenceList();
    //@}
};

}

#endif // __IntelligenceList_h_
