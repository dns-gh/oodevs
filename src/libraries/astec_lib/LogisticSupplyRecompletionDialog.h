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

#include "SafePointer.h"

class Agent;
class Controllers;
class Model;
class Equipment;
class Dotation;
class Publisher_ABC;
struct ASN1T_MagicActionRecompletementPartiel;

//=============================================================================
// Created:  SBO 2005-07-27 
//=============================================================================
class LogisticSupplyRecompletionDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructor/Destructor
    //@{
             LogisticSupplyRecompletionDialog( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher, const Model& model );
    virtual ~LogisticSupplyRecompletionDialog();
    //@}

    //! @name Modifiers
    //@{
    void Show( const Agent& agent );
    //@}

private slots:
    //! @name Slots
    //@{
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
    void InitializeEquipments ( const Agent& agent );
    void InitializePersonal   ( const Agent& agent );
    void InitializeDotations  ( const Agent& agent );
    void InitializeAmmunitions( const Agent& agent );
    void InitializeSupplies   ( const Agent& agent );

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
    Publisher_ABC& publisher_;
    const Model& model_;
    SafePointer< Agent > agent_;

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
