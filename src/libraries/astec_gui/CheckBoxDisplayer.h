// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CheckBoxDisplayer_h_
#define __CheckBoxDisplayer_h_

#include "astec_kernel/Displayer_ABC.h"

// =============================================================================
/** @class  CheckBoxDisplayer
    @brief  CheckBoxDisplayer
*/
// Created: AGE 2006-02-23
// =============================================================================
class CheckBoxDisplayer : public Displayer_ABC
                        , public Caller< bool >
{

public:
    //! @name Constructors/Destructor
    //@{
             CheckBoxDisplayer( QWidget* parent, const char* name );
    virtual ~CheckBoxDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CheckBoxDisplayer( const CheckBoxDisplayer& );            //!< Copy constructor
    CheckBoxDisplayer& operator=( const CheckBoxDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    virtual void Call( const bool& value );
    //@}

private:
    //! @name Member data
    //@{
    QCheckBox* box_;
    //@}
};

#endif // __CheckBoxDisplayer_h_
