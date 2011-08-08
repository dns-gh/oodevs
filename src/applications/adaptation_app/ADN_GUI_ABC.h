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

// =============================================================================
/** @class  ADN_GUI_ABC
    @brief  ADN_GUI_ABC
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_GUI_ABC : public QObject
                  , private boost::noncopyable
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

    QWidget* GetMainWidget() const { return pMainWidget_; }
    //@}

    //! @name Qt reimplementation.
    //@{
    QString tr( const char* s, const char* c = 0 );
    //@}

protected:
    QWidget* pMainWidget_;
    QString strClassName_;
};


#endif // __ADN_GUI_ABC_h_
