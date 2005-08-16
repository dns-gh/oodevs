//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogisticSupplyRecompletionDialog.h $
// $Author: Sbo $
// $Modtime: 29/07/05 11:24 $
// $Revision: 2 $
// $Workfile: MOS_LogisticSupplyRecompletionDialog.h $
//
//*****************************************************************************

#ifndef __MOS_LogisticSupplyRecompletionDialog_h_
#define __MOS_LogisticSupplyRecompletionDialog_h_

#include "MOS_Types.h"

class QListView;
class QPopupMenu;
class QCheckBox;
class QSpinBox;
class QGroupBox;
class QTable;
class QListViewItem;
class MOS_Agent;

//=============================================================================
// Created:  SBO 2005-07-27 
//=============================================================================
class MOS_LogisticSupplyRecompletionDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_LogisticSupplyRecompletionDialog );

public:
     MOS_LogisticSupplyRecompletionDialog( QWidget* pParent = 0 );
    ~MOS_LogisticSupplyRecompletionDialog();

    void SetAgent( const MOS_Agent& agent );

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Validate               ();
    void Reject                 ();
    void closeEvent             ( QCloseEvent * e );
    void OnDotationChanged      ( int nRow, int nCol );
    void OnMunitionFamilyChanged( int nRow, int nCol );
    void OnEquipmentChanged     ( int nRow, int nCol );
    void OnPersonalChanged      ( int nRow, int nCol );
    //void OnMunitionChanged      ( int nRow, int nCol );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, const MOS_Agent* > T_AgentIDMap;
    typedef T_AgentIDMap::const_iterator       CIT_AgentIDMap;
    //@}
   
private:
    const MOS_Agent* pAgent_;

    QGroupBox*       pEquiPersoGroupBox_;
    QTable*          pEquipmentsTable_;
    QStringList*     pEquipmentsStringList_;
    T_IntVector      equipmentsMax_;
    QTable*          pPersonalsTable_;

    QGroupBox*       pDotationsGroupBox_;
    QTable*          pDotationsTable_;
    QTable*          pMunitionsFamilyTable_;
    /*
    QTable*          pMunitionsTable_;
    QStringList*     pMunitionsStringList_;
    */
};

#include "MOS_LogisticSupplyRecompletionDialog.inl"

#endif // __MOS_LogisticSupplyRecompletionDialog_h_
