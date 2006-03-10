// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireResultListView_h_
#define __FireResultListView_h_

#include "ListDisplayer.h"
class SubItemDisplayer;

class PopulationFireResult;
class AgentFireResult;
class Equipment;
class Casualties;

// =============================================================================
/** @class  FireResultListView
    @brief  FireResultListView
*/
// Created: AGE 2006-03-10
// =============================================================================
class FireResultListView : public ListDisplayer< FireResultListView >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FireResultListView( QWidget* parent );
    virtual ~FireResultListView();
    //@}

    //! @name Operations
    //@{
    void Display( const PopulationFireResult* result, Displayer_ABC& displayer, ValuedListItem* item );
    void Display( const AgentFireResult* result, Displayer_ABC& displayer, ValuedListItem* item );
    void Display( const Equipment& equipment, Displayer_ABC& displayer, ValuedListItem* item );
    void Display( const Casualties& casualties, Displayer_ABC& displayer, ValuedListItem* item );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FireResultListView( const FireResultListView& );            //!< Copy constructor
    FireResultListView& operator=( const FireResultListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    SubItemDisplayer* agentDisplay_;
    //@}
};

#endif // __FireResultListView_h_
