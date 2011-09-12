// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationEditorToolbar_h_
#define __LocationEditorToolbar_h_

#include "LocationEditor_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "FeatureNameParser.h"
#pragma warning( push )
#pragma warning( disable : 4512 )
#include <boost/ptr_container/ptr_map.hpp>
#pragma warning( pop )

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controllers;
}

namespace gui
{
    class ParametersLayer;
    class LocationEditorBox;
    class LocationsLayer;
    class LocationParsers;
    class View_ABC;

// =============================================================================
/** @class  LocationEditorToolbar
    @brief  LocationEditorToolbar
*/
// Created: SBO 2007-03-06
// =============================================================================
class LocationEditorToolbar : public QToolBar
                            , public LocationEditor_ABC
                            , public tools::Observer_ABC
                            , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LocationEditorToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, View_ABC& view, LocationsLayer& layer );
    virtual ~LocationEditorToolbar();
    //@}

    //! @name Operations
    //@{
    virtual void StartEdit( ParametersLayer& parameters );
    virtual void EndEdit();
    //@}

private slots:
    //! @name Slots
    //@{
    void Goto();
    void AddPoint();
    void AddParamPoint();

    void CreateBookmark();
    void GotoBookmark( int index );
    void ClearBookmarks();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationEditorToolbar( const LocationEditorToolbar& );            //!< Copy constructor
    LocationEditorToolbar& operator=( const LocationEditorToolbar& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );
    bool GetPosition( geometry::Point2f& point );
    void SetAspect( bool oneValue, bool red );
    //@}

    //! @name Types
    //@{
    struct Bookmark
    {
        Bookmark() {}
        explicit Bookmark( const std::string& name, const std::string& position )
            : name_( name ), position_( position )
        {}
        std::string name_;
        std::string position_;
    };

    typedef std::vector< Bookmark >       T_Bookmarks;
    typedef T_Bookmarks::const_iterator CIT_Bookmarks;

    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    View_ABC& view_;
    LocationsLayer& layer_;
    ParametersLayer* parameters_;
    QToolButton* okButton_;
    QToolButton* paramsButton_;
    QToolButton* gotoButton_;
    Q3PopupMenu* bookmarksMenu_;
    T_Bookmarks bookmarks_;
    geometry::Point2f menuPoint_;
    FeatureNameParser featureNameParser_;
    LocationEditorBox* locBox_;
    //@}
};

}

#endif // __LocationEditorToolbar_h_
