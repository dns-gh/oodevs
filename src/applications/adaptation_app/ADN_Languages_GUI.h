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
class ADN_Languages_Dialog;

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
    void SetMenu( QMenu* menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLanguageChanged( const QString& name = "" );
    void OnLanguagesEdited();
    void OnMasterChanged();
    void OnSwap();
    //@}

private:
    //! @name Helpers
    //@{
    void BuildMenu();
    void UpdateMenu();
    void UpdateCurrentAction( const QString& name );
    QString CreateMasterText() const;
    void ChangeLanguage( const std::string& language );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Languages_Data& data_;
    QMenu* menu_;
    std::unique_ptr< ADN_Languages_Dialog > dialog_;
    std::vector< QAction* > actions_;
    QAction* swapAction_;
    QAction* swapSeparator_;
    QSignalMapper* mapper_;
    QAction* currentAction_;
    QAction* masterAction_;
    //@}
};

#endif // __ADN_Languages_GUI_h_
