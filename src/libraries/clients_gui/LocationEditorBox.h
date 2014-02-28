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

#include <vector>
#include <QtGui/QValidator>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controllers;
    class ContextMenu;
}

namespace gui
{
     class LocationParsers;
     class LocationParser_ABC;
}

namespace gui
{
struct Field
{
    QLabel*    label;
    QLineEdit* edit;
    QString    last;
};

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
    void UpdateField( const geometry::Point2f& position );
    const LocationParser_ABC* GetCurrentParser() const;
    //@}

public slots:
    //! @name Slots
    //@{
    void SelectParser( int index );
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectHint( int index );
    QValidator::State Complete( QString& data, int idx );
    QValidator::State Complete( QString& data, int idx, Field& field );
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateParamZone();
    void SetValid( bool valid );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    std::auto_ptr< LocationParsers >       parsers_;
    LocationParser_ABC*                    current_;
    QPushButton*                           combo_;
    kernel::ContextMenu*                   menu_;
    QMenu*                                 subMenu_;
    QListWidget*                           hints_;
    std::vector< Field >                   fields_;
    //@}
};
}
#endif // __LocationEditorBox_h_
