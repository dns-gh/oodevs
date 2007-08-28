using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.ADF.BaseClasses;

namespace Crossbow
{
    /// <summary>
    /// Summary description for CSwordBar.
    /// </summary>
    [ComVisible(true)]
    [Guid("666b9717-13e0-47d0-a388-7b3b7b581342")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("CSword.CSwordBar")]
    public sealed class CSwordBar : BaseToolbar
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
            MxCommandBars.Register(regKey);
        }
        /// <summary>
        /// Required method for ArcGIS Component Category unregistration -
        /// Do not modify the contents of this method with the code editor.
        /// </summary>
        private static void ArcGISCategoryUnregistration(Type registerType)
        {
            string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
            MxCommandBars.Unregister(regKey);
        }

        #endregion
        #endregion

        public CSwordBar()
        {
            AddItem("CSword.CSwordMenu");
            AddItem("CSword.CreateWorkspace");
            AddItem("CSword.AddDynamicMoleLayer");
            AddItem("CSword.OrbatCommand");
            AddItem("CSword.FilterView");
        }

        public override string Caption
        {
            get
            {
                return "CSwordBar";
            }
        }
        public override string Name
        {
            get
            {
                return "CSwordBar";
            }
        }
    }
}