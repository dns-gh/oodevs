// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyRecompletionDialog_h_
#define __LogisticSupplyRecompletionDialog_h_

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class Profile_ABC;
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
class Equipment;
class Dotation;
class ActionPublisher;
class Simulation;

//=============================================================================
// Created:  SBO 2005-07-27 
//=============================================================================
class LogisticSupplyRecompletionDialog : public QDialog
                                       , public tools::Observer_ABC
                                       , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructor/Destructor
    //@{
             LogisticSupplyRecompletionDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const Simulation& simulation, const kernel::Profile_ABC& profile );
    virtual ~LogisticSupplyRecompletionDialog();
    //@}

    //! @name Modifiers
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show                   ();
    void Validate               ();
    void Reject                 ();
    void closeEvent             ( QCloseEvent * e );
    void OnDotationChanged      ( int nRow, int nCol );
    void OnMunitionFamilyChanged( int nRow, int nCol );
    void OnEquipmentChanged     ( int nRow, int nCol );
    void OnPersonalChanged      ( int nRow, int nCol );
    void OnStockChanged         ( int nRow, int nCol );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticSupplyRecompletionDialog( const LogisticSupplyRecompletionDialog& );
    LogisticSupplyRecompletionDialog& operator=( const LogisticSupplyRecompletionDialog& );
    //@}

private:
    //! @name Helpers
    //@{
    void InitializeEquipments ();
    void InitializePersonal   ();
    void InitializeDotations  ();
    void InitializeAmmunitions();
    void InitializeSupplies   ();

    void AddPersonal( unsigned pos, const QString& label, unsigned max );
    void AddAmmunition( unsigned pos, const QString& label );

    void FillPersonal   ( actions::parameters::ParameterList& list );
    void FillEquipments ( actions::parameters::ParameterList& list );
    void FillDotations  ( actions::parameters::ParameterList& list );
    void FillAmmunitions( actions::parameters::ParameterList& list );
    void FillSupplies   ( actions::parameters::ParameterList& list );

    QStringList FilterEquipmentList() const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< unsigned > T_Ints;
    struct Item {
        QString  name_;
        unsigned long id_;
        unsigned quantity_;
    };

    typedef std::map< QString, const Equipment* > T_Equipments;
    typedef std::map< QString, const Dotation* >  T_Dotations;
    //@}
   
private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    ActionPublisher& actionPublisher_;
    actions::ActionsModel& actionsModel_;
    const Simulation& simulation_;
    const kernel::Profile_ABC& profile_;

    kernel::SafePointer< kernel::Agent_ABC > selected_;

    QTable*          equipmentsTable_;
    QStringList      equipmentsList_;
    T_Equipments     equipments_;
    T_Ints           equipmentsMax_;

    QTable*          personalsTable_;
    QTable*          dotationsTable_;
    QTable*          munitionsFamilyTable_;
    QTable*          stockTable_;
    T_Dotations      stocks_;
    //@}
};

#endif // __LogisticSupplyRecompletionDialog_h_
