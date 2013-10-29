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

#pragma warning( push, 0 )
#include <QtCore/qobject.h>
#include <QtCore/qlocale.h>
#pragma warning( pop )

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

    virtual void Initialize();
    void InitializeBugTrap();
    void InitializeLayoutDirection();
    virtual void InitializeStyle();

    void AddTranslator( const char* t );
    void DeleteTranslators();

    void CheckInterfaceComponentNaming( QObject* root, const tools::Path& outpath ) const;
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
