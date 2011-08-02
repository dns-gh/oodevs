// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MedicalCapacityUpdateDialog_h_
#define __MedicalCapacityUpdateDialog_h_

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/lexical_cast.hpp>

namespace kernel
{
    class Object_ABC;
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
    class StaticModel;
}

namespace actions
{
    class ActionsModel;
    namespace parameters
    {
        class ParameterList;
    }
}

struct MedicalTreatmentCapacity;
class Publisher_ABC;

class QLineEdit;
class Q3VBox;
class QSpinBox;
class Q3ListBox;

// =============================================================================
/** @class  MedicalCapacityUpdateDialog
    @brief  MedicalCapacityUpdateDialog
*/
// Created: JCR 2010-06-28
// =============================================================================
class MedicalCapacityUpdateDialog : public QDialog
                               , public tools::Observer_ABC
                               , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
            MedicalCapacityUpdateDialog( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::StaticModel& staticModel, const kernel::Time_ABC& simulation, Publisher_ABC& publisher, const kernel::Profile_ABC& profile );
    virtual ~MedicalCapacityUpdateDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& object, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show                   ();
    void Validate               ();
    void Reject                 ();
    void closeEvent             ( QCloseEvent * e );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalCapacityUpdateDialog( const MedicalCapacityUpdateDialog& );            //!< Copy constructor
    MedicalCapacityUpdateDialog& operator=( const MedicalCapacityUpdateDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateCanvas();
    void FillAttributes( actions::parameters::ParameterList& msg ) const;
    void Send( actions::parameters::ParameterList& msg ) const;
    void InitializeCapacities();
    //@}

private:
    //! @name Types
    //@{
    struct Capacity
    {
        explicit Capacity( QWidget* parent, const MedicalTreatmentCapacity& capacity );
        Capacity( const Capacity& rhs );
        void Update( const MedicalTreatmentCapacity& capacity );

        int id_;
        std::string name_;
        QSpinBox*   baseline_;
        QSpinBox*   patients_;
    };
    typedef boost::ptr_vector< Capacity >::const_iterator CIT_Capacities;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    actions::ActionsModel& actionsModel_;
    const kernel::StaticModel& static_;
    const kernel::Profile_ABC& profile_;
    const kernel::Time_ABC& simulation_;
    kernel::SafePointer< kernel::Object_ABC > selected_;
    //@}

    //! @name
    //@{
    QLineEdit* referenceID_;
    Q3VBox* vbox_;
    QSpinBox* doctors_;
    gui::ValuedComboBox< std::string >* status_;
    boost::ptr_vector< Capacity > capacities_;
    //@}
};

#endif // __MedicalCapacityUpdateDialog_h_
