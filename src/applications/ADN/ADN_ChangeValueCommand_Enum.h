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
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ChangeValueCommand_Enum.h $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_ChangeValueCommand_Enum.h $
//
// *****************************************************************************

#ifndef __ADN_ChangeValueCommand_Enum_h_
#define __ADN_ChangeValueCommand_Enum_h_

#include "qtundo.h"
#include "ADN_Type_Enum.h"

// =============================================================================
/** @class  ADN_ChangeValueCommand_Enum
    @brief  ADN_ChangeValueCommand_Enum
    @par    Using example
    @code
    ADN_ChangeValueCommand_Enum;
    @endcode
*/
// Created: AGN 2004-05-13
// =============================================================================
template< class EnumType, int nb >
class ADN_ChangeValueCommand_Enum
: public QtCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ChangeValueCommand_Enum( ADN_Type_Enum< EnumType, nb >& modifiedData, EnumType nOldValue, EnumType nNewValue );
    virtual ~ADN_ChangeValueCommand_Enum();
    //@}

    //! @name Operations
    //@{
    virtual void undo();    
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}

    //! @name Operators
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_ChangeValueCommand_Enum( const ADN_ChangeValueCommand_Enum& );            //!< Copy constructor
    ADN_ChangeValueCommand_Enum& operator=( const ADN_ChangeValueCommand_Enum& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    EnumType oldValue_;
    EnumType newValue_;
    ADN_Type_Enum< EnumType, nb >& modifiedData_;
    //@}
};

#include "ADN_ChangeValueCommand_Enum.inl"

#endif // __ADN_ChangeValueCommand_Enum_h_
