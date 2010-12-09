// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class FormationLevels;
}

namespace xml
{
    class xistream;
}

class IdManager;

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: SBO 2006-09-19
// =============================================================================
class Formation : public kernel::EntityImplementation< kernel::Formation_ABC >
                , public kernel::Extension_ABC
                , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( kernel::Controller& controller, const kernel::HierarchyLevel_ABC& level, IdManager& idManager );
             Formation( xml::xistream& xis, kernel::Controller& controller, const kernel::FormationLevels& levels, IdManager& idManager );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
    void Rename( const QString& name );
    virtual const kernel::LogisticLevel& GetLogisticLevel() const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::HierarchyLevel_ABC* level_;
    kernel::LogisticLevel*  logisticLevel_;
    std::string color_;
    std::string nature_;
    //@}
};

#endif // __Formation_h_
