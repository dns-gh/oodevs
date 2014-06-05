// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileList_h_
#define __UserProfileList_h_

#include "ProfilesChecker_ABC.h"

namespace kernel
{
    class Controller;
    class UserProfile_ABC;
}

namespace gui
{
    template< typename T > class RichWidget;
}

class UserProfileWidget;
class ProfilesModel;
class NewProfileDialog;
class ProfileEditor;

// =============================================================================
/** @class  UserProfileList
    @brief  UserProfileList
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileList : public QWidget
                      , public ProfilesChecker_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileList( QWidget* parent, UserProfileWidget& pages, kernel::Controller& controller, ProfilesModel& model );
    virtual ~UserProfileList();
    //@}

    //! @name Operations
    //@{
    void Commit();
    //@}

    //! @name From ProfilesChecker_ABC
    //@{
    virtual bool Exists( const QString& oldLogin, const QString& newLogin ) const;
    virtual bool Exists( const QString& login ) const;
    virtual void NotifyNameChanged( const kernel::UserProfile_ABC* profile ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged();
    void OnCreate();
    void OnDelete();
    //@}

private:
    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* event );
    virtual void hideEvent( QHideEvent* event );
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::unique_ptr< ProfileEditor > > T_LocalProfiles;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    ProfilesModel& model_;
    T_LocalProfiles localProfiles_;
    UserProfileWidget& pages_;
    gui::RichWidget< QListView >* list_;
    QSortFilterProxyModel* proxyModel_;
    std::auto_ptr< NewProfileDialog > pNewProfileDialog_;    //@}
};

#endif // __UserProfileList_h_
