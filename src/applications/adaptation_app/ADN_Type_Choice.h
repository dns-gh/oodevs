// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Type_Choice_h_
#define __ADN_Type_Choice_h_

#include "ADN_Connector_Vector_ABC.h"
#include "ADN_DataTreeNode_ABC.h"

// =============================================================================
/** @class  ADN_Type_Choice
    @brief  ADN_Type_Choice
*/
// Created: JCR 2008-08-20
// =============================================================================
template< typename T >
class ADN_Type_Choice
    :   public ADN_Connector_Vector_ABC
    ,   public ADN_DataTreeNode_ABC
{
public:
    typedef std::vector< T >                         T_ChoicesVector;
    typedef typename T_ChoicesVector::const_iterator CIT_ChoicesVector;

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Type_Choice( const T_ChoicesVector& choices );
    virtual ~ADN_Type_Choice();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
            T&   GetData();
    virtual void Initialize( ADN_Connector_Vector_ABC& dest ) const;
//    virtual std::string GetNodeName();
    //@}

    //-------------------------------------------------------------------------
    /** @name Operators*/
    //-------------------------------------------------------------------------
    //@{
    ADN_Type_Choice& operator =(const T& val);
    ADN_Type_Choice& operator =(const ADN_Type_Choice& e);
    bool          operator ==(const ADN_Type_Choice& o) const;
    bool          operator ==(const T& val) const;
    bool          operator !=(const ADN_Type_Choice& o) const;
    bool          operator !=(const T& val) const;
    //@}

    //! @name 
    //@{
    void SetDataName( const std::string& strName );
    //@}

protected:
    virtual void SetDataPrivate( void *data );
            void SetDataPrivate( const T& value, bool bCanBeUndone );

    
private:
    //! @name Copy/Assignment
    //@{
    ADN_Type_Choice( const ADN_Type_Choice& );            //!< Copy constructor    
    //@}

    //! @name 
    //@{
    bool CheckData( const T& data ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string strDataName_;
    T           data_;
    //@}

    //! @name 
    //@{
    T_ChoicesVector choices_;
    //@}
};

#include "ADN_Type_Choice.inl"

#endif // __ADN_Type_Choice_h_
