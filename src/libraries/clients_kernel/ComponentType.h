// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ComponentType_h_
#define __ComponentType_h_

namespace xml { class xistream; };

namespace kernel
{

// =============================================================================
/** @class  ComponentType
    @brief  ComponentType
*/
// Created: AGE 2006-02-14
// =============================================================================
class ComponentType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ComponentType( xml::xistream& xis );
    virtual ~ComponentType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    QString GetName() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ComponentType( const ComponentType& );            //!< Copy constructor
    ComponentType& operator=( const ComponentType& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadPresence( xml::xistream& , bool& flag ) const;
    //@}

private:
    //! @name Member data
    //@{
    QString   name_;
    unsigned long id_;

    bool hasMaintenance_;
    bool hasMedical_;
    bool hasSupply_;
    //@}
};

}

#endif // __ComponentType_h_
