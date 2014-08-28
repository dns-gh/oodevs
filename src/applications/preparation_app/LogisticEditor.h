// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _LogisticEditor_h
#define _LogisticEditor_h

#include "clients_kernel/SafePointer.h"

class StaticModel;

namespace gui
{
    class LogisticHierarchiesBase;
    class RichPushButton;
    template< typename T > class RichWidget;
}

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class Entity_ABC;
    class DotationType;
    class LogisticSupplyClass;
}

// =============================================================================
/** @class  LogisticEditor
    @brief  Color editor
*/
// Created: MMC 2011-06-23
// =============================================================================
class LogisticEditor : public QDialog
                     , public tools::Observer_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticEditor( QWidget* parent, const QString& objectName, kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~LogisticEditor();
    //@}

public:
    //! @name Operations
    //@{
    void Show( const kernel::Entity_ABC& entity );
    //@}

protected:
    //! @name Types
    //@{
    enum
    {
        eCategory,
        eDays
    };
    //@}

protected:
    //! @name Operations
    //@{
    virtual void SupplyHierarchy( const gui::LogisticHierarchiesBase& logHierarchy ) = 0;
    //@}

    //! @name Helpers
    //@{
    std::map< const kernel::LogisticSupplyClass*, double > GetDaysBySupplyClass() const;
    //@}

private:
    //! @name Helpers
    //@{
    void SupplyHierarchy( kernel::SafePointer< kernel::Entity_ABC > entity );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( QStandardItem* item );
    void Accept();
    void Reject();
    void closeEvent( QCloseEvent* pEvent );
    //@}

protected:
    //! @name Member data
    //@{
    const StaticModel& staticModel_;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QStandardItemModel* dataModel_;
    gui::RichWidget< QTableView >* tableView_;
    //@}
};

#endif // _LogisticEditor_h
