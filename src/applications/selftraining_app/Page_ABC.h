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
#include "clients_gui/WidgetLanguageObserver_ABC.h"

class QuitPage;

// =============================================================================
/** @class  Page_ABC
    @brief  Page_ABC
*/
// Created: SBO 2008-02-21
// =============================================================================
class Page_ABC : public gui::WidgetLanguageObserver_ABC< QWidget >
               , private boost::noncopyable
{
    Q_OBJECT

protected:
    enum EButtonFlags
    {
        eButtonBack     = 1 << 0,
        eButtonQuit     = 1 << 1,
        eButtonAdmin    = 1 << 2,
        eButtonStart    = 1 << 3,
        eButtonJoin     = 1 << 4,
        eButtonEdit     = 1 << 5,
        eButtonApply    = 1 << 6,
        eButtonDelete   = 1 << 7,
        eButtonUpgrade  = 1 << 8,
        eButtonCancel   = 1 << 9,
        eButtonExport   = 1 << 10,
    };

public:
    //! @name Constructors/Destructor
    //@{
             Page_ABC( QStackedWidget* pages, Page_ABC& previous, unsigned short flags );
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
    virtual void OnDelete() {}
    virtual void OnUpgrade() {}
    virtual void OnCancel() {}
    virtual void OnExport() {}
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
    QGridLayout* grid_;
    QStackedWidget* pages_;
    Page_ABC& previous_;
    static QuitPage* quitPage_;

    QPushButton* backButton_;
    QPushButton* adminButton_;
    QPushButton* quitButton_;
    QPushButton* startButton_;
    QPushButton* joinButton_;
    QPushButton* editButton_;
    QPushButton* applyButton_;
    QPushButton* deleteButton_;
    QPushButton* upgradeButton_;
    QPushButton* cancelButton_;
    QPushButton* exportButton_;
    QLabel*      titleLabel_;
    //@}
};

#endif // __Page_ABC_h_
