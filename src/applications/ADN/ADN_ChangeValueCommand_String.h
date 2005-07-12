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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ChangeValueCommand_String.h $
// $Author: Ape $
// $Modtime: 28/01/05 14:27 $
// $Revision: 2 $
// $Workfile: ADN_ChangeValueCommand_String.h $
//
// *****************************************************************************

#ifndef __ADN_ChangeValueCommand_String_h_
#define __ADN_ChangeValueCommand_String_h_

#include "qtundo.h"
#include "ADN_Types.h"

// =============================================================================
/** @class  ADN_ChangeValueCommand_String
    @brief  ADN_ChangeValueCommand_String
    @par    Using example
    @code
    ADN_ChangeValueCommand_String;
    @endcode
*/
// Created: AGN 2004-05-13
// =============================================================================
class ADN_ChangeValueCommand_String
: public QtCommand
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ChangeValueCommand_String( ADN_Type_String& modifiedData, const std::string& nOldValue, const std::string& nNewValue );
    virtual ~ADN_ChangeValueCommand_String();
    //@}

    //! @name Operations
    //@{
    virtual void undo();
    //@}

protected:
    bool mergeMeWith ( QtCommand* other );

private:
    //! @name Copy/Assignement
    //@{
    ADN_ChangeValueCommand_String( const ADN_ChangeValueCommand_String& );            //!< Copy constructor
    ADN_ChangeValueCommand_String& operator=( const ADN_ChangeValueCommand_String& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    std::string oldValue_;
    std::string newValue_;
    ADN_Type_String& modifiedData_;
    //@}
};


#endif // __ADN_ChangeValueCommand_String_h_
