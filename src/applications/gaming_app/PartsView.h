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
    class BreakdownPart;
    class Controller;
    class Controllers;
    class Dotations_ABC;
    class Entity_ABC;
}

class LogMaintenanceConsign;

class PartsView : public gui::RichTableView
                , public tools::Observer_ABC
                , public tools::ElementObserver_ABC< kernel::Dotations_ABC >
{
    Q_OBJECT
public:
     PartsView( kernel::Controllers& controllers, QWidget* parent );
    ~PartsView();

    void Fill( const std::vector< kernel::BreakdownPart >& parts );
    void SelectEntity( const kernel::Entity_ABC* entity );
    bool IsValid() const;

signals:
    void Updated();

private:
    void NotifyUpdated( const kernel::Dotations_ABC& dotations );
    bool SetAvailable( int row, int available );

private:
    kernel::Controller& controller_;
    QStandardItemModel* model_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    bool valid_;
};

#endif
