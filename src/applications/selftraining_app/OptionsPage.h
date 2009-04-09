// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OptionsPage_h_
#define __OptionsPage_h_

#include "ContentPage.h"

// =============================================================================
/** @class  OptionsPage
    @brief  OptionsPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class OptionsPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OptionsPage( QWidgetStack* pages, Page_ABC& previous );
    virtual ~OptionsPage();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnChangeLanguage( const QString& lang );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OptionsPage( const OptionsPage& );            //!< Copy constructor
    OptionsPage& operator=( const OptionsPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Commit();
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, std::string > T_Languages;
    //@}

private:
    //! @name Member data
    //@{
    T_Languages languages_;
    std::string selectedLanguage_;
    //@}
};

#endif // __OptionsPage_h_
