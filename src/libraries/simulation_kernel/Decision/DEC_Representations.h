// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Representations_h_
#define __DEC_Representations_h_

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
/** @class  DEC_Representations
    @brief  DEC_Representations
*/
// Created: LDC 2009-04-03
// =============================================================================
class DEC_Representations : public MT_Role_ABC
{
public:
    typedef DEC_Representations RoleInterface;

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Representations();
    virtual ~DEC_Representations();
    //@}

    //! @name Operations
    //@{
    const T_ObjectVector& GetCategory( const std::string& name );
    void AddToCategory( const std::string& name, DIA_TypedObject* pObject );
    void RemoveFromCategory( const std::string& name, DIA_TypedObject* pObject );
    void DeleteRepresentation( DIA_TypedObject* pObject );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_Representations( const DEC_Representations& );            //!< Copy constructor
    DEC_Representations& operator=( const DEC_Representations& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, T_ObjectVector > T_Representations;
    typedef T_Representations::iterator           IT_Representations;
    //@}

private:
    //! @name Member data
    //@{
    T_Representations representations_;
    //@}
};

#endif // __DEC_Representations_h_
