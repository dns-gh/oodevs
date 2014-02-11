// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef GAMING_APP_PARTS_VIEW_H
#define GAMING_APP_PARTS_VIEW_H

#include "clients_kernel/SafePointer.h"
#include "clients_gui/RichTableView.h"
#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"

#include <map>
#include <string>

namespace kernel
{
    class Controller;
    class Controllers;
    class Entity_ABC;
}

class LogMaintenanceConsign;
class SupplyStates;

class PartsView : public gui::RichTableView
                , public tools::Observer_ABC
                , public tools::ElementObserver_ABC< SupplyStates >
{
    Q_OBJECT
public:
     PartsView( kernel::Controllers& controllers, QWidget* parent );
    ~PartsView();

    void Select( kernel::Entity_ABC* handler, const LogMaintenanceConsign& consign );
    bool IsValid() const;

private:
    void Purge();
    void NotifyUpdated( const SupplyStates& states );

protected:
    void setSelection( const QRect& /*rect*/, QItemSelectionModel::SelectionFlags /*flags*/ );

private:
    kernel::Controller& controller_;
    QStandardItemModel* model_;
    kernel::SafePointer< kernel::Entity_ABC > base_;
    std::map< std::string, int > parts_;
    bool valid_;
};

#endif
