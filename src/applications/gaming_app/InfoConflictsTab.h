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

namespace kernel
{
    class Controllers;
    class KnowledgeConverter_ABC;
}

namespace gui
{
    class DisplayExtractor;
}

class FireResultListView;

// =============================================================================
/** @class  InfoConflictsTab
    @brief  InfoConflictsTab
*/
// Created: SBO 2007-02-15
// =============================================================================
class InfoConflictsTab : public Q3VBox
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoConflictsTab( QTabWidget* parent, kernel::Controllers& controllers,
                 gui::DisplayExtractor& extractor, const kernel::KnowledgeConverter_ABC& converter );
    virtual ~InfoConflictsTab();
    //@}

    void Purge();

private:
    //! @name Member data
    //@{
    FireResultListView* list_;
    //@}
};

#endif // __InfoConflictsTab_h_
