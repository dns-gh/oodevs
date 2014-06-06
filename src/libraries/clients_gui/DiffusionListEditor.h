// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __DiffusionListEditor_h_
#define __DiffusionListEditor_h_

#include "VerticalHeaderTableView.h"
#include "clients_kernel/SafePointer.h"
#include "DiffusionListFunctors.h"
#include <tools/Resolver.h>

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class ExtensionTypes;
    class Formation_ABC;
    class Team_ABC;
}

namespace gui
{

// =============================================================================
/** @class  DiffusionListEditor
    @brief  DiffusionListEditor
*/
// Created: ABR 2012-02-23
// =============================================================================
class DiffusionListEditor : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DiffusionListEditor( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver< kernel::Agent_ABC >& agents, const tools::Resolver< kernel::Formation_ABC >& formations, const kernel::ExtensionTypes& extensions );
    virtual ~DiffusionListEditor();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Fill( kernel::SafePointer< kernel::Entity_ABC > currentTeam, const kernel::Entity_ABC* currentAgent = 0 );
    void Commit();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClear();
    void OnGenerate();
    void OnGeometriesChanged();
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< QStyle > style_;
    kernel::SafePointer< kernel::Entity_ABC >       currentTeam_;
    const tools::Resolver< kernel::Agent_ABC >&     agents_;
    const tools::Resolver< kernel::Formation_ABC >& formations_;
    const kernel::ExtensionTypes&                   extensions_;
    VerticalHeaderTableView                         tableView_;
    QStandardItemModel                              dataModel_;
    DiffusionListData::T_Entities                   emitters_;
    DiffusionListData::T_Entities                   receivers_;
    std::string                                     extensionName_;
    //@}
};

}

#endif // __DiffusionListEditor_h_
