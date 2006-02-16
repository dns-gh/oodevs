// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationListView_h_
#define __PopulationListView_h_

#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"

class Controller;
class Population;

// =============================================================================
/** @class  PopulationListView
    @brief  PopulationListView
*/
// Created: HME 2005-10-03
// =============================================================================
class PopulationListView : public QListView
                         , public Observer_ABC
                         , public ElementObserver_ABC< Population >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationListView( QWidget* pParent, Controller& controller );
    virtual ~PopulationListView();
    //@}

private:
    //! @name Copy / Assignment
    //@{
    PopulationListView( const PopulationListView& );
    PopulationListView& operator=( const PopulationListView& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Population& popu );
    virtual void NotifyUpdated( const Population& popu );
    virtual void NotifyDeleted( const Population& popu );
    //@}

private:
    //! @name Member data
    //@{
    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __PopulationListView_h_
