using System;
using System.Drawing;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Framework;

namespace Sword
{
    namespace Crossbow
    {
        /// <summary>
        /// Summary description for CreateWorkspace.
        /// </summary>
        [ComVisible(true)]
        [Guid("987e32aa-8556-4e83-aa40-02c799ee734b")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("Sword.Crossbow.CreateWorkspace")]
        public class CreateWorkspace : BaseCommand, IDisposable
        {
            #region COM Registration Function(s)
            [ComRegisterFunction()]
            [ComVisible(false)]
            static void RegisterFunction(Type registerType)
            {
                // Required for ArcGIS Component Category Registrar support
                ArcGISCategoryRegistration(registerType);

                //Registry.Register(registerType, //Registry.COMPONENT_MX_COMMAND);
            }

            [ComUnregisterFunction()]
            [ComVisible(false)]
            static void UnregisterFunction(Type registerType)
            {
                // Required for ArcGIS Component Category Registrar support
                ArcGISCategoryUnregistration(registerType);

                //Registry.Unregister(registerType, //Registry.COMPONENT_MX_COMMAND);
            }

            #region ArcGIS Component Category Registrar generated code
            /// <summary>
            /// Required method for ArcGIS Component Category registration -
            /// Do not modify the contents of this method with the code editor.
            /// </summary>
            private static void ArcGISCategoryRegistration(Type registerType)
            {
                string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
                MxCommands.Register(regKey);
                GMxCommands.Register(regKey);
            }
            /// <summary>
            /// Required method for ArcGIS Component Category unregistration -
            /// Do not modify the contents of this method with the code editor.
            /// </summary>
            private static void ArcGISCategoryUnregistration(Type registerType)
            {
                string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
                MxCommands.Unregister(regKey);
                GMxCommands.Unregister(regKey);
            }

            #endregion
            #endregion

            private WorkspaceConfigurationForm m_form;
            private bool m_disposed;

            public CreateWorkspace()
            {
                base.m_category = "Sword"; //localizable text
                base.m_caption = "Create Sword workspace";  //localizable text
                base.m_message = "Create Sword workspace";  //localizable text 
                base.m_toolTip = "Create Sword workspace";  //localizable text 
                base.m_name = "Sword_Crossbow_CreateWorkspace";   //unique id, non-localizable (e.g. "MyCategory_MyCommand")

                try
                {
                    string bitmapResourceName = GetType().Name + ".bmp";
                    base.m_bitmap = new Bitmap(GetType(), bitmapResourceName);
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Trace.WriteLine(ex.Message, "Invalid Bitmap");
                }
            }

            ~CreateWorkspace()
            {
                Dispose(false);
            }

            #region Overriden Class Methods

            /// <summary>
            /// Occurs when this command is created
            /// </summary>
            /// <param name="hook">Instance of the application</param>
            public override void OnCreate(object hook)
            {
                if (Tools.IsSupportedApplication(hook))
                {
                    WorkspaceConfiguration config = Tools.GetCSwordExtension().Config; // $$$$ SBO 2007-08-20: bof bof...
                    m_form = new WorkspaceConfigurationForm(config);
                    m_enabled = m_form != null;
                    if (m_enabled)
                        config.ConfigurationLoaded += new EventHandler(Config_ConfigurationLoaded);
                }
                else
                    m_enabled = false;
            }

            void Config_ConfigurationLoaded(object sender, EventArgs e)
            {
                WorkspaceBuilder.Build((WorkspaceConfiguration)sender);
                Tools.GetCSwordExtension().NotifyModelLoaded();
            }

            /// <summary>
            /// Occurs when this command is clicked
            /// </summary>
            public override void OnClick()
            {
                if (m_form != null)
                    m_form.ShowDialog();
            }

            #endregion

            #region IDisposable Members

            public void Dispose()
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }

            protected virtual void Dispose(bool usercall)
            {
                if (!m_disposed)
                {
                    m_disposed = true;
                    if (usercall && m_form != null && !m_form.IsDisposed)
                        m_form.Dispose();
                }
            }

            #endregion
        }
    }
}