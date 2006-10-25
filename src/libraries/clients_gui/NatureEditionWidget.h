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
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             NatureEditionWidget( QWidget* parent, const std::string& symbolFile);
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
    //! @name Copy/Assignment
    //@{
    NatureEditionWidget( const NatureEditionWidget& );            //!< Copy constructor
    NatureEditionWidget& operator=( const NatureEditionWidget& ); //!< Assignement operator
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
