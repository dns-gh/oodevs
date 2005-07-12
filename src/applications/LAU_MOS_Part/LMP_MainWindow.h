//*****************************************************************************
//
// $Created : AML 03-04-02 $
//  $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/LMP_MainWindow.h $
//   $Author: Aml $
//  $Modtime: 25/07/03 12:14 $
// $Revision: 16 $
// $Workfile: LMP_MainWindow.h $
//
//*****************************************************************************

#ifndef __LMP_MainWindow_h_
#define __LMP_MainWindow_h_

#include <qmainwindow.h>

#include "LMP.h"


#include "LMP_ASN_Types.h"


class LMP_Workspace;

class QToolButton;
class QListBox;
class QTable;
class QLabel;
class QLineEdit;
class QPushButton;

//*****************************************************************************
// Object : PUT THE COMMENTS ON THE CLASS HERE
// Created: AML 03-04-02
//*****************************************************************************
class LMP_MainWindow : public QMainWindow
{
    Q_OBJECT;

    MT_COPYNOTALLOWED( LMP_MainWindow );

public:
    explicit LMP_MainWindow( LMP_Workspace& workspace );
    virtual ~LMP_MainWindow();
    
    //-----------------------------------------------------------------------------
    /** @name Main methods */
    //-----------------------------------------------------------------------------
    //@{

    void ClearListMission();
    void ClearListCheckpoint();
    void UpdateListMission( ASN1T_MsgLauMos_ListMission* pMsg );
    void UpdateListSim( ASN1T_MsgLauMos_ListSim* pMsg );
    void UpdateListCheckpoint( ASN1T_MsgLauMos_ListCheckpoint* pMsg );
    void UpdateInfoLaunch( const uint32 nExerciceID, const ASN1T_EnumCtrlErrorCodeLauncher nErrorCode, const uint16 nPort, const int nMagic );
    void UpdateInfoFailure( const uint32 nExerciceID );
    void UpdateInfoExecSuccess( const uint32 nExerciceID, const bool32 bFinishedState );

    void ResetTableInfoSim  ( const uint32 nExerciceID );
    void UpdateTableInfoSim ( const uint32 nExerciceID, const int32 nLaunched = -1, const int32 nInit = -1, const int32 nInitStatus = -1, const int32 nRunning = -1, const int32 nStopped = -1, const int32 nExecSuccess = -1, const int32 nPort = -1, const int32 nMagic = -1 );
    
    void SetConnected( const bool32 bNewVal = true );

    void ShowLastLineOfListBox( void ) const;
    
    
    //@}


    
    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{
    //@}

    static LMP_MainWindow* GetMainWindow( void );   // un peu crado mais l'objet MOS_LAU est censer etre facilement incluable dans MOS ou MOS_Light

    static      const char* GetLibelleFromLaunchErrorCode( const int32 nLaunchErrorCode );

private slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotPushButtonReconnect();
    void SlotPushButtonRequestListMission();
    void SlotPushButtonRequestListSim();
    void SlotPushButtonRequestListCheckpoint();
    void SlotPushButtonLaunchSelectedMission();
    void SlotPushButtonLaunchSelectedCheckpoint();
    void SlotPushButtonStopSimulation();
    void SlotPushButtonStopAllSimulation();
    //@}


private:

    LMP_Workspace&      workSpace_;                         // (not owned)

    QToolButton*        pButtonReconnect_;                  // (not owned)
    QToolButton*        pButtonRequestListMission_;         // (not owned)
    QToolButton*        pButtonRequestListSim_;             // (not owned)
    QToolButton*        pButtonRequestListCheckpoint_;      // (not owned)

    QLineEdit*          pLineEditOption_;                   // (not owned)
    QPushButton*        pButtonLaunchSelectedMission_;      // (not owned)
    QPushButton*        pButtonLaunchSelectedCheckpoint_;   // (not owned)

    QLineEdit*          pLineEditHostname_;                 // (not owned)

    QListBox*           pListBoxExistingMission_;           // (not owned)
    QListBox*           pListBoxExistingCheckpoint_;        // (not owned)
    QTable*             pTableSimState_;                    // (not owned)
    QListBox*           pListBoxSimStatus_;                 // (not owned)

    QLineEdit*          pLineEditExerciceID_;               // (not owned)
    QPushButton*        pButtonStopSimulation_;             // (not owned)
    QPushButton*        pButtonStopAllSimulation_;          // (not owned)

    QLabel*             pLabelStatus_;                      // (not owned)

    QPixmap*            pPixmapError_;                      // (not owned)

    std::string         strCommandLineToSend_;              // must be stored, because the message will be sended later...


private:

    static LMP_MainWindow* static_pTheOnlyMainWindow;

};


#ifdef USE_INLINE
#   include "LMP_MainWindow.inl"
#endif

#endif // __LMP_MainWindow_h_
