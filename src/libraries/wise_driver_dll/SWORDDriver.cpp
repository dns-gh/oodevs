#include "wise_driver_dll_pch.h"
#include "SWORDDriver.h"
#include "SwordFacade.h"

CSWORDDriver::CSWORDDriver(void) :
    CWISEDriver()
{
}

CSWORDDriver::~CSWORDDriver(void)
{
}

WISE_RESULT CSWORDDriver::OnInitialize( IWISEDriverSettings* pSettings )
{
    WISE_RESULT wResult = WISE_OK;
    
    try
    {
        SetThreadingMode( CWISEDriver::ThreadingOnNoCommands );

        // Call base class to perform validation...
        CHECK_WISE_RESULT_EX( CWISEDriver::OnInitialize( pSettings ) );
        if( pSettings )
            sword_.reset( new SwordFacade( *pSettings, *this ) );
    }
    catch( WISE_RESULT& wErr )
    {
        wResult = wErr;
    }
    return wResult;
}

WISE_RESULT CSWORDDriver::OnUninitialize( void )
{
    WISE_RESULT wResult = WISE_OK;
    
    try
    {
        sword_.reset();
        CHECK_WISE_RESULT_EX( CWISEDriver::OnUninitialize() );
    }
    catch( WISE_RESULT& wErr )
    {
        wResult = wErr;
    }
    return wResult;
}

WISE_RESULT CSWORDDriver::OnCreateDatabase( const std::wstring& strDatabaseName, 
        const WISE_HANDLE& hDatabase, const WISE_HANDLE& hTemplateDatabase,
        const WISEDatabaseDistType& eDatabaseDistType, const WISEDatabaseType& eDatabaseType, 
        const WALOwnershipMode& eModeOwnership, const WALAttributeQualityMode& eModeAttributeQuality,
        const WALAttributeTimeMode& eModeAttributeTime )
{
    WISE_RESULT wResult = WISE_OK;
    
    try
    {
        // Validate internal state and parameters...
        CHECK_VALID_POINTER_EX( m_pSink, MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_NOT_INITIATED ) );

        // Call base class to save the database in the name/handle index...
        CHECK_WISE_RESULT_EX( CWISEDriver::OnCreateDatabase( strDatabaseName, hDatabase, hTemplateDatabase, 
                eDatabaseDistType,eDatabaseType, eModeOwnership, eModeAttributeQuality, eModeAttributeTime ) );
    
        // Check the database type to avoid distribution of template databases.
        if( eDatabaseType == eApplication )
            sword_->Connect( hDatabase );
    }
    catch( WISE_RESULT& wErr )
    {
        wResult = wErr;
    }
    return wResult;
}

WISE_RESULT CSWORDDriver::OnCloseDatabase( const WISE_HANDLE& hDatabase )
{
    WISE_RESULT wResult = WISE_OK;

    if( hDatabase == WISE_INVALID_HANDLE )
    {
        return MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_INVALID_ARG );
    }

    try
    {
        // Validate internal state and parameters...
        CHECK_VALID_POINTER_EX( m_pSink, MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_NOT_INITIATED ) );
        sword_->Disconnect();
        
        // Call base class implementation in order to remove the database from the index...
        CHECK_WISE_RESULT_EX( CWISEDriver::OnCloseDatabase( hDatabase ) );
    }
    catch( WISE_RESULT& wErr )
    {
        wResult = wErr;
    }
    return wResult;
}

WISE_RESULT CSWORDDriver::OnAddObject( const timeb& timeStamp, const WISE_HANDLE& hDatabase, 
        const WISE_HANDLE& hObject, const std::wstring& strObjectName,
        const std::map<std::wstring, WISE_HANDLE >& associatedObjects, 
        const std::map<std::wstring, std::list<WISE_HANDLE> >& associatedObjectLists )
{
    WISE_RESULT wResult = WISE_OK;

    if( (hDatabase == WISE_INVALID_HANDLE) || (hObject == WISE_INVALID_HANDLE) )
    {
        return MAKE_WISE_RESULT(WISE_FACILITY_COM_ADAPTER, WISE_E_INVALID_ARG);
    }
    
    try
    {
        // Validate internal state and parameters...
        CHECK_VALID_POINTER_EX( m_pSink, MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_NOT_INITIATED ) );

        //
        // TODO: Create new object on communications protocol.
        //

        // Call base class implementation to add object to the local index
        CHECK_WISE_RESULT_EX( CWISEDriver::OnAddObject( timeStamp, hDatabase, hObject, strObjectName, 
                associatedObjects, associatedObjectLists ) );

        // Loopback call into WISE to activate triggers.
        WISE_HANDLE hObjectLoop = WISE_INVALID_HANDLE;
        GetSink()->CreateWISEObject( strObjectName, hObjectLoop );
        GetSink()->AddObjectToDatabase( hDatabase, hObjectLoop );
    }
    catch( WISE_RESULT& wErr )
    {
        wResult = wErr;
    }
    return wResult;
}

