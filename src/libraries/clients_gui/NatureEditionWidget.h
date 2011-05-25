// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NatureEditionWidget_h_
#define __NatureEditionWidget_h_

#include <boost/noncopyable.hpp>

namespace kernel {
    class SymbolRule;
}

namespace gui
{
    class NatureEditionCategory;

// =============================================================================
/** @class  NatureEditionWidget
    @brief  Nature edition widget
*/
// Created: AGE 2006-10-24
// =============================================================================
class NatureEditionWidget : public QVBox
                          , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             NatureEditionWidget( QWidget* parent, const std::string& symbolFile );
    virtual ~NatureEditionWidget();
    //@}

    //! @name Operations
    //@{
    QString text();
    //@}

public slots:
    //! @name Slots
    //@{
    void setText( const QString& );
    //@}

signals:
    //! @name Signals
    //@{
    void textChanged( const QString& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::SymbolRule* root_;
    NatureEditionCategory* rootWidget_;
    //@}
};

}

#endif // __NatureEditionWidget_h_
