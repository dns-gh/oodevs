// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoConflictsTab_h_
#define __InfoConflictsTab_h_

#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class KnowledgeConverter_ABC;
}

namespace gui
{
    class DisplayExtractor;
}

// =============================================================================
/** @class  InfoConflictsTab
    @brief  InfoConflictsTab
*/
// Created: SBO 2007-02-15
// =============================================================================
class InfoConflictsTab : public Q3VBox
                       , public tools::Observer_ABC
                       , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoConflictsTab( QTabWidget* parent, kernel::Controllers& controllers,
                 gui::DisplayExtractor& extractor, const kernel::KnowledgeConverter_ABC& converter );
    virtual ~InfoConflictsTab();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QTabWidget* parent_;
    //@}
};

#endif // __InfoConflictsTab_h_
