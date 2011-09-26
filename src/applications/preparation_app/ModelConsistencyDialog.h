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

namespace kernel
{
    class ActionController;
    class Entity_ABC;
}

// =============================================================================
/** @class  ModelConsistencyDialog
    @brief  ModelConsistencyDialog
*/
// Created: ABR 2011-09-23
// =============================================================================
class ModelConsistencyDialog : public QDialog
                             , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ModelConsistencyDialog( QWidget* parent, Model& model, const StaticModel& staticModel, kernel::ActionController& actionController );
    virtual ~ModelConsistencyDialog();
    //@}

public slots:
    //! @name Slots
    //@{
    void CheckConsistency( unsigned int filters = ModelConsistencyChecker::eAllChecks );
    //@}

private:
    //! @name Types
    //@{
    enum E_Column { eID = 0, eName = 1, eDescription = 2 };
    //@}

    //! @name Helpers
    //@{
    void UpdateDataModel();
    void AddItem( int row, int col, QString text, const kernel::Entity_ABC& entity );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRefresh();
    void OnSelectionChanged( const QModelIndex& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController&         actionController_;
    ModelConsistencyChecker           checker_;
    unsigned int                      currentFilters_;
    QTableView*                       tableView_;
    QStandardItemModel*               dataModel_;
    QStringList                       horizontalHeaders_;
    std::map< unsigned int, QString > errorDescriptions_;
    //@}
};

#endif // __ModelConsistencyDialog_h_
