// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLocationList_h_
#define __ParamLocationList_h_

#include "game_asn/Asn.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "ParamListView.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class LocationCreator;
    class ParametersLayer;
}

// =============================================================================
/** @class  ParamLocationList
    @brief  ParamLocationList
*/
// Created: AGE 2006-04-03
// =============================================================================
class ParamLocationList : public ParamListView, private gui::ShapeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamLocationList( QObject* pParent, const QString& name, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamLocationList();

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Handle( kernel::Location_ABC& location );
    virtual bool CheckValidity();
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    //@}

public:
    //! @name Slots
    //@{
    virtual void OnDeleteSelectedItem();
    virtual void OnClearList();
    //@}

protected:
    //! @name Helpers
    //@{
    void CommitTo( ASN1T_ListLocalisation*& asn ) const;
    void Clean( ASN1T_ListLocalisation*& asn ) const;
    void SetShapeFilter( bool point, bool line, bool polygon, bool circle );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamLocationList( const ParamLocationList& );            //!< Copy constructor
    ParamLocationList& operator=( const ParamLocationList& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< kernel::Location_ABC* > T_Locations;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    gui::LocationCreator* creator_;
    T_Locations locations_;
    kernel::ActionController* controller_;
    //@}
};

#endif // __ParamLocationList_h_
