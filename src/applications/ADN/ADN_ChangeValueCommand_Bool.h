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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ChangeValueCommand_Bool.h $
// $Author: Ape $
// $Modtime: 28/01/05 14:27 $
// $Revision: 2 $
// $Workfile: ADN_ChangeValueCommand_Bool.h $
//
// *****************************************************************************

#ifndef __ADN_ChangeValueCommand_Bool_h_
#define __ADN_ChangeValueCommand_Bool_h_

#include "qtundo.h"
#include "ADN_Types.h"

// =============================================================================
/** @class  ADN_ChangeValueCommand_Bool
    @brief  ADN_ChangeValueCommand_Bool
    @par    Using example
    @code
    ADN_ChangeValueCommand_Bool;
    @endcode
*/
// Created: AGN 2004-05-13
// =============================================================================
class ADN_ChangeValueCommand_Bool
: public QtCommand
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ChangeValueCommand_Bool( ADN_Type_Bool& modifiedData, bool nOldValue, bool nNewValue );
    virtual ~ADN_ChangeValueCommand_Bool();
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
    ADN_ChangeValueCommand_Bool( const ADN_ChangeValueCommand_Bool& );            //!< Copy constructor
    ADN_ChangeValueCommand_Bool& operator=( const ADN_ChangeValueCommand_Bool& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    bool oldValue_;
    bool newValue_;
    ADN_Type_Bool& modifiedData_;
    //@}
};


#endif // __ADN_ChangeValueCommand_Bool_h_
