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

#include "Types.h"

class QListView;
class QPopupMenu;
class QCheckBox;
class QSpinBox;
class QGroupBox;
class QTable;
class QListViewItem;
class Agent;

//=============================================================================
// Created:  SBO 2005-07-27 
//=============================================================================
class LogisticSupplyRecompletionDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( LogisticSupplyRecompletionDialog );

public:
    //! @name Constructor/Destructor
    //@{
     LogisticSupplyRecompletionDialog( QWidget* pParent = 0 );
    ~LogisticSupplyRecompletionDialog();
    //@}

    //! @name Modifiers
    //@{
    void SetAgent( const Agent& agent );
    //@}

private slots:
    //! @name Operations
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
    //! @name Types
    //@{
    typedef std::map< uint, const Agent* > T_AgentIDMap;
    typedef T_AgentIDMap::const_iterator       CIT_AgentIDMap;
    //@}
   
private:
    const Agent* pAgent_;

    QGroupBox*       pEquiPersoGroupBox_;
    QTable*          pEquipmentsTable_;
    QStringList*     pEquipmentsStringList_;
    T_IntVector      equipmentsMax_;
    QTable*          pPersonalsTable_;

    QGroupBox*       pDotationsGroupBox_;
    QTable*          pDotationsTable_;
    QTable*          pMunitionsFamilyTable_;

    QGroupBox*       pStockGroupBox_;
    QTable*          pStockTable_;
};

#endif // __LogisticSupplyRecompletionDialog_h_
