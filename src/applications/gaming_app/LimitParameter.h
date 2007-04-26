// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimitParameter_h_
#define __LimitParameter_h_

#include "game_asn/Asn.h"
#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class TacticalLine_ABC;
    class CoordinateConverter_ABC;
}

namespace gui
{
    class RichLabel;
}

class Limit;
class ActionParameter_ABC;

// =============================================================================
/** @class  LimitParameter
    @brief  LimitParameter
*/
// Created: SBO 2006-11-14
// =============================================================================
class LimitParameter : public QObject
                     , public Param_ABC
                     , public kernel::ContextMenuObserver_ABC< kernel::TacticalLine_ABC >
                     , public kernel::ElementObserver_ABC< Limit >

{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LimitParameter( QObject* pParent, const QString& name, const kernel::CoordinateConverter_ABC& converter, bool optional );
    virtual ~LimitParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent );
    void CommitTo( ASN1T_Line& asn ) const;
    void Clean( ASN1T_Line& asn ) const;
    bool IsSet() const;
    void CommitTo( ActionParameter_ABC& parameter ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void MenuItemValidated();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LimitParameter( const LimitParameter& );            //!< Copy constructor
    LimitParameter& operator=( const LimitParameter& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Display( const QString& what );
    virtual void NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const Limit& ) {};
    virtual void NotifyDeleted( const Limit& entity );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    gui::RichLabel* pLabel_;
    QLabel*         entityLabel_; // $$$$ AGE 2006-03-14: LabelDisplayer ?
    const Limit*    potential_;
    const Limit*    selected_;
    bool            optional_;
    //@}
};

#endif // __LimitParameter_h_
