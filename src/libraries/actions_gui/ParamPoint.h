// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef ParamPoint_h
#define ParamPoint_h

#include "ParamLocation_ABC.h"

namespace gui
{
    class FeatureNameParser;
    class LocationEditorBox;
}

namespace actions
{

namespace gui
{

// =============================================================================
/** @class  ParamPoint
    @brief  ParamPoint
*/
// Created: ABR 2014-02-26
// =============================================================================
class ParamPoint : public ParamLocation_ABC< actions::parameters::Point >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamPoint( const InterfaceBuilder_ABC& builder,
                         const kernel::OrderParameter& parameter );
    virtual ~ParamPoint();
    //@}

    //! @name Operations
    //@{
    virtual void Purge();
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void SetSwitchEditorChecked( bool checked );
    virtual void Handle( kernel::Location_ABC& location );
    virtual void InternalVisit( const actions::parameters::Location& param );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSwitchEditorClicked( bool );
    void OnEditorDataChanged();
    //@}

private:
    //! @name Types
    //@{
    typedef ParamLocation_ABC< actions::parameters::Point > T_Parent;
    //@}

private:
    //! @name Member data
    //@{
    ::gui::LocationEditorBox* locationEditor_;
    std::shared_ptr< ::gui::FeatureNameParser > featureNameParser_;
    //@}
};

} //! namespace gui

} //! namespace actions

#endif // ParamPoint_h
