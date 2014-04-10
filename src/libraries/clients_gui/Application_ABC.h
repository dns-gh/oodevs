// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __Application_ABC_h_
#define __Application_ABC_h_

#include "tools/LanguageObserver_ABC.h"

#pragma warning( push, 0 )
#include <QtCore/qobject.h>
#include <QtCore/qlocale.h>
#pragma warning( pop )

namespace tools
{
    class Languages;
}

namespace gui
{
    class ApplicationMonitor;

// =============================================================================
/** @class  Application_ABC
    @brief  Application_ABC
*/
// Created: HBD 2010-06-28
// =============================================================================
class Application_ABC : public QObject
                      , public tools::Observer_ABC
                      , public tools::LanguageObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit Application_ABC( ApplicationMonitor& monitor ) ;
    virtual ~Application_ABC();
    //@}

    //! @name Abstract operations
    //@{
    virtual int Run() = 0;
    virtual void CreateTranslators() = 0;
    //@}

protected:
    //! @name Protected operations
    virtual void CheckLicense( const std::string& licenseName );
    void LoadCommandLineLanguage( const tools::Languages& language, const std::string& languageCode );

    virtual void Initialize();
    void InitializeLayoutDirection();
    virtual void InitializeStyle();

    void AddTranslator( const char* t );
    void DeleteTranslators();

    void CheckInterfaceComponentNaming( QObject* root, const tools::Path& outpath ) const;
    //@}

    //! @name LanguageObserver_ABC implementation
    //@{
    virtual void OnLanguageChanged();
    //@}

    //! @name Accessors
    //@{
    bool IsInvalidLicense() const;
    const QString& GetExpiration() const;
    //@}

private slots:
    //! @name Helpers
    //@{
    void DisplayError( const QString& text ) const;
    //@}

protected:
    //! @name Member data
    //@{
    QApplication& app_;
    //@}

private:
    //! @name Member data
    //@{
    QString                     expiration_;
    bool                        invalidLicense_;
    QLocale                     locale_;
    std::vector< QTranslator* > translators_;
    //@}
};
} //! namespace gui

#endif // __Application_ABC_h_
