// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LocationEditorBox_h_
#define __LocationEditorBox_h_

namespace kernel
{
    class CoordinateSystems;
    class CoordinateConverter_ABC;
    class Controllers;
}

namespace gui
{
     class LocationParsers;
     class LocationParser_ABC;
// =============================================================================
/** @class  LocationEditorBox
    @brief  LocationEditorBox
*/
// Created: AME 2010-03-12
// =============================================================================
class LocationEditorBox : public Q3HBox
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             LocationEditorBox( QWidget* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter );
    virtual ~LocationEditorBox();
    //@}

    //! @name Operations
    //@{
    void FillDefaultMenu();
    void AddParser( LocationParser_ABC& parser, const QString& name );
    bool GetPosition( geometry::Point2f& result );
    void SelectDefaultParser( int index );
    void UpdateField( const geometry::Point2f& position );
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectParser( int index );
    void GetSelectedItemInSubList( int index );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationEditorBox( const LocationEditorBox& );            //!< Copy constructor
    LocationEditorBox& operator=( const LocationEditorBox& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void UpdateParamZone( int index );
    void SetAspect( bool oneValue, bool red );
    //@}

private:
    //! @name Member data
    //@{
    LocationParsers* parsers_;
    kernel::Controllers& controllers_;
    const kernel::CoordinateSystems& coordinateSystems_;
    const kernel::CoordinateConverter_ABC& converter_;
    QLineEdit* latitude_;
    QLineEdit* longitude_;
    QLabel* latitudeLabel_;
    QLabel* longitudeLabel_;
    QToolButton* choiceParserButton_;
    Q3PopupMenu* parserMenu_;
    QLineEdit* singleField_;
    QMenu* subMenu_;
    Q3ListBox* list_;
    Q3HBox* coordBox_;
    Q3HBox* valueBox_;
    int parserSelected_;
    //@}
};
}
#endif // __LocationEditorBox_h_
