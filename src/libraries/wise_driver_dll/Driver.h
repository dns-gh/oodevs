#pragma once

////////////////////////////////////////////////
// Includes
//
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/WISEDriver.h>
#pragma warning( pop )

class SwordFacade;

//! \brief  Implementation of WISE driver for SWORD.
class Driver :
    public CWISEDriver

{
public:
    //! \brief  Class constructor.
    //!         Initializes the object on construction.
    Driver();

    //! \brief  Class destructor.
    //!         Performs clean-up on object destruction.
    virtual ~Driver(void);

protected:

    //
    // CWISEDriver overrides
    //

    virtual WISE_RESULT OnInitialize( IWISEDriverSettings* pSettings );
    virtual WISE_RESULT OnUninitialize();
    virtual WISE_RESULT OnCreateDatabase(
                            const std::wstring& strDatabaseName,
                            const WISE_HANDLE& hDatabase,
                            const WISE_HANDLE& hTemplateDatabase,
                            const WISEDatabaseDistType& eDatabaseDistType,
                            const WISEDatabaseType& eDatabaseType,
                            const WALOwnershipMode& eModeOwnership,
                            const WALAttributeQualityMode& eModeAttributeQuality,
                            const WALAttributeTimeMode& eModeAttributeTime );
    virtual WISE_RESULT OnCloseDatabase( const WISE_HANDLE& hDatabase );
    virtual WISE_RESULT OnAddObject(
                            const timeb& timeStamp,
                            const WISE_HANDLE& hDatabase,
                            const WISE_HANDLE& hObject,
                            const std::wstring& strObjectName,
                            const std::map<std::wstring, WISE_HANDLE >& associatedObjects,
                            const std::map<std::wstring, std::list<WISE_HANDLE> >& associatedObjectLists );
    virtual WISE_RESULT OnRemoveObject(
                            const timeb& timeStamp,
                            const WISE_HANDLE& hDatabase,
                            const WISE_HANDLE& hObject );
    virtual WISE_RESULT OnSendEvent(
                            const timeb& timeStamp,
                            const WISE_HANDLE& hDatabase,
                            const WISE_HANDLE& hEvent,
                            const std::map<std::wstring, WISE_HANDLE >& associatedObjects,
                            const std::map<std::wstring, std::list<WISE_HANDLE> >& associatedObjectLists );
    virtual WISE_RESULT OnUpdateAttribute(
                            const timeb& timeStamp,
                            const WISE_HANDLE& hDatabase,
                            const WISE_HANDLE& hObject,
                            const WISE_HANDLE& hAttribute,
                            const CWISEValueUnion& value,
                            const WISE_ATTRIBUTE_QUALITY& quality );


    //
    // Threading overrides
    //
    virtual void OnRun();

private:
    //! @name Member data
    //@{
    std::auto_ptr< SwordFacade > sword_;
    //@}

};
