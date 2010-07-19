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

#include <qvbox.h>

class QWidgetStack;
class QuitPage;

// =============================================================================
/** @class  Page_ABC
    @brief  Page_ABC
*/
// Created: SBO 2008-02-21
// =============================================================================
class Page_ABC : public QVBox
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
    };

public:
    //! @name Constructors/Destructor
    //@{
             Page_ABC( QWidgetStack* pages, Page_ABC& previous, unsigned short flags );
    virtual ~Page_ABC();
    //@}

protected:
    void AddContent( QWidget* widget );
    void AddTitle( const QString& title );
    void EnableButton( unsigned short flags, bool enable );
    void SetButtonText( unsigned short flags, const QString& text );
    void Previous();

private:
    //! @name Copy/Assignment
    //@{
    Page_ABC( const Page_ABC& );            //!< Copy constructor
    Page_ABC& operator=( const Page_ABC& ); //!< Assignment operator
    //@}

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

    void OnBack();
    void OnQuit();
    //@}

private:
    //! @name Member data
    //@{
    QGridLayout* grid_;
    QWidgetStack* pages_;
    Page_ABC& previous_;
    static QuitPage* quitPage_;
    QButton* startButton_;
    QButton* joinButton_;
    QButton* editButton_;
    //@}
};

#endif // __Page_ABC_h_
