// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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

class StaticModel;
class Equipment;
class Dotation;
class Publisher_ABC;
struct ASN1T_MagicActionRecompletementPartiel;

//=============================================================================
// Created:  SBO 2005-07-27 
//=============================================================================
class LogisticSupplyRecompletionDialog : public QDialog
                                       , public kernel::Observer_ABC
                                       , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructor/Destructor
    //@{
             LogisticSupplyRecompletionDialog( QWidget* pParent, kernel::Controllers& controllers, Publisher_ABC& publisher, const StaticModel& staticModel, const kernel::Profile_ABC& profile );
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

    void FillPersonal   ( ASN1T_MagicActionRecompletementPartiel& action );
    void FillEquipments ( ASN1T_MagicActionRecompletementPartiel& action );
    void FillDotations  ( ASN1T_MagicActionRecompletementPartiel& action );
    void FillAmmunitions( ASN1T_MagicActionRecompletementPartiel& action );
    void FillSupplies   ( ASN1T_MagicActionRecompletementPartiel& action );

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
    Publisher_ABC& publisher_;
    const StaticModel& static_;
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
