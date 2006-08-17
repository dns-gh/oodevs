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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ChangeValueCommand_Double.h $
// $Author: Ape $
// $Modtime: 28/01/05 14:27 $
// $Revision: 2 $
// $Workfile: ADN_ChangeValueCommand_Double.h $
//
// *****************************************************************************

#ifndef __ADN_ChangeValueCommand_Double_h_
#define __ADN_ChangeValueCommand_Double_h_

#include "qtundo.h"
#include "ADN_Types.h"

// =============================================================================
/** @class  ADN_ChangeValueCommand_Double
    @brief  ADN_ChangeValueCommand_Double
    @par    Using example
    @code
    ADN_ChangeValueCommand_Double;
    @endcode
*/
// Created: AGN 2004-05-13
// =============================================================================
class ADN_ChangeValueCommand_Double
: public QtCommand
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ChangeValueCommand_Double( ADN_Type_Double& modifiedData, double nOldValue, double nNewValue );
    virtual ~ADN_ChangeValueCommand_Double();
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
    ADN_ChangeValueCommand_Double( const ADN_ChangeValueCommand_Double& );            //!< Copy constructor
    ADN_ChangeValueCommand_Double& operator=( const ADN_ChangeValueCommand_Double& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    double oldValue_;
    double newValue_;
    ADN_Type_Double& modifiedData_;
    //@}
};


#endif // __ADN_ChangeValueCommand_Double_h_
