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

class QPushButton;

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
             LocationEditorBox( kernel::Controllers& controllers,
                                const kernel::CoordinateConverter_ABC& converter,
                                Qt::Orientation orientation = Qt::Horizontal );
    virtual ~LocationEditorBox();
    //@}

    //! @name Operations
    //@{
    bool IsValid() const;
    void AddParser( LocationParser_ABC* parser, const QString& name );
    bool GetPosition( geometry::Point2f& result ) const;
    void UpdateField( const geometry::Point2f& position );
    const LocationParser_ABC* GetCurrentParser() const;
    void Purge();
    //@}

signals:
    //! @name Signals
    //@{
    void DataChanged();
    //@}

public slots:
    //! @name Slots
    //@{
    void SelectParser( int index );
    void UpdateValidity();
    //@}

private slots:
    //! @name Slots
    //@{
    QValidator::State Complete( QString& data, int idx );
    QValidator::State Complete( QString& data, int idx, Field& field );
    //@}

private:
    //! @name Helpers
    //@{
    void ResetFields();
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
    std::vector< Field >                   fields_;
    bool valid_;
    //@}
};

} //! namespace gui

#endif // __LocationEditorBox_h_
