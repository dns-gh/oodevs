// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EquipmentType_h_
#define __EquipmentType_h_

namespace xml { class xistream; };

namespace kernel
{

// =============================================================================
/** @class  EquipmentType
    @brief  EquipmentType
*/
// Created: AGE 2006-02-21
// =============================================================================
class EquipmentType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit EquipmentType( xml::xistream& xis );
    virtual ~EquipmentType();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    unsigned long GetId() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EquipmentType( const EquipmentType& );            //!< Copy constructor
    EquipmentType& operator=( const EquipmentType& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    unsigned long id_;
    //@}
};

}

#endif // __EquipmentType_h_
