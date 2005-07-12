//*****************************************************************************
//
// $Created : AML 03-04-01 $
// $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/MOS_LAU.h $
// $Author: Aml $
// $Modtime: 25/07/03 12:07 $
// $Revision: 14 $
// $Workfile: MOS_LAU.h $
//
//*****************************************************************************

#ifndef __MOS_LAU_h_
#define __MOS_LAU_h_

#include "LMP.h"

#include "LMP_ASN_Types.h"


class DIN::DIN_Engine;
class DIN::DIN_ConnectionProtocols;
//class DIN::DIN_ConnectionServiceServerUserCbk;

#define MOS_LAU_DEFAULT_MAGIC_CONNECTION    3232

enum MOS_LAU_Connector
{
//    eConnector_SIM_MOS = (DIN::DIN_Connector_ABC::DIN_ConnectionID)-1,        // pas utile de le connaitre ici
//    eConnector_SIM_LAU = 1,                                                   // pas utile de le connaitre ici
    eConnector_MOS_LAU = 2
};


//*****************************************************************************
// Object : classe minimaliste pour gerer les echanges avec le launcheur
//          (au final cela fera partie de MOS)
// Created: AML 03-04-01
//*****************************************************************************
class MOS_LAU
{
    enum 
    {
        eMsg_MosLau = 1,
        eMsg_LauMos,
        eMsg_LauSim,
        eMsg_SimLau,
    };

    MT_COPYNOTALLOWED( MOS_LAU );

public:
    explicit    MOS_LAU( const std::string& strHostName, uint16 nPort, DIN::DIN_Engine* pEngine );
    virtual     ~MOS_LAU();

    void        ConnectToLauncher();
    void        CloseConnection();
    void        Update();
    
    //-----------------------------------------------------------------------------
    /** @name Main methods */
    //-----------------------------------------------------------------------------
    //@{
    //@}
    
    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{
    void                    SetDinEngine( DIN::DIN_Engine* pEngine );
    const std::string&      GetHostName() const;
    void                    SetHostName( const std::string& strHostName );    
    //@}

    //-------------------------------------------------------------------------
    /** @name Messages tools */
    //-------------------------------------------------------------------------
    //@{
    DIN::DIN_BufferedMessage BuildMessage();
    //@}


    //-------------------------------------------------------------------------
    /** @name Connection service callbacks */
    //-------------------------------------------------------------------------
    //@{
    void OnConnected     ( DIN::DIN_Link& link );
    void OnNotConnected  ( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    void OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}


    //-------------------------------------------------------------------------
    /** @name Service callback */
    //-------------------------------------------------------------------------
    //@{
    bool OnError( DIN::DIN_Link &link, const DIN::DIN_ErrorDescription &info );
    //@}

    //-----------------------------------------------------------------------------
    /** @name Message To Send */
    //-----------------------------------------------------------------------------
    //@{
        bool32 SendMsg_MosLau_RequestListMission    ();
        bool32 SendMsg_MosLau_LaunchSim             ( const uint32 nExerciceID, const uint32 nScenarioID, const std::string& strCommandLine = "" );
        bool32 SendMsg_MosLau_RequestListSim        ();
        bool32 SendMsg_MosLau_StopSim               ( const uint32 nExerciceID );

        bool32 SendMsg_MosLau_RequestListCheckpoint ( const uint32 nExerciceID );
        bool32 SendMsg_MosLau_LaunchSimFromCheckpoint( const uint32 nExerciceID, const uint32 nCheckpointID, const std::string& strCommandLine = "" );


        bool32 SendMsg_MosLau( ASN1T_MsgsMosLauncher& asnMsg );

    //@}

    //-------------------------------------------------------------------------
    /** @name Msg callbacks */
    //-------------------------------------------------------------------------
    //@{     

    void OnReceiveMsg_LauMos_ListMission( ASN1T_MsgLauMos_ListMission* pMsg );
    void OnReceiveMsg_LauMos_SimLaunched( ASN1T_MsgLauMos_SimLaunched* pMsg );
    void OnReceiveMsg_LauMos_ListSim( ASN1T_MsgLauMos_ListSim* pMsg );
    void OnReceiveMsg_LauMos_SimFailure( ASN1T_MsgLauMos_SimFailure* pMsg );
    void OnReceiveMsg_LauMos_ListCheckpoint( ASN1T_MsgLauMos_ListCheckpoint* pMsg );
    void OnReceiveMsg_LauMos_ExecSuccess( ASN1T_MsgLauMos_ExecSuccess* pMsg );

    void OnReceiveMsg_LauMos( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );


    //@}


    static MOS_LAU* GetTheOnlyMOS_LAU( void );
    

private:

    // partie variables d'etats
    


    // partie reseau vrac

    std::string             strHostName_;
    uint16                  nPort_;

    DIN::DIN_Link*          pSessionWithLauncher_;  // (not owned)
    DIN::DIN_Engine*        pEngine_;               // (not owned) (potentially shared with other objects)


    DIN::DIN_ConnectionProtocols                            connectionProtocols_;
    DIN::DIN_ConnectionServiceClientUserCbk< MOS_LAU >*     pConnService_;      // (owned)

    DIN::DIN_MessageServiceUserCbk<MOS_LAU>*                pMessageService_;   // (owned)

    
    ASN1OCTET               aASNEncodeBuffer_[100000];
    ASN1OCTET               aASNDecodeBuffer_[100000];


private:

    static MOS_LAU*         static_pTheOnlyMOS_LAU;
};


#ifdef USE_INLINE
#	include "MOS_LAU.inl"
#endif

#endif // __MOS_LAU_h_
