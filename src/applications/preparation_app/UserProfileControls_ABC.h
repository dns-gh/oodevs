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
    void Update( bool supervisor );
    //@}

protected:
    //! @name Slots
    //@{
    void OnItemClicked( Q3ListViewItem* item, const QPoint& point, int column );
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
    void Commit();
    void Clear();
    void SetItem( Q3ListViewItem* item, Status status );

    void ReadRights( gui::ValuedListItem* item, bool control );
    void UpdateRights( gui::ValuedListItem* item, bool control );
    void Select( gui::ValuedListItem* item, bool control );
    void Deselect( gui::ValuedListItem* item );
    bool IsControlled( gui::ValuedListItem* item ) const;

    void Check( gui::ValuedListItem* item, bool control );
    void CheckErrors( gui::ValuedListItem* item, T_Errors& errors );
    void CheckErrors( const kernel::Entity_ABC& entity, T_Errors& errors );

    void UpdateColor( gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    Q3ListView* listView_;
    ControlsChecker_ABC& checker_;
    UserProfile* profile_;
    QPixmap check_;
    bool supervisor_;
    //@}
};

#endif // __UserProfileControls_ABC_h_
