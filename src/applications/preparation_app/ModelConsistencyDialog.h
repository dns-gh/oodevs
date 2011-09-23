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

#include <boost/noncopyable.hpp>

#include "preparation/ModelConsistencyChecker.h"

// =============================================================================
/** @class  ModelConsistencyDialog
    @brief  ModelConsistencyDialog
*/
// Created: ABR 2011-09-23
// =============================================================================
class ModelConsistencyDialog : public QDialog
                             , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelConsistencyDialog( QWidget* parent, Model& model, const StaticModel& staticModel );
    virtual ~ModelConsistencyDialog();
    //@}

    //! @name Operations
    //@{
    void CheckConsistency( unsigned int filters = ModelConsistencyChecker::eAllChecks );
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateErrorListView();
    //@}

private:
    //! @name Member data
    //@{
    ModelConsistencyChecker checker_;
    unsigned int            currentFilters_;
    //@}
};

#endif // __ModelConsistencyDialog_h_
