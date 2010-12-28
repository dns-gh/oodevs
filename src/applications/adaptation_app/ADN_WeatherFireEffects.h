// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_WeatherFireEffects_h_
#define __ADN_WeatherFireEffects_h_

#include "ADN_Equipement_Data.h"

// =============================================================================
/** @class  ADN_WeatherFireEffects
    @brief  ADN_WeatherFireEffects
    @see    4311340-Mod�les de feu.doc
*/
// Created: BCI 2010-12-02
// =============================================================================
class ADN_WeatherFireEffects : public ADN_Ref_ABC
                                 , public ADN_DataTreeNode_ABC

{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_WeatherFireEffects( E_SensorWeatherModifiers weatherType );
    virtual ~ADN_WeatherFireEffects();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetNodeName();
    std::string GetItemName();

    void CopyFrom( ADN_WeatherFireEffects& other );

    void ReadArchive( xml::xistream& );
    void WriteArchive( xml::xostream&, const std::string& tag = "weather-effect" );
    //@}

public:
    //! @name Operators
    //@{
    xml::xostream& operator<<( xml::xostream& xos );
    //@}

public:
    typedef ADN_Equipement_Data::CategoryInfo T_Item;

    class Cmp : public std::unary_function< ADN_WeatherFireEffects* , bool >
    {
    public:
        Cmp( E_SensorWeatherModifiers val ) : val_( val ){}
        bool operator()( ADN_WeatherFireEffects* other ) const
        { return val_ == other->weatherType_; }

    private:
        E_SensorWeatherModifiers val_;
    };

public:
    //! @name Member data
    //@{
    E_SensorWeatherModifiers weatherType_;
    ADN_Type_Int heatDecreaseRate_;
    //@}
};

typedef ADN_Type_Vector_ABC< ADN_WeatherFireEffects > T_WeatherFireEffects_Vector;
typedef T_WeatherFireEffects_Vector::iterator    IT_WeatherFireEffects_Vector;

#endif // __ADN_WeatherFireEffects_h_
