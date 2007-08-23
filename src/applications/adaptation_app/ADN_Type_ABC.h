//*****************************************************************************
//
// $Created: JDY 03-06-23 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Type_ABC.h $
// $Author: Ape $
// $Modtime: 22/04/05 15:36 $
// $Revision: 6 $
// $Workfile: ADN_Type_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_Type_ABC_h_
#define __ADN_Type_ABC_h_

#include "ADN_Connector_ABC.h"
#include "ADN_DataTreeNode_ABC.h"
#include <iostream>

//*****************************************************************************
// Created: JDY 03-07-02
//*****************************************************************************
template <class T > 
class ADN_Type_ABC
: public ADN_Connector_ABC
, public ADN_DataTreeNode_ABC
{
    MT_COPYNOTALLOWED( ADN_Type_ABC )

public:
    typedef typename T BaseType;

    //-------------------------------------------------------------------------
    /** @name Constructor-Destructor*/
    //-------------------------------------------------------------------------
    //@{
    ADN_Type_ABC();
    ADN_Type_ABC(const T& val);
    virtual ~ADN_Type_ABC();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    T    GetData();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    void SetDataName( const std::string& strName );
    virtual std::string GetNodeName();
    void SetUndoAvailable( bool b );

    virtual void Initialize( ADN_Connector_ABC& dest ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Operators*/
    //-------------------------------------------------------------------------
    //@{
    ADN_Type_ABC& operator =(const T& val);
    bool          operator ==(const T& val) const;
    bool          operator !=(const T& val) const;
    //@}

public:
    //-----------------------------------------------------------------------------
    // Compare operator
    //-----------------------------------------------------------------------------
    class Cmp : public std::unary_function< ADN_Type_ABC* , bool >
    {
    public:
        Cmp(const T& val) : val_(val) {}
        ~Cmp() {}

        bool operator()( ADN_Type_ABC* adn_type ) const { return adn_type->GetData()==val_; }

    private:
        T  val_;
    };

protected:
    void SetData(const T& data, bool bCanBeUndone );
    void SetDataPrivate(void *data);

private:
    bool bUndoAvailable_;
    std::string strDataName_;
    T val_;
};

template< typename T >
std::ostream& operator<<( std::ostream& os, ADN_Type_ABC<T>& value )
{
    os << value.GetData();
    return os;
}

#include "ADN_Type_ABC.inl"

#endif // __ADN_Type_ABC_h_
