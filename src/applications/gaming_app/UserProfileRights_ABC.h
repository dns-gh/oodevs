// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileRights_ABC_h_
#define __UserProfileRights_ABC_h_

namespace gui
{
    class ValuedListItem;
}

class UserProfile;

// =============================================================================
/** @class  UserProfileRights_ABC
    @brief  UserProfileRights_ABC
*/
// Created: SBO 2007-01-18
// =============================================================================
class UserProfileRights_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UserProfileRights_ABC( QListView* listView );
    virtual ~UserProfileRights_ABC();
    //@}

    //! @name Operations
    //@{
    void Commit( bool savedStatus );
    void Reset();
    bool NeedsSaving() const;
    virtual bool NeedsCommit() const = 0;

    void Display( UserProfile& profile );
    //@}

protected:
    //! @name Slots
    //@{
    void OnItemClicked( QListViewItem* item, const QPoint& point, int column );
    void OnShow();
    void OnHide();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfileRights_ABC( const UserProfileRights_ABC& );            //!< Copy constructor
    UserProfileRights_ABC& operator=( const UserProfileRights_ABC& ); //!< Assignment operator
    //@}

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
    void CloseAll();

    void SetStatus( QListViewItem* item, Status status );
    void SetStatus( gui::ValuedListItem* item, bool inheritsReadable, bool inheritsWriteable );
    void SetStatus( gui::ValuedListItem* item, bool isReadable, bool isWriteable, bool inheritsReadable, bool inheritsWriteable );
    Status MakeStatus( bool read, bool write, bool inheritedRead, bool inheritedWrite );
    void SetInheritedStatus( QListViewItem* item, Status status );
    //@}

private:
    //! @name Member data
    //@{
    QListView* listView_;
    UserProfile* selectedProfile_;
    bool needsSaving_;

    QPixmap check_, check_grey_;
    //@}
};

#endif // __UserProfileRights_ABC_h_
