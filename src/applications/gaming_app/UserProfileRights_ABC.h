// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileRights_ABC_h_
#define __UserProfileRights_ABC_h_

#include <boost/noncopyable.hpp>
#include "clients_gui/StandardModelVisitor_ABC.h"

namespace gui
{
    class RichTreeView;
    class StandardModel;
}

class UserProfile;

// =============================================================================
/** @class  UserProfileRights_ABC
    @brief  UserProfileRights_ABC
*/
// Created: SBO 2007-01-18
// =============================================================================
class UserProfileRights_ABC : public gui::StandardModelVisitor_ABC
                            , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UserProfileRights_ABC( gui::RichTreeView& listView, gui::StandardModel& model, const QString& name );
    virtual ~UserProfileRights_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Visit( QStandardItem& item );
    void Commit( bool savedStatus );
    void Reset();
    bool NeedsSaving() const;
    virtual bool NeedsCommit() const = 0;

    void Display( UserProfile& profile );
    //@}

protected:
    //! @name Slots
    //@{
    void OnItemClicked( const QModelIndex& index );
    void OnShow();
    void OnHide();
    //@}

private:
    //! @name Types
    //@{
    enum Status
    {
        eNothing,
        eReadOnly,
        eReadInherited,
        eWrite,
        eWriteInherited
    };
    //@}

    //! @name Helpers
    //@{
    void SetStatus( QStandardItem* item, Status status );
    void SetStatus( QStandardItem* item, bool inheritsReadable, bool inheritsWriteable );
    void SetStatus( QStandardItem* item, bool isReadable, bool isWriteable, bool inheritsReadable, bool inheritsWriteable );
    Status MakeStatus( bool read, bool write, bool inheritedRead, bool inheritedWrite );
    void EnsureVisible( QStandardItem* item );
    //@}

private:
    //! @name Member data
    //@{
    gui::RichTreeView& listView_;
    gui::StandardModel& model_;
    UserProfile* profile_;
    bool needsSaving_;

    QPixmap check_, check_grey_;
    //@}
};

#endif // __UserProfileRights_ABC_h_
