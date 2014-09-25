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

#include "clients_kernel/OptionsObserver_ABC.h"
#include <QtGui/QValidator>
#include <boost/optional.hpp>
#include <vector>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controllers;
    class ContextMenu;
    class OptionsController;
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
                        , public tools::Observer_ABC
                        , public kernel::OptionsObserver_ABC
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
    void AddParser( const std::shared_ptr< const LocationParser_ABC >& parser, const QString& name );
    bool GetPosition( geometry::Point2f& result ) const;
    void UpdateField( const geometry::Point2f& position );
    const LocationParser_ABC& GetCurrentParser() const;
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
    void OnActivated( int index );
    void UpdateValidity();
    //@}

private slots:
    //! @name Slots
    //@{
    QValidator::State Complete( QString& data, int idx );
    QValidator::State Complete( QString& data, int idx, Field& field );
    //@}

private:
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );

    //! @name Helpers
    //@{
    void ResetFields();
    void SetValid( bool valid );
    void SelectParser( int index );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::OptionsController& options_;
    std::unique_ptr< LocationParsers > parsers_;
    std::shared_ptr< const LocationParser_ABC > current_;
    QPushButton* combo_;
    kernel::ContextMenu* menu_;
    std::vector< Field > fields_;
    bool valid_;
    boost::optional< geometry::Point2f > lastValidPosition_;
    //@}
};

} //! namespace gui

#endif // __LocationEditorBox_h_
