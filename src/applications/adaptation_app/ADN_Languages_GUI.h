// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Languages_GUI_h_
#define __ADN_Languages_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Languages_Data;

// =============================================================================
/** @class  ADN_Languages_GUI
    @brief  ADN_Languages_GUI
*/
// Created: ABR 2013-07-08
// =============================================================================
class ADN_Languages_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Languages_GUI( ADN_Languages_Data& data );
    virtual ~ADN_Languages_GUI();
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    void FillMenu( QMenu* menu );
    const std::string& GetCurrentLanguage() const;
    //@}

signals:
    //! @name Signals
    //@{
    void LanguageChanged( const std::string& shortName );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLanguageChanged( const QString& name );
    void OnEditLanguages();
    //@}

private:
    //! @name Helpers
    //@{
    void ChangeLanguage( const std::string& language );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Languages_Data& data_;
    std::vector< QAction* > actions_;
    QSignalMapper* mapper_;
    std::string currentLanguage_;
    QAction* currentAction_;
    QAction* defaultAction_;
    //@}
};

#endif // __ADN_Languages_GUI_h_
