using System;
using System.Drawing;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.ArcMapUI;

namespace Crossbow
{
    /// <summary>
    /// Summary description for CreateWorkspace.
    /// </summary>
    [ComVisible(true)]
    [Guid("987e32aa-8556-4e83-aa40-02c799ee734b")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("CSword.CreateWorkspace")]
    public sealed class CreateWorkspace : BaseCommand
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

        private WorkspaceConfigurationForm m_workspaceConfigurationForm;

        public CreateWorkspace()
        {
            base.m_category = "CSword"; //localizable text
            base.m_caption = "Create CSword workspace";  //localizable text
            base.m_message = "Create CSword workspace";  //localizable text 
            base.m_toolTip = "Create CSword workspace";  //localizable text 
            base.m_name = "CSword_CreateWorkspace";   //unique id, non-localizable (e.g. "MyCategory_MyCommand")

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

        #region Overriden Class Methods

        /// <summary>
        /// Occurs when this command is created
        /// </summary>
        /// <param name="hook">Instance of the application</param>
        public override void OnCreate(object hook)
        {
            base.m_enabled = hook != null && hook is IMxApplication;
        }

        /// <summary>
        /// Occurs when this command is clicked
        /// </summary>
        public override void OnClick()
        {
            if (m_workspaceConfigurationForm == null)
                m_workspaceConfigurationForm = new WorkspaceConfigurationForm(Tools.GetCSwordExtension().Config);
            m_workspaceConfigurationForm.Show();
        }

        #endregion
    }
}
