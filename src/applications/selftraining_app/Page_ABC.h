// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Page_ABC_h_
#define __Page_ABC_h_

#include <boost/noncopyable.hpp>
#include "clients_gui/LanguageChangeObserver_ABC.h"

class QuitPage;

// =============================================================================
/** @class  Page_ABC
    @brief  Page_ABC
*/
// Created: SBO 2008-02-21
// =============================================================================
class Page_ABC : public gui::LanguageChangeObserver_ABC< Q3VBox >
               , private boost::noncopyable
{
    Q_OBJECT

protected:
    enum EButtonFlags
    {
        eButtonBack     = 0x01,
        eButtonQuit     = 0x02,
        eButtonOptions  = 0x04,
        eButtonStart    = 0x08,
        eButtonJoin     = 0x10,
        eButtonEdit     = 0x20,
        eButtonApply    = 0x40,
    };

public:
    //! @name Constructors/Destructor
    //@{
             Page_ABC( Q3WidgetStack* pages, Page_ABC& previous, unsigned short flags );
    virtual ~Page_ABC();
    //@}

public:
    //! @name Operation
    //@{
    void SetTitle( const QString& title );
    const QString GetTitle() const;
    //@}

protected:
    //! @name Protected Helpers
    //@{
    void AddContent( QWidget* widget );
    void EnableButton( unsigned short flags, bool enable );
    void SetButtonText( unsigned short flags, const QString& text );
    void Previous();
    virtual void OnLanguageChanged();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* event );
    virtual void Update() {}
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnOptions() {}
    virtual void OnStart() {}
    virtual void OnJoin() {}
    virtual void OnEdit() {}
    virtual void OnApply() {}
    void OnQuit();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnBack();
    //@}

private:
    //! @name Member data
    //@{
    Q3GridLayout* grid_;
    Q3WidgetStack* pages_;
    Page_ABC& previous_;
    static QuitPage* quitPage_;

    QPushButton* backButton_;
    QPushButton* settingsButton_;
    QPushButton* quitButton_;
    QPushButton* startButton_;
    QPushButton* joinButton_;
    QPushButton* editButton_;
    QPushButton* applyButton_;
    QLabel*      titleLabel_;
    //@}
};

#endif // __Page_ABC_h_
