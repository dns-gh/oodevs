// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ActionsParameters_EngineerConstructionList_h_
#define __ActionsParameters_EngineerConstructionList_h_

#include "Parameter.h"
#include "tools/Resolver_ABC.h"

namespace Common
{
    class MsgPlannedWorkList;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class ObjectType;
    class Automat_ABC;
    class Controller;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  EngineerConstructionList
    @brief  EngineerConstructionList
*/
// Created: JCR 2008-11-03
// =============================================================================
class EngineerConstructionList : public Parameter< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit EngineerConstructionList( const kernel::OrderParameter& parameter );
             EngineerConstructionList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::ObjectType, std::string >& resolver, const tools::Resolver_ABC< kernel::Automat_ABC >& automats, const Common::MsgPlannedWorkList& asn, kernel::Controller& controller );
             EngineerConstructionList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::ObjectType, std::string >& resolver, const tools::Resolver_ABC< kernel::Automat_ABC >& automats, xml::xistream& xis, kernel::Controller& controller );
    virtual ~EngineerConstructionList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& asn ) const;
    virtual void Clean( Common::MsgMissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EngineerConstructionList( const EngineerConstructionList& );            //!< Copy constructor
    EngineerConstructionList& operator=( const EngineerConstructionList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadEngineerConstruction( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::ObjectType, std::string >& resolver, const tools::Resolver_ABC< kernel::Automat_ABC >& automats, kernel::Controller& controller );
    //@}
};

} }

#endif // __ActionsParameters_EngineerConstructionList_h_
