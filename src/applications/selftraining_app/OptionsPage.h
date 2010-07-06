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

namespace tools
{
    class GeneralConfig;
}

class QCheckBox;
class QLineEdit;
class QSpinBox;

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
    enum ERandomContexts
    {
        eFire,
        eWounds,
        ePerception,
        eBreakdowns,
        eContextsNbr
    };

public:
    //! @name Constructors/Destructor
    //@{
             OptionsPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config );
    virtual ~OptionsPage();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnChangeLanguage( const QString& lang );
    void OnChangeDataDirectory();
    void OnContextChanged( int index );
    void OnDistributionChanged( int index );
    void OnDeviationChanged( const QString& );
    void OnMeanChanged( const QString& );
    void OnSeedToggled();
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
    void ReadRandomValues();
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
    QLineEdit* dataDirectory_;
    QComboBox* contextList_;
    QComboBox* distributionList_;
    QLineEdit* deviation_;
    QLineEdit* mean_;
    QCheckBox* hasSeed_;
    QSpinBox* seed_;
    bool bDistributions_[ eContextsNbr ];
    double rDeviations_[ eContextsNbr ];
    double rMeans_[ eContextsNbr ];
    //@}
};

#endif // __OptionsPage_h_
