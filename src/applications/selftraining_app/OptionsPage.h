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

class Config;

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
             OptionsPage( QWidget* parent, Q3WidgetStack* pages, Page_ABC& previous, Config& config );
    virtual ~OptionsPage();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnChangeLanguage( const QString& lang );
    void OnChangeDataDirectory();
    void OnEditDataDirectory();
    virtual void OnApply();
    virtual void OnBack();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    void Reset();
    void Commit();
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, QString > T_Languages;
    //@}

private:
    //! @name Member data
    //@{
    QWidget* parent_;
    Config& config_;
    T_Languages languages_;

    std::string selectedLanguage_;
    std::string selectedDataDir_;

    QLabel*      languageLabel_;
    QComboBox*   languageCombo_;

    QLabel*      dataLabel_;
    QPushButton* dataButton_;

    QLineEdit*   dataDirectory_;

    bool hasChanged_;
    bool languageHasChanged_;
    //@}
};

#endif // __OptionsPage_h_
