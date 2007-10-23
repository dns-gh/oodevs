using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.esriSystem;

namespace Crossbow
{
    /// <summary>
    /// Summary description for CreateEventTool.
    /// </summary>
    [ComVisible(true)]
    [Guid("dc81821b-1304-4f9b-b7ec-6846a7a4907c")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("CSword.CreateEventTool")]
    public sealed class CreateEventTool : BaseTool
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

        public CreateEventTool()
        {
            //
            // TODO: Define values for the public properties
            //
            base.m_category = "CSword"; //localizable text 
            base.m_caption = "Create event";  //localizable text 
            base.m_message = "Create an event";  //localizable text
            base.m_toolTip = "Create event";  //localizable text
            base.m_name = "CSword_CreateEvent";   //unique id, non-localizable (e.g. "MyCategory_ArcMapTool")
            try
            {
                //
                // TODO: change resource name if necessary
                //
                string bitmapResourceName = GetType().Name + ".bmp";
                base.m_bitmap = new Bitmap(GetType(), bitmapResourceName);
                base.m_cursor = new System.Windows.Forms.Cursor(GetType(), GetType().Name + ".cur");
            }
            catch (Exception ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.Message, "Invalid Bitmap");
            }
        }

        #region Overriden Class Methods

        public override void OnCreate(object hook)
        {
            if (Tools.IsSupportedApplication(hook))
                Tools.GetCSwordExtension().ModelLoaded += new EventHandler(OnModelLoaded);
            m_enabled = false;
        }

        void OnModelLoaded(object sender, EventArgs e)
        {
            m_enabled = true;
        }

        public override bool OnContextMenu(int X, int Y)
        {
            try
            {
                Tools.GetCSwordExtension().EmergencyHandler.OnContextMenu(X, Y);
                ICommandBar menu = Tools.GetIDocument().CommandBars.Create("EmergencyMenu", esriCmdBarType.esriCmdBarTypeShortcutMenu);
                BuildContextMenu(menu);
                if (menu.Count > 0)
                    menu.Popup(0, 0);
                return true;
            }
            catch (Exception e)
            {
                System.Console.Write("OnContextMenu throws :" + e.Message);
                return false;
            }
        }

        private void BuildContextMenu(ICommandBar menu)
        {
            if (menu == null)
                return;

            UID uid = new UIDClass();
            uid.Value = "Crossbow.EmergencyCreationMenu";
            object Missing = Type.Missing;
            menu.Add(uid, ref Missing);
        }
        #endregion
    }
}
