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
    class ContextMenu;
}

namespace gui
{
     class LocationParsers;
     class LocationParser_ABC;
     class RichPushButton;
     class RichLineEdit;
     template< typename T > class RichWidget;
// =============================================================================
/** @class  LocationEditorBox
    @brief  LocationEditorBox
*/
// Created: AME 2010-03-12
// =============================================================================
class LocationEditorBox : public QWidget
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             LocationEditorBox( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter );
    virtual ~LocationEditorBox();
    //@}

    //! @name Operations
    //@{
    void FillDefaultMenu();
    void AddParser( LocationParser_ABC* parser, const QString& name );
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
    //! @name Helpers
    //@{
    void UpdateParamZone( int index );
    void SetAspect( bool oneValue, bool red );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    std::auto_ptr< LocationParsers > parsers_;
    RichLineEdit* latitude_;
    RichLineEdit* longitude_;
    QLabel* latitudeLabel_;
    QLabel* longitudeLabel_;
    RichPushButton* choiceParserButton_;
    kernel::ContextMenu* parserMenu_;
    RichLineEdit* singleField_;
    QMenu* subMenu_;
    RichWidget< QListWidget >* list_;
    QWidget* coordBox_;
    int parserSelected_;
    //@}
};
}
#endif // __LocationEditorBox_h_