WISE_RESULT CSWORDDriver::OnRemoveObject( const timeb& timeStamp, const WISE_HANDLE& hDatabase, 
        const WISE_HANDLE& hObject )
{
    WISE_RESULT wResult = WISE_OK;

    if( (hDatabase == WISE_INVALID_HANDLE) || (hObject == WISE_INVALID_HANDLE) )
    {
        return MAKE_WISE_RESULT(WISE_FACILITY_COM_ADAPTER, WISE_E_INVALID_ARG);
    }
    
    try
    {
        // Validate internal state and parameters...
        CHECK_VALID_POINTER_EX( m_pSink, MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_NOT_INITIATED ) );

        //
        // TODO: Remove object on communications protocol.
        //
    
        // Call base class to update local cache
        CHECK_WISE_RESULT_EX( CWISEDriver::OnRemoveObject( timeStamp, hDatabase, hObject ) );

        // Loopback call into WISE to activate triggers.
        GetSink()->RemoveObjectFromDatabase( hDatabase, hObject );
    }
    catch( WISE_RESULT& wErr )
    {
        wResult = wErr;
    }
    return wResult;
}

WISE_RESULT CSWORDDriver::OnSendEvent( const timeb& timeStamp, const WISE_HANDLE& hDatabase, 
        const WISE_HANDLE& hEvent, const std::map<std::wstring, WISE_HANDLE >& associatedObjects, 
        const std::map<std::wstring, std::list<WISE_HANDLE> >& associatedObjectLists )
{
    WISE_RESULT wResult = WISE_OK;
    
    try
    {
        // Validate internal state and parameters...
        CHECK_VALID_POINTER_EX( m_pSink, MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_NOT_INITIATED ) );

        // Call base class to perform initialization...
        CHECK_WISE_RESULT_EX( CWISEDriver::OnSendEvent( timeStamp, hDatabase, hEvent, associatedObjects, associatedObjectLists ) );
    
        if( sword_.get() )
            sword_->OnEventReceived( hEvent );

        // Loopback call into WISE to activate triggers.
        GetSink()->SendEvent( hDatabase, hEvent );
    }
    catch( WISE_RESULT& wErr )
    {
        wResult = wErr;
    }
    return wResult;
}

WISE_RESULT CSWORDDriver::OnUpdateAttribute( const timeb& timeStamp, 
        const WISE_HANDLE& hDatabase, const WISE_HANDLE& hObject, const WISE_HANDLE& hAttribute, 
        const CWISEValueUnion& value, const WISE_ATTRIBUTE_QUALITY& quality )
{
    WISE_RESULT wResult = WISE_OK;
    
    try
    {
        // Validate internal state and parameters...
        CHECK_VALID_POINTER_EX( m_pSink, MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_NOT_INITIATED ) );

        //
        // TODO: Update attribute on communications protocol.
        //

        // Loopback update into WISE to activate triggers...
        CHECK_WISE_RESULT_EX( CWISEDriver::InternalNotifyAttributeUpdate( timeStamp, hDatabase, hObject, hAttribute, value, quality ) );
    }
    catch( WISE_RESULT& wErr )
    {
        wResult = wErr;
    }
    return wResult;
}

// -----------------------------------------------------------------------------
// Name: CSWORDDriver::OnRun
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void CSWORDDriver::OnRun()
{
    try
    {
        // Wait 20 ms between iterations.
        while( !m_StopEvent->Wait( 20 ) )
        {
            if( sword_.get() )
                sword_->OnRun();
        }
    }
    catch( ... )
    {
        NotifyErrorMessage( L"Exception occurred in CSWORDDriver::OnRun()! Thread will terminate.", 
            MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_ERROR ) );
    }
}
