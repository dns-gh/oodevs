// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_NumericLimitsEditor_h_
#define __gui_NumericLimitsEditor_h_

#include <boost/function.hpp>
#include "RichSpinBox.h"

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
    class StandardModel;

// =============================================================================
/** @class  NumericLimitsEditor_ABC
    @brief  NumericLimitsEditor_ABC
*/
// Created: ABR 2012-06-20
// =============================================================================
class NumericLimitsEditor_ABC : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             NumericLimitsEditor_ABC( QWidget* parent );
    virtual ~NumericLimitsEditor_ABC();
    //@}

    //! @name Abstract operations
    //@{
    virtual bool ApplyFilter( QStandardItem& item, StandardModel& model ) const = 0;
    virtual void Clear() = 0;
    //@}

    //! @name Operations
    //@{

    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnMinValueChanged( int ) {}
    virtual void OnMaxValueChanged( int ) {}
    virtual void OnMinValueChanged( double ) {}
    virtual void OnMaxValueChanged( double ) {}
    //@}

signals:
    //! @name Signals
    //@{
    void ValueChanged();
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

// =============================================================================
/** @class  NumericLimitsEditor
    @brief  NumericLimitsEditor
*/
// Created: ABR 2012-06-20
// =============================================================================
template< typename NumericType, typename SpinBox >
class NumericLimitsEditor : public NumericLimitsEditor_ABC
{
public:
    //! @name Types
    //@{
    typedef boost::function< NumericType ( const kernel::Entity_ABC&, bool& valid ) > T_Extractor;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             NumericLimitsEditor( QWidget* parent = 0 );
    virtual ~NumericLimitsEditor();
    //@}

    //! @name NumericLimitsEditor_ABC operations
    //@{
    virtual bool ApplyFilter( QStandardItem& item, StandardModel& model ) const;
    virtual void Clear();
    virtual void OnMinValueChanged( NumericType value );
    virtual void OnMaxValueChanged( NumericType value );
    //@}

    //! @name Operations
    //@{
    void SetExtractor( T_Extractor extractor );
    void SetMinMax( NumericType min, NumericType max );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateInterface();
    //@}

private:
    //! @name Member data
    //@{
    NumericType minValue_;
    NumericType maxValue_;
    T_Extractor extractor_;
    SpinBox*    minSpin_;
    SpinBox*    maxSpin_;
    //@}
};

} //! namespace gui

#include "NumericLimitsEditor.inl"

#endif // __gui_NumericLimitsEditor_h_
