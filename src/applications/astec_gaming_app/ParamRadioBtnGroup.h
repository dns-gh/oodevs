// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamRadioBtnGroup_h_
#define __ParamRadioBtnGroup_h_

#include "Param_ABC.h"
#include <qhbuttongroup.h>

// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
template< typename T >
class ParamRadioBtnGroup : public QHButtonGroup, public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamRadioBtnGroup( QWidget* parent, T& output, const std::string strLabel );
    virtual ~ParamRadioBtnGroup();
    //@}

    //! @name Operations
    //@{
    void AddButton( const QString& label, const T& value, bool selected = false );
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamRadioBtnGroup( const ParamRadioBtnGroup& );
    ParamRadioBtnGroup& operator=( const ParamRadioBtnGroup& );
    //@}

private:
    //! @name Member data
    //@{
    T& output_;
    std::vector< T > values_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< typename T >
ParamRadioBtnGroup<T>::ParamRadioBtnGroup( QWidget* parent, T& output, const std::string strLabel  )
    : QHButtonGroup( strLabel.c_str(), parent, strLabel.c_str() )
    , output_( output )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< typename T >
ParamRadioBtnGroup<T>::~ParamRadioBtnGroup()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup::AddButton
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
template< typename T >
void ParamRadioBtnGroup<T>::AddButton( const QString& label, const T& value, bool selected /*= false*/ )
{
    QRadioButton* button = new QRadioButton( label, this, 0 );
    button->setChecked( values_.empty() || selected );
    values_.push_back( value );
}

// -----------------------------------------------------------------------------
// Name: ParamRadioBtnGroup::Commit
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
template< typename T >
void ParamRadioBtnGroup<T>::Commit()
{
    output_ = values_.at( selectedId() );
}

#endif // __ParamRadioBtnGroup_h_
