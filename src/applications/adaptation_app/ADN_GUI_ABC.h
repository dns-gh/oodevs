// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GUI_ABC.h $
// $Author: Ape $
// $Modtime: 19/04/05 16:12 $
// $Revision: 7 $
// $Workfile: ADN_GUI_ABC.h $
//
// *****************************************************************************

#ifndef __ADN_GUI_ABC_h_
#define __ADN_GUI_ABC_h_

#include <boost/noncopyable.hpp>

class ADN_MainWindow;
class ADN_HtmlBuilder;
struct ADN_UsedByInfos;

class ADN_BaseGui_ABC : public QObject
                      , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_BaseGui_ABC() {}
    virtual ~ADN_BaseGui_ABC() {}
    //@}

signals:
    //! @name Signals
    //@{
    void ApplyFilterList( const ADN_UsedByInfos& );
    //@}
};

// =============================================================================
/** @class  ADN_GUI_ABC
    @brief  ADN_GUI_ABC
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_GUI_ABC : public ADN_BaseGui_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_GUI_ABC( const char* szClassName ) : pMainWidget_ ( 0 ), strClassName_( szClassName ) {};
    virtual ~ADN_GUI_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Build() = 0;
    virtual void RegisterTable( ADN_MainWindow& /*mainWindow*/ ) {}
    virtual void ExportHtml( ADN_HtmlBuilder& /*mainIndexBuilder*/, const QString& /*strPath*/ ) {}
    virtual void ChangeCurrentSubTab( int /*subTab*/ ) {}

    void ApplyFilter( const ADN_UsedByInfos& usedByInfos );

    QWidget* GetMainWidget() const { return pMainWidget_; }
    QWidget* CreateScrollArea( QWidget& content, QWidget* list = 0, bool paintSplitter = true, bool paintBackground = false, bool showFrameBorder = true, int margin = 10, int spacing = 10 );
    //@}

    //! @name Qt reimplementation.
    //@{
    QString tr( const char* s, const char* c = 0 );
    //@}

protected:
    //! @name Member data
    //@{
    QWidget* pMainWidget_;
    QString  strClassName_;
    //@}
};


#endif // __ADN_GUI_ABC_h_
