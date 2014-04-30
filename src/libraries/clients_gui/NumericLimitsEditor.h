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

#include "Filter_ABC.h"
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
                              , public Filter_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             NumericLimitsEditor_ABC( QWidget* parent );
    virtual ~NumericLimitsEditor_ABC();
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
    typedef std::function< NumericType ( const QStandardItem& item,
                                         bool& valid ) > T_Extractor;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             NumericLimitsEditor( QWidget* parent,
                                  const T_Extractor& extractor,
                                  NumericType min,
                                  NumericType max );
    virtual ~NumericLimitsEditor();
    //@}

    //! @name Filter_ABC operations
    //@{
    virtual bool Apply( QStandardItem& item ) const;
    virtual void Clear();
    virtual QWidget* GetWidget();
    virtual QWidget* GetMenuContent();
    //@}

    //! @name NumericLimitsEditor_ABC operations
    //@{
    virtual void OnMinValueChanged( NumericType value );
    virtual void OnMaxValueChanged( NumericType value );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateInterface();
    //@}

private:
    //! @name Member data
    //@{
    const T_Extractor extractor_;
    NumericType minValue_;
    NumericType maxValue_;
    SpinBox*    minSpin_;
    SpinBox*    maxSpin_;
    //@}
};

} //! namespace gui

#include "NumericLimitsEditor.inl"

#endif // __gui_NumericLimitsEditor_h_
