// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DatasetList_h_
#define __DatasetList_h_

#include "frontend/Dataset.h"
#include "ElementListView.h"

namespace kernel
{
    class Controllers;
}

namespace frontend
{
    class PhysicalModel;
}

// =============================================================================
/** @class  DatasetList
    @brief  DatasetList
*/
// Created: SBO 2007-02-01
// =============================================================================
class DatasetList : public ElementListView< frontend::Dataset >
                  , public kernel::ElementObserver_ABC< frontend::PhysicalModel >
{

public:
    //! @name Constructors/Destructor
    //@{
             DatasetList( QWidget* parent, kernel::Controllers& controllers );
    virtual ~DatasetList();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DatasetList( const DatasetList& );            //!< Copy constructor
    DatasetList& operator=( const DatasetList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const frontend::PhysicalModel& element );
    virtual void NotifyUpdated( const frontend::PhysicalModel& element );
    virtual void NotifyDeleted( const frontend::PhysicalModel& element );
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __DatasetList_h_
