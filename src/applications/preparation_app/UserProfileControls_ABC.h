// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileControls_ABC_h_
#define __UserProfileControls_ABC_h_

#include <boost/noncopyable.hpp>

namespace gui
{
    class ValuedListItem;
}

namespace kernel
{
    class Entity_ABC;
}

class UserProfile;
class ControlsChecker_ABC;

// =============================================================================
/** @class  UserProfileControls_ABC
    @brief  User profile controls declaration
*/
// Created: LGY 2011-09-12
// =============================================================================
class UserProfileControls_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UserProfileControls_ABC( Q3ListView* listView, ControlsChecker_ABC& checker );
    virtual ~UserProfileControls_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateFilter();
    virtual void HideAssignedAutomats();
    virtual void ShowAssignedAutomats();
    virtual void RemoveFilter();

    virtual void Display( UserProfile& profile );
    void Update( bool supervisor, UserProfile& profile );
    void Initialize( bool collapse = true );
    bool NeedsSaving();
    void Commit();
    void BindWithProfileControl( UserProfileControls_ABC* profileControl );
    //@}

protected:
    //! @name Slots
    //@{
    void OnItemClicked( Q3ListViewItem* item, const QPoint& point, int column );
    //@}

    //! @name Helpers
    //@{
    virtual bool CanWrite( const kernel::Entity_ABC* entity ) const;
    //@}

    //! @name Types
    //@{
    enum Status
    {
        eNothing,
        eControl,
    };
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, const kernel::Entity_ABC* > T_Error;
    typedef std::vector< T_Error >                              T_Errors;
    //@}

    //! @name Helpers
    //@{
    void SetItem( Q3ListViewItem* item, Status status );

    void ReadRights( gui::ValuedListItem* item, bool control );
    void UpdateRights( gui::ValuedListItem* item, bool control );
    void SelectChild( gui::ValuedListItem* item );
    void SelectParent( gui::ValuedListItem* item );
    void DeselectParent( gui::ValuedListItem* item );
    void DeselectChild( gui::ValuedListItem* item );
    bool IsControlled( gui::ValuedListItem* item ) const;
    bool AreAllChildrenControlled( const gui::ValuedListItem* item );
    const gui::ValuedListItem* GetListItem( unsigned int entityID );

    void Check( gui::ValuedListItem* item, bool control );
    void CheckErrors( gui::ValuedListItem* item, T_Errors& errors );
    void CheckErrors( const kernel::Entity_ABC& entity, T_Errors& errors );

    void UpdateColor( gui::ValuedListItem* item );
    //@}

protected:
    UserProfile* selectedProfile_;

private:
    //! @name Member data
    //@{
    Q3ListView* listView_;
    ControlsChecker_ABC& checker_;
    QPixmap check_;
    bool supervisor_;
    bool needsSaving_;
    UserProfileControls_ABC* profileControl_;
    //@}
};

#endif // __UserProfileControls_ABC_h_
