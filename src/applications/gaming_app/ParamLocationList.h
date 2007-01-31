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
#include "Param_ABC.h"
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

class LocationSerializer;

// =============================================================================
/** @class  ParamLocationList
    @brief  ParamLocationList
*/
// Created: AGE 2006-04-03
// =============================================================================
class ParamLocationList : public ParamListView, public Param_ABC, private gui::ShapeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamLocationList( QWidget* pParent, ASN1T_ListLocalisation*& asn, const QString& label, const QString& menu, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
             ParamLocationList( QWidget* pParent, ASN1T_ListPolygon*& asn,      const QString& label, const QString& menu, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
             ParamLocationList( QWidget* pParent, ASN1T_ListPoint*& asn,        const QString& label, const QString& menu, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamLocationList();

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Handle( kernel::Location_ABC& location );
    virtual bool CheckValidity();
    virtual void Commit();
    //@}

public:
    //! @name Slots
    //@{
    virtual void OnDeleteSelectedItem();
    virtual void OnClearList();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamLocationList( const ParamLocationList& );            //!< Copy constructor
    ParamLocationList& operator=( const ParamLocationList& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< kernel::Location_ABC* >       T_Locations;
    typedef std::vector< LocationSerializer* > T_Serializers;
    //@}

    //! @name Helpers
    //@{
    void ClearSerializers();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    ASN1T_ListLocalisation* asn_;
    ASN1T_Localisation* pAsnLocalisationList_;

    gui::LocationCreator* creator_;
    T_Locations locations_;
    T_Serializers serializers_;
    kernel::ActionController* controller_;
    //@}
};

#endif // __ParamLocationList_h_
