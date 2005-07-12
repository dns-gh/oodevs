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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ChangeValueCommand_Int.h $
// $Author: Ape $
// $Modtime: 28/01/05 14:27 $
// $Revision: 2 $
// $Workfile: ADN_ChangeValueCommand_Int.h $
//
// *****************************************************************************

#ifndef __ADN_ChangeValueCommand_Int_h_
#define __ADN_ChangeValueCommand_Int_h_

#include "qtundo.h"
#include "ADN_Types.h"

// =============================================================================
/** @class  ADN_ChangeValueCommand_Int
    @brief  ADN_ChangeValueCommand_Int
    @par    Using example
    @code
    ADN_ChangeValueCommand_Int;
    @endcode
*/
// Created: AGN 2004-05-13
// =============================================================================
class ADN_ChangeValueCommand_Int
: public QtCommand
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ChangeValueCommand_Int( ADN_Type_Int& modifiedData, int nOldValue, int nNewValue );
    virtual ~ADN_ChangeValueCommand_Int();
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
    ADN_ChangeValueCommand_Int( const ADN_ChangeValueCommand_Int& );            //!< Copy constructor
    ADN_ChangeValueCommand_Int& operator=( const ADN_ChangeValueCommand_Int& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    int oldValue_;
    int newValue_;
    ADN_Type_Int& modifiedData_;
    //@}
};


#endif // __ADN_ChangeValueCommand_Int_h_
