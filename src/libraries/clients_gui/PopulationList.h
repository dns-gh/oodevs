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

class Controllers;
class ItemFactory_ABC;

// =============================================================================
/** @class  PopulationList
    @brief  Population list
*/
// Created: AGE 2006-08-10
// =============================================================================
class PopulationList : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationList( Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~PopulationList();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationList( const PopulationList& );            //!< Copy constructor
    PopulationList& operator=( const PopulationList& ); //!< Assignement operator
    //@}
};

#endif // __PopulationList_h_
