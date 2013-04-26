// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef gui_PropertyModel_h
#define gui_PropertyModel_h

#include <QtGui/qstandarditemmodel.h>
#include "KeyModel.h"
#include "clients_gui/Property_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/VariantPointer.h"

namespace gui
{
    class PropertyDisplayer;
// =============================================================================
/** @class  PropertyModel
    @brief  Property model
*/
// Created: LGY 2012-08-09
// =============================================================================
class PropertyModel : public KeyModel
                    , public kernel::Displayer_ABC
                    , public tools::Caller< Property_ABC* >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertyModel( PropertyDisplayer& displayer );
    virtual ~PropertyModel();
    //@}

    //! @name Operations
    //@{
    virtual void Clear();
    virtual void Hide();
    virtual void Call( Property_ABC* const& property );

    void Update( QWidget* editor, const QModelIndex& index );
    void Update( const QString& category );
    //@}

private:
    //! @name Operations
    //@{
    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    //@}

    //! @name Helpers
    //@{
    void Update( QStandardItem* parent, E_Category category );
    void Update( QStandardItem* parent, QStandardItem* property );
    //@}

signals:
    //! @name Signals
    //@{
    void InternalItemChanged();
    //@}

private:
    //! @name Member data
    //@{
    PropertyDisplayer& displayer_;
    //@}
};

}

#endif // gui_PropertyModel_h