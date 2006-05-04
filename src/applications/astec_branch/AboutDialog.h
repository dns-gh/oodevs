// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AboutDialog_h_
#define __AboutDialog_h_

#include <qdialog.h>

// =============================================================================
/** @class  AboutDialog
    @brief  AboutDialog
*/
// Created: SBO 2006-05-04
// =============================================================================
class AboutDialog : public QDialog
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AboutDialog( QWidget* parent );
    virtual ~AboutDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AboutDialog( const AboutDialog& );            //!< Copy constructor
    AboutDialog& operator=( const AboutDialog& ); //!< Assignement operator
    //@}

private:
    //! @name Member Data
    //@{
    QSize sizeHint_;
    //@}
};

#endif // __AboutDialog_h_
