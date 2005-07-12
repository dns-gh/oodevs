//*****************************************************************************
//
// $Created: JDY 03-07-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Vector_ABC.h $
// $Author: Ape $
// $Modtime: 3/03/05 14:35 $
// $Revision: 7 $
// $Workfile: ADN_Connector_Vector_ABC.h $
//
//*****************************************************************************

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
    ADN_BinaryPredicateWrapper( ADN_BinaryPredicate& predicate ) : predicate_( predicate ) {};
    virtual bool operator()( void* pL, void* pR ) { return predicate_( pL, pR ); }

    const ADN_BinaryPredicateWrapper& operator=( const ADN_BinaryPredicateWrapper& );

    ADN_BinaryPredicate& predicate_;
};



//*****************************************************************************
// Created: JDY 03-07-02
//*****************************************************************************
class ADN_Connector_Vector_ABC
:   public ADN_Connector_ABC
{
    Q_OBJECT

public:
    explicit ADN_Connector_Vector_ABC( const char* szName = 0 );
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
    void RemItemNoEmit( void* pObj );
    
    void SwapItem( int i, int j );
    void Sort( ADN_BinaryPredicateWrapper& lessComp );
    void Clear( bool bInConnection = false );
    
signals:
    void ItemAdded( void* pObj );
    void ItemRemoved( void* pObj );
    void ItemSwapped( int i, int j );
    void Sorted( ADN_BinaryPredicateWrapper& lessComp );
    void Cleared( bool bInConnection = false);

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
    virtual bool AddItemPrivate( void* pObj, bool bCreateCommand = false );
    virtual bool RemItemPrivate( void* pObj, bool bCreateCommand = false );
    virtual void SwapItemPrivate( int i, int j );
    virtual void SortPrivate( ADN_BinaryPredicateWrapper& lessComp );
    virtual void ClearPrivate( bool bInConnection = false );

protected:
    bool bConnecting_;
};


#endif // __ADN_Connector_Vector_ABC_h_