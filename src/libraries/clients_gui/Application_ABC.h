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
#include <QtGui/qapplication.h>
#include <QtCore/qlocale.h>
#pragma warning( pop )

namespace gui
{

// =============================================================================
/** @class  Application_ABC
    @brief  Application_ABC
*/
// Created: HBD 2010-06-28
// =============================================================================
class Application_ABC : public QApplication
{
public:
    //! @name Constructors/Destructor
    //@{
             Application_ABC( int& argc, char** argv ) ;
    virtual ~Application_ABC();
    //@}

    //! @name Abstract operations
    //@{
    virtual int Run() = 0;
    virtual void CreateTranslators() = 0;
    //@}

    //! @name Operations
    //@{
    virtual void DeleteTranslators();
    virtual void InitializeLayoutDirection();
    void SetLocale();
    //@}

protected:
    //! @name Protected operations
    virtual void CheckLicense( const std::string& licenseName );

    virtual void Initialize();
    virtual void InitializeBugTrap();
    virtual void InitializeStyle();

    void AddTranslator( const char* t );
    //@}

    //! @name Accessors
    //@{
    bool IsInvalidLicense() const;
    const QString& GetExpiration() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool notify( QObject* receiver, QEvent* e );
    void DisplayError( const QString& text ) const;
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
