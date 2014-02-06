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
    // $$$$ 2014-02-06: TODO: remove this class, use EquipmentType instead

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
    std::string GetName() const;
    float GetWeight() const;
    float GetWidth() const;
    float GetSpacing() const;
    float GetLength() const;
    float GetSafetyDistance() const;
    float GetSpeedSafetyDistance() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ComponentType( const ComponentType& );            //!< Copy constructor
    ComponentType& operator=( const ComponentType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadPresence( xml::xistream& , bool& flag ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned long id_;
    float weight_;
    float width_;
    float spacing_;
    float length_;
    float safetyDistance_;
    float speedSafetyDistance_;

    bool hasMaintenance_;
    bool hasMedical_;
    bool hasSupply_;
    //@}
};

}

#endif // __ComponentType_h_
