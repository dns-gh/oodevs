// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Connector_Vector_ABC_h_
#define __ADN_Connector_Vector_ABC_h_

#include "ADN_Connector_ABC.h"

class ADN_BinaryPredicate
{
public:
    ADN_BinaryPredicate(){};
    virtual bool operator()( void* pL, void* pR ) const = 0;
};

class ADN_BinaryPredicateWrapper
{
public:
    explicit ADN_BinaryPredicateWrapper( ADN_BinaryPredicate& predicate ) : predicate_( predicate ) {};
    virtual bool operator()( void* pL, void* pR ) { return predicate_( pL, pR ); }

    const ADN_BinaryPredicateWrapper& operator=( const ADN_BinaryPredicateWrapper& );

    ADN_BinaryPredicate& predicate_;
};

//*****************************************************************************
// Created: JDY 03-07-02
//*****************************************************************************
class ADN_Connector_Vector_ABC : public ADN_Connector_ABC
{
    Q_OBJECT

public:
             ADN_Connector_Vector_ABC();
    virtual ~ADN_Connector_Vector_ABC();

    //! @name Connection
    //@{
    virtual void    Connect( ADN_Ref_ABC* pTarget, bool bConnect );

    virtual void    Connect( ADN_Ref_ABC* pTarget );
    virtual void Disconnect( ADN_Ref_ABC* pTarget );

    virtual void Initialize( ADN_Connector_Vector_ABC& dest ) const;
    //@}

public slots:
    void AddItem( void* pObj );
    void RemItem( void* pObj );

    void AddItemNoEmit( void* pObj );
    void AddItemsNoEmit( const std::vector< void* >& pObjs );
    void RemItemNoEmit( void* pObj );

    void Clear( bool bInConnection = false );

    virtual void Rename( void* ) {}

signals:
    void ItemAdded( void* pObj );
    void ItemRemoved( void* pObj );

public:
    //! @name Double dispatch connexion mechanism
    virtual void ConnectPrivate( ADN_Ref_ABC* pTarget );
    virtual void ConnectPrivate( ADN_Connector_ABC* pTarget );
    virtual void ConnectPrivate( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivate( ADN_Ref_ABC* pTarget );
    virtual void DisconnectPrivate( ADN_Connector_ABC* pTarget );
    virtual void DisconnectPrivate( ADN_Connector_Vector_ABC* pTarget );

    virtual void ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    //@}

    virtual void SetDataPrivate( void* pData );
    virtual bool AddItemPrivate( void* pObj );
    virtual bool RemItemPrivate( void* pObj);
    virtual void ClearPrivate( bool bInConnection = false );

protected:
    bool bConnecting_;
};

#endif // __ADN_Connector_Vector_ABC_h_