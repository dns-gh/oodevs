// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __EquipmentTransferDialog_h_
#define __EquipmentTransferDialog_h_

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include <boost/noncopyable.hpp>

#pragma warning( push, 0 )
#include <Qt3Support/q3table.h>
#pragma warning( pop )

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
    namespace parameters
    {
        class ParameterList;
    }
}

class StaticModel;

// =============================================================================
/** @class  EquipmentTransferDialog
    @brief  EquipmentTransferDialog
*/
// Created: ABR 2011-06-15
// =============================================================================
class EquipmentTransferDialog : public QDialog
                              , public tools::Observer_ABC
                              , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                              , public tools::SelectionObserver_ABC
                              , public tools::SelectionObserver_Base< kernel::Agent_ABC >
                              , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EquipmentTransferDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~EquipmentTransferDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void Select( const kernel::Agent_ABC& agent );
    virtual void BeforeSelection();
    virtual void AfterSelection();
    void InitializeEquipments();
    void FillEquipments( actions::parameters::ParameterList& list );
    bool CheckValidity() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void closeEvent( QCloseEvent* e );
    void Reject();
    void Show();
    void Validate();
    //@}

private:
    //! @name Type
    //@{
    class EquipmentTable : public Q3Table
    {
    public:
                 EquipmentTable( int numRows, int numCols, QWidget * parent = 0, const char * name = 0 ) : Q3Table( numRows, numCols, parent, name ) {}
        virtual ~EquipmentTable() {}
        virtual void setCellContentFromEditor ( int row, int col ) { Q3Table::setCellContentFromEditor( row, col ); } // $$$$ ABR 2011-06-16: Needed if clicking on Ok when editing cell
    };
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                     controllers_;
    const StaticModel&                       static_;
    actions::ActionsModel&                   actionsModel_;
    const kernel::Time_ABC&                  simulation_;
    const kernel::Profile_ABC&               profile_;

    kernel::SafePointer< kernel::Agent_ABC > selectedFrom_;
    kernel::SafePointer< kernel::Agent_ABC > selectedTo_;
    QLabel*                                  labelFrom_;
    QLabel*                                  labelTo_;
    QPushButton*                             okButton_;
    EquipmentTable*                          equipmentTable_;
    std::map< QString, unsigned int >        equipmentIdMap_;
    //@}
};

#endif // __EquipmentTransferDialog_h_
