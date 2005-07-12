//*****************************************************************************
//
// $Created: JDY 03-08-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Ref_ABC.h $
// $Author: Ape $
// $Modtime: 3/03/05 14:23 $
// $Revision: 5 $
// $Workfile: ADN_Ref_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_Ref_ABC_h_
#define __ADN_Ref_ABC_h_

#include <qobject.h>

class ADN_Connector_ABC;
class ADN_Connector_Vector_ABC;


// =============================================================================
/** @class  ADN_Ref_ABC
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_Ref_ABC
: public QObject
{
    Q_OBJECT
    friend class ADN_Connector_ABC;
    friend class ADN_Connector_Vector_ABC;

public:
    explicit ADN_Ref_ABC( const char* szName = 0 );
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

public slots:
    void Invalidate( void* pObj, bool bDel = false );

signals:
    void Invalidated( void* pObj, bool bDel );

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

#endif // __ADN_Ref_ABC_h_