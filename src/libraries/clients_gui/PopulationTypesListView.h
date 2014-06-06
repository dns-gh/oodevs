// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationTypesListView_h_
#define __PopulationTypesListView_h_

#include "RichTreeView.h"

#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
    class PopulationType;
    struct PopulationPrototype;
}

namespace gui
{

// =============================================================================
/** @class  PopulationTypesListView
    @brief  PopulationTypesListView
*/
// Created: SBO 2006-11-09
// =============================================================================
class PopulationTypesListView : public gui::RichTreeView
                              , public tools::Observer_ABC
                              , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                              , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
    PopulationTypesListView( const QString& objectName, QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::PopulationType >& types, const QSpinBox& number );
    virtual ~PopulationTypesListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual QStringList MimeTypes() const;
    virtual QMimeData* MimeData( const QModelIndexList& indexes, bool& overriden ) const;
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const tools::Resolver_ABC< kernel::PopulationType >& types_;
    const QSpinBox& number_;
    std::unique_ptr< kernel::PopulationPrototype > prototype_;
    //@}
};

}

#endif // __PopulationTypesListView_h_
