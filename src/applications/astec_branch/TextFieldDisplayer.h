// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TextFieldDisplayer_h_
#define __TextFieldDisplayer_h_

#include "Displayer_ABC.h"

// =============================================================================
/** @class  TextFieldDisplayer
    @brief  TextField Displayer
*/
// Created: SBO 2006-04-18
// =============================================================================
class TextFieldDisplayer : public Displayer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TextFieldDisplayer( QWidget* parent, const char* name );
    virtual ~TextFieldDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TextFieldDisplayer( const TextFieldDisplayer& );            //!< Copy constructor
    TextFieldDisplayer& operator=( const TextFieldDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit* edit_;
    QString message_;
    //@}
};

#endif // __TextFieldDisplayer_h_
