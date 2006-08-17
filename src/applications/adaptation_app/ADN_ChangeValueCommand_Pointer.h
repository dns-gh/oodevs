// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-13 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ChangeValueCommand_Pointer.h $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_ChangeValueCommand_Pointer.h $
//
// *****************************************************************************

#ifndef __ADN_ChangeValueCommand_Pointer_h_
#define __ADN_ChangeValueCommand_Pointer_h_

#include "qtundo.h"
#include "ADN_Type_ABC.h"
#include "ADN_TypePtr_InVector_ABC.h"

// =============================================================================
/** @class  ADN_ChangeValueCommand_Pointer
    @brief  ADN_ChangeValueCommand_Pointer
    @par    Using example
    @code
    ADN_ChangeValueCommand_Pointer;
    @endcode
*/
// Created: AGN 2004-05-13
// =============================================================================
template< class PointedClass >
class ADN_ChangeValueCommand_Pointer
: public QtCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ChangeValueCommand_Pointer( ADN_TypePtr_InVector_ABC< PointedClass >& modifiedData, PointedClass* pOldValue, PointedClass* pNewValue );
    virtual ~ADN_ChangeValueCommand_Pointer();
    //@}

    //! @name Operations
    //@{
    virtual void undo();
    //@}


private:
    //! @name Copy/Assignement
    //@{
    ADN_ChangeValueCommand_Pointer( const ADN_ChangeValueCommand_Pointer& );            //!< Copy constructor
    ADN_ChangeValueCommand_Pointer& operator=( const ADN_ChangeValueCommand_Pointer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    PointedClass* pOldValue_;
    PointedClass* pNewValue_;
    ADN_TypePtr_InVector_ABC< PointedClass >& modifiedData_;
    //@}
};

// specialization for ADN_Type_ABC< std::string >
template<>
class ADN_ChangeValueCommand_Pointer< ADN_Type_ABC< std::string > >
: public QtCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ChangeValueCommand_Pointer( ADN_TypePtr_InVector_ABC< ADN_Type_ABC< std::string > >& modifiedData, ADN_Type_ABC< std::string >* pOldValue, ADN_Type_ABC< std::string >* pNewValue );
    virtual ~ADN_ChangeValueCommand_Pointer();
    //@}

    //! @name Operations
    //@{
    virtual void undo();
    //@}


private:
    //! @name Copy/Assignement
    //@{
    ADN_ChangeValueCommand_Pointer( const ADN_ChangeValueCommand_Pointer& );            //!< Copy constructor
    ADN_ChangeValueCommand_Pointer& operator=( const ADN_ChangeValueCommand_Pointer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Type_ABC< std::string >* pOldValue_;
    ADN_Type_ABC< std::string >* pNewValue_;
    ADN_TypePtr_InVector_ABC< ADN_Type_ABC< std::string > >& modifiedData_;
    //@}
};

#include "ADN_ChangeValueCommand_Pointer.inl"

#endif // __ADN_ChangeValueCommand_Pointer_h_
