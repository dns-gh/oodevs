// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Ref_ABC_h_
#define __ADN_Ref_ABC_h_

#include "ADN_Checker.h"

class ADN_Connector_ABC;
class ADN_Connector_Vector_ABC;
class ADN_ConsistencyChecker;

// =============================================================================
/** @class  ADN_Ref_ABC
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_Ref_ABC : public QObject
{
    Q_OBJECT
    friend class ADN_Connector_ABC;
    friend class ADN_Connector_Vector_ABC;

public:
             ADN_Ref_ABC();
    virtual ~ADN_Ref_ABC();

    //! @name Connection
    //@{
    virtual void    Connect( ADN_Ref_ABC* pTarget, bool bConnect );

    virtual void    Connect( ADN_Ref_ABC* pTarget );
    virtual void Disconnect( ADN_Ref_ABC* pTarget );

    void BindExistenceTo( const ADN_Ref_ABC* pObj );
    //@}

    //! @name Reference counting
    //@{
    void AddRef();
    void RemoveRef();
    bool IsMultiRef();
    //@}

    //! @name Validity
    //@{
    ADN_ErrorStatus GetErrorStatus() const;
    void SetErrorStatus( ADN_ErrorStatus errorStatus, const QString& errorMsg = "" );
    //@}

public slots:
    //! @name Slots
    //@{
    void Invalidate( void* pObj, bool bDel = false );
    void CheckTypeValidity( ADN_Ref_ABC& type );
    virtual void CheckValidity();
    virtual void CheckValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );
    //@}

signals:
    //! @name Signals
    //@{
    void Invalidated( void* pObj, bool bDel );
    void SendErrorStatus( ADN_ErrorStatus status, const QString& errorMsg );
    //@}

public:
    //! @name Double dispatch connexion mechanism
    virtual void ConnectPrivate( ADN_Ref_ABC* pTarget );
    virtual void ConnectPrivate( ADN_Connector_ABC* pTarget );
    virtual void ConnectPrivate( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivate( ADN_Ref_ABC* pTarget );
    virtual void DisconnectPrivate( ADN_Connector_ABC* pTarget );
    virtual void DisconnectPrivate( ADN_Connector_Vector_ABC* pTarget );

    virtual void ConnectPrivateSub( ADN_Ref_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Ref_ABC* pTarget );
    //@}

    virtual void InvalidatePrivate( void* ptr, bool bDel = false );
    virtual void AutoCreatePrivate( void* ptr);

    //! @name Slot blocking (to avoid signal loops between connectors)
    //@{
    bool SlotsBlocked() const;
    void BlockSlots( bool bBlock );
    //@}

protected slots:
    void AutoCreate( void* pObj );

private slots:
    void AutoDelete();
    void AutoDeleteEx();

private:
    bool bConnecting_;
    bool bSlotsBlocked_;
    int  nRef_;
    ADN_ErrorStatus errorStatus_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::AddRef
// Created: APE 2005-02-25
// -----------------------------------------------------------------------------
inline
void ADN_Ref_ABC::AddRef()
{
    ++nRef_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::RemoveRef
// Created: APE 2005-02-25
// -----------------------------------------------------------------------------
inline
void ADN_Ref_ABC::RemoveRef()
{
    assert( nRef_ > 0 );
    --nRef_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::IsMultiRef
// Created: JDY 03-08-26
//-----------------------------------------------------------------------------
inline
bool ADN_Ref_ABC::IsMultiRef()
{
    return nRef_ > 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::SlotsBlocked
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
inline
bool ADN_Ref_ABC::SlotsBlocked() const
{
    return bSlotsBlocked_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::BlockSlots
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
inline
void ADN_Ref_ABC::BlockSlots( bool bBlock )
{
    bSlotsBlocked_ = bBlock;
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::GetErrorStatus
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
inline
ADN_ErrorStatus ADN_Ref_ABC::GetErrorStatus() const
{
    return errorStatus_;
}

#endif // __ADN_Ref_ABC_h_
