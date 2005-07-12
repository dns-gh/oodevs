//*****************************************************************************
//
// $Created: JDY 03-06-26 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Type_Vector_ABC.h $
// $Author: Ape $
// $Modtime: 7/04/05 17:17 $
// $Revision: 7 $
// $Workfile: ADN_Type_Vector_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_Type_Vector_ABC_h_
#define __ADN_Type_Vector_ABC_h_

#include "ADN_Connector_Vector_ABC.h"
#include "ADN_DataTreeNode_ABC.h"

//*****************************************************************************
// Created: JDY 03-06-26
//*****************************************************************************
template < class T > 
class ADN_Type_Vector_ABC 
: public ADN_Connector_Vector_ABC
, public std::vector<T*>
, public ADN_DataTreeNode_ABC
{
    MT_COPYNOTALLOWED( ADN_Type_Vector_ABC )

public:
    typedef std::vector<T*>                T_PtrVector;
    typedef T_PtrVector::iterator          IT_PtrVector;
    typedef T_PtrVector::const_iterator    CIT_PtrVector;

public:
    ADN_Type_Vector_ABC( bool bAutoRef = true, const char* szName = 0 );
    virtual ~ADN_Type_Vector_ABC();

    virtual void Initialize( ADN_Connector_Vector_ABC& dest ) const;

    virtual void  EndVector();
            void  Reset();

    virtual std::string GetNodeName();
    void SetNodeName( const std::string& strNodeName );

    const std::string& GetItemTypeName() const;
    void SetItemTypeName( const std::string& strItemTypeName );

protected:
    virtual void SetDataPrivate( void* pData );
    virtual bool AddItemPrivate( void* pObj, bool bCreateCommand = false );
    virtual bool RemItemPrivate( void* pObj, bool bCreateCommand = false );
    virtual void SwapItemPrivate( int i, int j );
    virtual void SortPrivate( ADN_BinaryPredicateWrapper& lessComp );
    virtual void ClearPrivate( bool bInConnection = false );
    virtual void InvalidatePrivate( void* pObj, bool bDel = false );
    virtual void push_back( T* const & x );

protected:
    bool bAutoRef_;

    std::string strNodeName_;
    std::string strItemTypeName_;
};

#include "ADN_Type_Vector_ABC.inl"

#endif // __ADN_Type_Vector_ABC_h_