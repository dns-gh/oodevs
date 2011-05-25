// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoReportsTab_h_
#define __InfoReportsTab_h_

#include "tools/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoReportsTab
    @brief  InfoReportsTab
*/
// Created: SBO 2007-02-06
// =============================================================================
class InfoReportsTab : public QVBox
                     , public tools::Observer_ABC
                     , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoReportsTab( QTabWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoReportsTab();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoReportsTab( const InfoReportsTab& );            //!< Copy constructor
    InfoReportsTab& operator=( const InfoReportsTab& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QTabWidget* parent_;
    std::auto_ptr< gui::ItemFactory_ABC > factory_;
    //@}
};

#endif // __InfoReportsTab_h_
