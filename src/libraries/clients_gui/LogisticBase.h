// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LogisticBase_h_
#define __LogisticBase_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
    class Controllers;
    class Entity_ABC;
}

namespace sword
{
    class AutomatAttributes;
    class AutomatCreation;
    class FormationCreation;
    class FormationUpdate;
    enum EnumLogisticLevel;
}

namespace gui
{
    class PropertiesDictionary;

// =============================================================================
/** @class  LogisticBase
    @brief  LogisticBase
*/
// Created: ABR 2014-01-17
// =============================================================================
class LogisticBase : public kernel::Extension_ABC
                   , public kernel::Serializable_ABC
                   , public kernel::Updatable_ABC< sword::AutomatAttributes >
                   , public kernel::Updatable_ABC< sword::AutomatCreation >
                   , public kernel::Updatable_ABC< sword::FormationCreation >
                   , public kernel::Updatable_ABC< sword::FormationUpdate >
                   , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticBase( kernel::Controllers& controllers,
                           const kernel::Entity_ABC& entity,
                           gui::PropertiesDictionary& dictionary,
                           bool active,
                           bool isBase );
             LogisticBase( kernel::Controllers& controllers,
                           const kernel::Entity_ABC& entity,
                           gui::PropertiesDictionary& dictionary,
                           bool active,
                           xml::xistream& xis );
    virtual ~LogisticBase();
    //@}

    //! @name accessors
    //@{
    bool IsBase() const;
    void SetBase( const bool& isBase );
    const kernel::Entity_ABC& GetEntity() const;
    bool IsMaintenanceManual() const;
    bool IsSupplyManual() const;
    //@}

private:
    //! @name Serializable_ABC implementation
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Updatable_ABC
    //@{
    virtual void DoUpdate( const sword::AutomatCreation& message );
    virtual void DoUpdate( const sword::AutomatAttributes& message );
    virtual void DoUpdate( const sword::FormationCreation& message );
    virtual void DoUpdate( const sword::FormationUpdate& message );
    //@}

    //! @name Helpers
    //@{
    bool SetMaintenanceManual( bool manual );
    bool SetSupplyManual( bool manual );
    template< typename T >
    void Update( const T& message );
    void CreateDictionary( gui::PropertiesDictionary& dictionary,
                           bool active,
                           bool readOnly );
    //@}

private:
    //! @name Attributes
    //@{
    kernel::Controller& controller_;
    const kernel::Entity_ABC& entity_;
    bool isBase_;
    bool isMaintenanceManual_;
    bool isSupplyManual_;
    //@}
};

} //! namespace gui

#endif // __LogisticBase_h_
