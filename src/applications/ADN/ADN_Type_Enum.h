//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Type_Enum.h $
// $Author: Ape $
// $Modtime: 17/02/05 15:46 $
// $Revision: 4 $
// $Workfile: ADN_Type_Enum.h $
//
//*****************************************************************************

#ifndef __ADN_Type_Enum_h_
#define __ADN_Type_Enum_h_

#include "ADN_Connector_Vector_ABC.h"
#include "ADN_DataTreeNode_ABC.h"

#include "ENT/ENT_Tr_ABC.h"


//*****************************************************************************
// Created: JDY 03-07-22
//*****************************************************************************
template <class T,int nb>
class ADN_Type_Enum
:   public ADN_Connector_Vector_ABC
,   public ADN_DataTreeNode_ABC
{
public:
    typedef typename const std::string& (*T_Converter)( T, ENT_Tr_ABC::E_Conversion );
    
    explicit ADN_Type_Enum();
    explicit ADN_Type_Enum( const T& value );
    virtual ~ADN_Type_Enum();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    T           GetData();
    virtual void Initialize( ADN_Connector_Vector_ABC& dest ) const;
    virtual std::string GetNodeName();
    //@}

    //-------------------------------------------------------------------------
    /** @name Operators*/
    //-------------------------------------------------------------------------
    //@{
    ADN_Type_Enum& operator =(const T& val);
    ADN_Type_Enum& operator =(const ADN_Type_Enum& e);
    bool          operator ==(const ADN_Type_Enum& o) const;
    bool          operator ==(const T& val) const;
    bool          operator !=(const ADN_Type_Enum& o) const;
    bool          operator !=(const T& val) const;
    //@}

    static std::string ConvertValue( T nVal, ENT_Tr_ABC::E_Conversion nConversion );
    static void SetConverter( typename ADN_Type_Enum::T_Converter enumConverter );

    void SetDataName( const std::string& strName );

protected:
    virtual void SetDataPrivate(void *data);
            void SetDataPrivate(const T& value, bool bCanBeUndone );

private:
    explicit ADN_Type_Enum(const ADN_Type_Enum& e);
    
private:
    
    std::string strDataName_;
    T           data_;

    static T_Converter converter_;
};

#include "ADN_Type_Enum.inl"

#endif // __ADN_Type_Enum_h_