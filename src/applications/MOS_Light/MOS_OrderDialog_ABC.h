//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_OrderDialog_ABC.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 8 $
// $Workfile: MOS_OrderDialog_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_OrderDialog_ABC_h_
#define __MOS_OrderDialog_ABC_h_

#include "MOS_Types.h"
#include "MOS_AgentDialog_ABC.h"

#include "MOS_Order_Def.h"

class MOS_Agent;

class QListView;
class QListViewItem;
class QComboBox;
class QPopupMenu;
class QPushButton;
class QVBoxLayout;
class QGroupBox;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_OrderDialog_ABC : public MOS_AgentDialog_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_OrderDialog_ABC );

public:
     MOS_OrderDialog_ABC( QWidget* pParent = 0 );
    ~MOS_OrderDialog_ABC();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize( MOS_Agent& agent );
    virtual void Cleanup   ();
    //@}

protected slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void SlotAddLima                 ();
    void SlotAddLimaFunction         ();
    void SlotRemoveLimaOrLimaFunction();
    void SlotLimaMenuRequested       ( QListViewItem* pItem );
    void SlotLimaItemPressed         ( QListViewItem* pItem );

    void SlotChangeLimaFunction      ( int nIdx );

    void SlotValidate();
    void SlotCancel  ();
    //@}

protected:
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    //virtual void WriteOrderMsg( DIN::DIN_BufferedMessage& msg ) = 0;
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    void InitLimitComboBox       ( QComboBox& comboBox );
    void InitLimaComboBox        ( QComboBox& comboBox );
    void InitLimaFunctionComboBox( QComboBox& comboBox );

    //DIN::DIN_BufferedMessage WriteOrder();
    //@}

protected:
    QVBoxLayout* pMainLayout_;
    QGroupBox*   pFirstGroupBox_;
    QGroupBox*   pSecondGroupBox_;
    
private:
    QComboBox* pOrderTypeComboBox_;

    QComboBox* pLeftLimitComboBox_;
    QComboBox* pRightLimitComboBox_;

    QListView*  pLimaListView_;
    QComboBox*  pLimaComboBox_;
    QComboBox*  pLimaFunctionComboBox_;
    QComboBox*  pLimaFunctionValueComboBox_;
    QPopupMenu* pLimaListViewMenu_;

    QPushButton* pAddLimaFunctionButton_;

    QComboBox* pStartTimeComboBox_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_OrderDialog_ABC.inl"
#endif

#endif // __MOS_OrderDialog_ABC_h_
