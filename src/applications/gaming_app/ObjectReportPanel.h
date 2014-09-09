// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectReportPanel_h_
#define __ObjectReportPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/SelectionObserver_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class KnowledgeConverter_ABC;
    class Object_ABC;
}

namespace gui
{
    class DisplayExtractor;
}

// =============================================================================
/** @class  ObjectReportPanel
    @brief  ObjectReportPanel
*/
// Created: SBO 2005-09-08
// =============================================================================
class ObjectReportPanel : public gui::InfoPanel_ABC
                        , public tools::Observer_ABC
                        , public tools::SelectionObserver< kernel::Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectReportPanel( QWidget* parent, gui::PanelStack_ABC& panel,
                 kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                 const kernel::KnowledgeConverter_ABC& converter );
    virtual ~ObjectReportPanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Object_ABC& );
    virtual void NotifySelected( const kernel::Object_ABC* );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Object_ABC > selected_;
    //@}
};

#endif // __ObjectReportPanel_h_
