// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OptionWidgets_h_
#define __OptionWidgets_h_

#include "ColorButton.h"
#include "RichSpinBox.h"
#include "RichWidget.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace gui
{
    template< typename T >
    class OptionWidget : public T
                       , public tools::Observer_ABC
                       , public kernel::OptionsObserver_ABC
    {
    public:
        OptionWidget( kernel::OptionsController& options,
                      const QString& objectName,
                      const std::string& optionName,
                      QWidget* parent )
            : T( objectName, parent )
            , options_( options )
            , optionName_( optionName )
        {
            options_.Register( *this );
        }
        ~OptionWidget()
        {
            options_.Unregister( *this );
        }

        virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value )
        {
            if( optionName_ == name )
                OnOptionChanged( value );
        }

    protected:
        virtual void OnOptionChanged( const kernel::OptionVariant& ) {}

    protected:
        kernel::OptionsController& options_;
        const std::string optionName_;
    };

    // -----------------------------------------------------------------------------

    class OptionColorButton : public OptionWidget< ColorButton >
    {
    public:
                OptionColorButton( kernel::OptionsController& options,
                                   const QString& objectName,
                                   const std::string& optionName,
                                   const QColor& color = Qt::black,
                                   QWidget* parent = 0 );
        virtual ~OptionColorButton();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    class OptionCheckBox : public OptionWidget< RichWidget< QCheckBox > >
    {
    public:
                OptionCheckBox( kernel::OptionsController& options,
                                const QString& objectName,
                                const std::string& optionName,
                                const QString& text,
                                QWidget* parent = 0 );
        virtual ~OptionCheckBox();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    class OptionSpinBox : public OptionWidget< RichSpinBox >
    {
    public:
                OptionSpinBox( kernel::OptionsController& options,
                               const QString& objectName,
                               const std::string& optionName,
                               int min,
                               int max,
                               QWidget* parent = 0 );
        virtual ~OptionSpinBox();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    class OptionSlider : public OptionWidget< RichWidget< QSlider > >
    {
    public:
                OptionSlider( kernel::OptionsController& options,
                              const QString& objectName,
                              const std::string& optionName,
                              int min,
                              int max,
                              QWidget* parent = 0 );
        virtual ~OptionSlider();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    class OptionGroupBox : public OptionWidget< RichWidget< QGroupBox > >
    {
    public:
                OptionGroupBox( kernel::OptionsController& options,
                                const QString& objectName,
                                const std::string& optionName,
                                const QString& title,
                                QWidget* parent = 0 );
        virtual ~OptionGroupBox();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    class OptionDial : public OptionWidget< RichWidget< QDial > >
    {
    public:
                OptionDial( kernel::OptionsController& options,
                            const QString& objectName,
                            const std::string& optionName,
                            int min,
                            int max,
                            QWidget* parent = 0 );
        virtual ~OptionDial();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

} //! namespace gui

#endif // __OptionWidgets_h_
