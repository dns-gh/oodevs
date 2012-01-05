// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FormationHierarchyEditor_h_
#define __FormationHierarchyEditor_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Automat_ABC;
    class Entity_ABC;
    class Team_ABC;
    class Controllers;
    class Formation_ABC;
    class ModelLoaded;
    class ModelUnLoaded;
    class SymbolFactory;
    class FormationLevels;
}

namespace gui
{
    class EntitySymbols;
}

namespace tools
{
    class ExerciseConfig;
}

class Formation;

// =============================================================================
/** @class  FormationHierarchyEditor
    @brief  Symbol editor
*/
// Created: MMC 2012-01-05
// =============================================================================
class FormationHierarchyEditor: public QObject
                  , public tools::Observer_ABC
                  , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                  , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FormationHierarchyEditor( QWidget* parent, kernel::Controllers& controllers, const kernel::FormationLevels& levels );
    virtual ~FormationHierarchyEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChangeLevel( int levelId );
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateHierarchies();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< Formation > selected_;
    const kernel::FormationLevels& levels_;
    //@}
};

#endif // __FormationHierarchyEditor_h_
