//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogisticSupplyRecompletionDialog.h $
// $Author: Sbo $
// $Modtime: 29/07/05 11:24 $
// $Revision: 2 $
// $Workfile: LogisticSupplyRecompletionDialog.h $
//
//*****************************************************************************

#ifndef __LogisticSupplyRecompletionDialog_h_
#define __LogisticSupplyRecompletionDialog_h_

#include "SafePointer.h"

class Agent;
class Controllers;
class Model;

//=============================================================================
// Created:  SBO 2005-07-27 
//=============================================================================
class LogisticSupplyRecompletionDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             LogisticSupplyRecompletionDialog( QWidget* pParent, Controllers& controllers, const Model& model );
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
    //@}

    //! @name Types
    //@{
    typedef std::vector< unsigned > T_Ints;
    struct Item {
        QString  name_;
        unsigned long id_;
        unsigned quantity_;
    };
    //@}
   
private:
    //! @name Member data
    //@{
    const Model& model_;
    SafePointer< Agent > agent_;

    QGroupBox*       pEquiPersoGroupBox_;
    QTable*          pEquipmentsTable_;
    QStringList      equipmentsList_;
    T_Ints           equipmentsMax_;

    QTable*          pPersonalsTable_;

    QGroupBox*       pDotationsGroupBox_;
    QTable*          pDotationsTable_;
    QTable*          pMunitionsFamilyTable_;

    QGroupBox*       pStockGroupBox_;
    QTable*          pStockTable_;
    //@}
};

#endif // __LogisticSupplyRecompletionDialog_h_
