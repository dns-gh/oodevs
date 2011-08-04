// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_GraphValue_h_
#define __ADN_GraphValue_h_

#include "ADN_Types.h"
#include <boost/noncopyable.hpp>

class ADN_GraphData;

// =============================================================================
/** @class  ADN_GraphValue
    @brief  ADN_GraphValue
    @par    Using example
    @code
    ADN_GraphValue;
    @endcode
*/
// Created: APE 2004-12-14
// =============================================================================
class ADN_GraphValue : public QObject
                     , private boost::noncopyable
{
    Q_OBJECT

public:
    typedef std::pair< double, double > T_Point;

public:
    //! @name Constructors/Destructor
    //@{
     ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Int&    x, ADN_Type_Int&    y );
     ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Double& x, ADN_Type_Int&    y );
     ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Double& x, ADN_Type_Double& y );
     ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Int&    x, ADN_Type_Double& y );
     ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Int&    y );
     ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Double& y );
    virtual ~ADN_GraphValue();
    //@}

    //! @name Accessors
    //@{
    void* GetRelatedObject();
    T_Point GetPoint();
    //@}

private:
    //! @name Helpers
    //@{
    void ConstructorHelper();
    //@}


private slots:
    //! @name Internal slots
    //@{
    void OnDataChanged();
    //@}

private:
    enum E_XYTypes
    {
        eNotSet         = 0,
        eNoX            = 1,
        eXInt           = 2,
        eXDouble        = 4,
        eYInt           = 8,
        eYDouble        = 16
    };

private:
    //! @name Member data
    //@{
    ADN_GraphData& graphData_;
    void* pRelatedObject_;

    E_XYTypes       nTypes_;
    ADN_Type_Double rX_;
    ADN_Type_Int    nX_;
    ADN_Type_Double rY_;
    ADN_Type_Int    nY_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_GraphValue::GetRelatedObject
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
inline
void* ADN_GraphValue::GetRelatedObject()
{
    return pRelatedObject_;
}

#endif // __ADN_GraphValue_h_
