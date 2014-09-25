// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Settings_h_
#define __Settings_h_

#include "Settings_ABC.h"
#pragma warning( push, 0 )
#pragma warning( disable : 4127 )
#include <QtCore/qsettings.h>
#pragma warning( pop )

namespace kernel
{
    class TristateOption;
    class FourStateOption;

// =============================================================================
// Created: APE 2004-06-01
// =============================================================================
class Settings : public QSettings
               , public kernel::Settings_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Settings( const QString & organization, const QString & application );
    virtual ~Settings();
    //@}

    //! @name Helpers
    //@{
    virtual void BeginGroup( const QString& prefix );
    virtual void EndGroup();
    virtual void RemoveKey( const QString& key );

    virtual QStringList EntryList( const char* path );
    virtual QStringList SubEntriesList( const char* path );

    virtual void Save( const std::string& name, int value );
    virtual void Save( const std::string& name, bool value );
    virtual void Save( const std::string& name, float value );
    virtual void Save( const std::string& name, const kernel::TristateOption& value );
    virtual void Save( const std::string& name, const kernel::FourStateOption& value );
    virtual void Save( const std::string& name, const QString& value );

    virtual bool Remove( const std::string& name );

    virtual int Load( const std::string& name, int defaultValue );
    virtual bool Load( const std::string& name, bool defaultValue );
    virtual float Load( const std::string& name, float defaultValue );
    virtual kernel::TristateOption Load( const std::string& name, const kernel::TristateOption& defaultValue );
    virtual kernel::FourStateOption Load( const std::string& name, const kernel::FourStateOption& defaultValue );

    virtual QString Load( const std::string& name, const QString& defaultValue );

    template< typename T >
    void Save( const std::string&, T ) {};
    template< typename T >
    T Load( const std::string&, T defaultValue ) { return defaultValue; };
    //@}

    //! @name Operations
    //@{
    void WriteMainWindowEntries( const QString& field, QWidget& widget );
    void RemoveMainWindowEntries( const QString& field );
    void ReadEntry( const QString& field, QWidget& widget, int nW, int nH, int nX, int nY, bool bVisible );
    //@}
};

} //! namespace kernel

#endif // __Settings_h_
