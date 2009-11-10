using System;
using System.Drawing;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Display;

namespace Sword
{
    namespace Crossbow
    {
        /// <summary>
        /// Summary description for InfoPanelCommand.
        /// </summary>
        [ComVisible(true)]
        [Guid("c1a51b6e-bb30-43d9-84e4-989355fed456")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("Sword.Crossbow.InfoPanelCommand")]
        public sealed class InfoPanelCommand : BaseCommand
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

            private IDockableWindow m_dockableWindow;

            public InfoPanelCommand()
            {
                base.m_category = "Sword"; //localizable text
                base.m_caption = "Information panel";  //localizable text
                base.m_message = "Show/Hide information panel";  //localizable text 
                base.m_toolTip = "Show/Hide information panel";  //localizable text 
                base.m_name = "Sword_Crossbow_InfoPanelCommand";   //unique id, non-localizable (e.g. "MyCategory_MyCommand")

                try
                {
                    //
                    // TODO: change bitmap name if necessary
                    //
                    string bitmapResourceName = GetType().Name + ".bmp";
                    base.m_bitmap = new Bitmap(GetType(), bitmapResourceName);
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Trace.WriteLine(ex.Message, "Invalid Bitmap");
                }
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
                    CreateDockableWindow(hook as IDockableWindowManager);
                    Tools.GetExtension().ModelLoaded += new EventHandler(OnModelLoaded);
                }
                m_enabled = false;
            }

            void OnModelLoaded(object sender, EventArgs e)
            {
                if (m_dockableWindow != null)
                    m_enabled = true;
            }

            /// <summary>
            /// Occurs when this command is clicked
            /// </summary>
            public override void OnClick()
            {
                if (!Enabled || m_dockableWindow == null)
                    return;
                bool visible = m_dockableWindow.IsVisible();
                m_dockableWindow.Show(!visible);
                base.m_checked = visible;
            }

            public override bool Checked
            {
                get
                {
                    return base.m_checked && m_dockableWindow != null && m_dockableWindow.IsVisible();
                }
            }
            #endregion

            private void CreateDockableWindow(IDockableWindowManager dockWindowManager)
            {
                if (m_dockableWindow == null && dockWindowManager != null)
                {
                    UID windowID = new UIDClass();
                    windowID.Value = "Sword.Crossbow.InfoPanel";
                    m_dockableWindow = dockWindowManager.GetDockableWindow(windowID);
                }
            }
        }
    }
}