using System;
using System.Drawing;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Display;

namespace Crossbow
{
    /// <summary>
    /// Summary description for dockable window toggle command
    /// </summary>
    [ComVisible(true)]
    [Guid("3ea5717a-2554-4839-9daf-667305a11285")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("Crossbow.CommandOrbatCommand")]
    public class OrbatCommand : BaseCommand, IDisposable
    {
        #region COM Registration Function(s)
        [ComRegisterFunction()]
        [ComVisible(false)]
        static void RegisterFunction(Type registerType)
        {
            // Required for ArcGIS Component Category Registrar support
            ArcGISCategoryRegistration(registerType);

            //
            // TODO: Add any COM registration code here
            //
        }

        [ComUnregisterFunction()]
        [ComVisible(false)]
        static void UnregisterFunction(Type registerType)
        {
            // Required for ArcGIS Component Category Registrar support
            ArcGISCategoryUnregistration(registerType);

            //
            // TODO: Add any COM unregistration code here
            //
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

        }
        /// <summary>
        /// Required method for ArcGIS Component Category unregistration -
        /// Do not modify the contents of this method with the code editor.
        /// </summary>
        private static void ArcGISCategoryUnregistration(Type registerType)
        {
            string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
            MxCommands.Unregister(regKey);

        }

        #endregion
        #endregion

        private IDockableWindow m_dockableWindow;
        private const string DockableWindowGuid = "{02a14a61-dd9f-493e-80be-7927ae8a5bac}";
       
        public OrbatCommand()
        {
            base.m_category = "CSword"; //localizable text
            base.m_caption = "Show/Hide Orbat";  //localizable text
            base.m_message = "Command toggles dockable Orbat";  //localizable text 
            base.m_toolTip = "Toggle Dockable Orbat (C#)";  //localizable text 
            base.m_name = "DeveloperTemplate_OrbatCommand";   //unique id, non-localizable (e.g. "MyCategory_ArcMapCommand")

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

        ~OrbatCommand()
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
            IApplication application = hook as IApplication;
            if (application != null)
                SetupDockableWindow(application as IDockableWindowManager);                
            base.m_enabled = m_dockableWindow != null;
        }

        /// <summary>
        /// Toggle visiblity of dockable window and show the visible state by its checked property
        /// </summary>
        public override void OnClick()
        {
            if (m_dockableWindow == null)
                return;            
            if (m_dockableWindow.IsVisible())
            {                
                m_dockableWindow.Show(false);
                // IOrbat orbat = (IOrbat)dockWndDef.UserData;
            }
            else
            {
                IOrbat orbat = (IOrbat)m_dockableWindow.UserData;
                if (orbat != null)
                    orbat.Load();
                m_dockableWindow.Show(true);
            }

            base.m_checked = m_dockableWindow.IsVisible();
        }

        public override bool Checked
        {
            get
            {
                return m_dockableWindow != null && m_dockableWindow.IsVisible();
            }
        }
        #endregion

        private void SetupDockableWindow(IDockableWindowManager dockWindowManager)
        {
            if (m_dockableWindow == null && dockWindowManager != null)
            {
                UID windowID = new UIDClass();
                windowID.Value = DockableWindowGuid;
                m_dockableWindow = dockWindowManager.GetDockableWindow(windowID);
            }
        }

        #region IDisposable Members

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposing)
                base.m_bitmap.Dispose();
        }

        #endregion
    }
}
