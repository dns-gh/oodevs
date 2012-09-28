// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __DataPage_h_
#define __DataPage_h_

#include "ContentPage.h"

class Config;

// =============================================================================
/** @class  DataPage
    @brief  Data page
*/
// Created: LGY 2012-02-28
// =============================================================================
class DataPage : public ContentPage
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DataPage( QWidget* parent, Q3WidgetStack* pages, Page_ABC& previous, const Config& config );
    virtual ~DataPage();
    //@}

    //! @name Operations
    //@{
    void SetTerrainsEnabled( bool value );
    void SetModelsEnabled( bool value );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    virtual void OnDelete();
    virtual void Update();
    QListWidgetItem* CurrentItem() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void UpdateButton();
    //@}

private:
    //! @name Types
    //@{
    enum E_Tabs{ eTabs_Terrains = 0, eTabs_Models = 1 };
    //@}

private:
    //! @name Member data
    //@{
    QWidget* parent_;
    const Config& config_;
    QTabWidget* mainTabs_;
    QListWidget* terrains_;
    QListWidget* models_;
    //@}
};

#endif // __DataPage_h_
