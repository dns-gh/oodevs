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

class FlexLmLicense;

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  AboutDialog
    @brief  AboutDialog
*/
// Created: SBO 2006-05-04
// =============================================================================
class AboutDialog : public QDialog
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             AboutDialog( QWidget* parent, gui::ItemFactory_ABC& factory, const QString& line, const FlexLmLicense* license );
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

}

#endif // __AboutDialog_h_
