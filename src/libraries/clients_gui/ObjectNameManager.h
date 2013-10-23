// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __NamingManager_h_
#define __NamingManager_h_

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  NamingManager
    @brief  NamingManager
*/
// Created: NPT 2013-03-11
// =============================================================================
class ObjectNameManager : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectNameManager();
    virtual ~ObjectNameManager();
    //@}

    //! @name Operations
    //@{
    static ObjectNameManager* getInstance();
    void AddSubLevel( const QString& name );
    QString GetSubName();
    void RemoveSubLevel();
    void SetObjectName( QObject* obj, const QString& name );
    void RemoveRegisteredName( const QString& name );
    void CheckErrors();
    void Purge();
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QStringList objectNamePath_;
    QStringList paths_;
    //@}
};

}

#endif // __NamingManager_h_
