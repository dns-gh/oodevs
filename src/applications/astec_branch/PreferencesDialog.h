// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PreferencesDialog_h_
#define __PreferencesDialog_h_

#include <qdialog.h>

class Controllers;
class GraphicPreferences;
class GraphicsPanel;

// =============================================================================
/** @class  PreferencesDialog
    @brief  PreferencesDialog
*/
// Created: SBO 2006-05-03
// =============================================================================
class PreferencesDialog : public QDialog
{

public:
    //! @name Constructors/Destructor
    //@{
             PreferencesDialog( QWidget* parent, Controllers& controllers );
    virtual ~PreferencesDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
    GraphicPreferences& GetPreferences() const; // $$$$ SBO 2006-05-04: 
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PreferencesDialog( const PreferencesDialog& );            //!< Copy constructor
    PreferencesDialog& operator=( const PreferencesDialog& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    GraphicsPanel* pGraphicPrefPanel_;
    //@}
};

#endif // __PreferencesDialog_h_
