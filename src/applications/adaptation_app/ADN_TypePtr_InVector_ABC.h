//*****************************************************************************
//
// $Created: JDY 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TypePtr_InVector_ABC.h $
// $Author: Ape $
// $Modtime: 3/03/05 14:40 $
// $Revision: 6 $
// $Workfile: ADN_TypePtr_InVector_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_TypePtr_InVector_ABC_h_
#define __ADN_TypePtr_InVector_ABC_h_

#include "ADN_Connector_Vector_ABC.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_DataTreeNode_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-18
//*****************************************************************************
template <class T>
class ADN_TypePtr_InVector_ABC
:   public ADN_Connector_Vector_ABC
,   public ADN_DataTreeNode_ABC
{
    MT_COPYNOTALLOWED( ADN_TypePtr_InVector_ABC )

public:
    typedef typename T*                      T_TypePtr;
    typedef typename ADN_Type_Vector_ABC<T>  T_TypeVector;
    
public:
    explicit ADN_TypePtr_InVector_ABC( const char* szName = 0 );
    explicit ADN_TypePtr_InVector_ABC( const T_TypePtr& value, const char* szName = 0 );
    explicit ADN_TypePtr_InVector_ABC( T_TypeVector& v, const T_TypePtr& value, const char* szName = 0 );
    virtual ~ADN_TypePtr_InVector_ABC();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize( ADN_Connector_Vector_ABC& dest ) const;
    virtual void Initialize( ADN_Connector_ABC& dest ) const;

    void          SetData(const T_TypePtr& value, bool bCanBeUndone );
    T_TypePtr     GetData();

    void          SetVector(const T_TypeVector& v);
    T_TypeVector& GetVector();

    virtual std::string GetNodeName();
    void SetNodeName( const char* szName );  // $$$$ AGN 2004-05-14: const char* car si on utilise
                                             // un const std::string& on a une FATAL INTERNAL COMPILER ERROR.
    //@}

    //-------------------------------------------------------------------------
    /** @name Operators*/
    //-------------------------------------------------------------------------
    //@{
//    ADN_TypePtr_InVector_ABC& operator =(const ADN_TypePtr_InVector_ABC& o);
    ADN_TypePtr_InVector_ABC& operator =(const T_TypePtr& val);
    bool          operator ==(const ADN_TypePtr_InVector_ABC& val) const;
    bool          operator ==(const T_TypePtr& val) const;
    //@}

protected:
    virtual void ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );

    virtual void  SetDataPrivate(void *data);
    virtual void  InvalidatePrivate( void *ptr ,bool bDel=false);
    virtual bool  AddItemPrivate(void *item,bool bCreateCommand = false );
    virtual bool  RemItemPrivate(void *item, bool bCreateCommand = false );
        
private:
    std::string     strDataName_;
    T_TypePtr       pData_;
    T_TypeVector*   pVector_;
};


#include "ADN_TypePtr_InVector_ABC.inl"

#endif // __ADN_TypePtr_InVector_ABC_h_