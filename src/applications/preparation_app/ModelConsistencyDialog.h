// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ModelConsistencyDialog_h_
#define __ModelConsistencyDialog_h_

#include "clients_gui/ConsistencyDialog_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "preparation/ConsistencyErrorTypes.h"

namespace kernel
{
    class ActionController;
    class Controllers;
    class Entity_ABC;
}

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  ModelConsistencyDialog
    @brief  ModelConsistencyDialog
*/
// Created: ABR 2011-09-23
// =============================================================================
class ModelConsistencyDialog : public gui::ConsistencyDialog< E_ConsistencyCheck, kernel::SafePointer< kernel::Entity_ABC > >
{
    typedef gui::ConsistencyDialog< E_ConsistencyCheck, kernel::SafePointer< kernel::Entity_ABC > > T_Parent;

public:
    //! @name Constructors/Destructor
    //@{
             ModelConsistencyDialog( QWidget* parent, Model& model, const StaticModel& staticModel, kernel::Controllers& controllers, tools::RealFileLoaderObserver_ABC& fileLoaderObserver );
    virtual ~ModelConsistencyDialog();
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnSelectionChanged( const QModelIndex& );
    //@}

    //! @name Helpers
    //@{
    virtual void UpdateDataModel();
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController& actionController_;
    const kernel::SafePointer< kernel::Entity_ABC > emptyEntity_; 
    //@}
};

#endif // __ModelConsistencyDialog_h_
