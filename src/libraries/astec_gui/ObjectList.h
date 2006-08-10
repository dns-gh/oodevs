// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectList_h_
#define __ObjectList_h_

class Controllers;
class ItemFactory_ABC;

// =============================================================================
/** @class  ObjectList
    @brief  Object list
*/
// Created: AGE 2006-08-10
// =============================================================================
class ObjectList : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectList( Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~ObjectList();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectList( const ObjectList& );            //!< Copy constructor
    ObjectList& operator=( const ObjectList& ); //!< Assignement operator
    //@}
};

#endif // __ObjectList_h_
