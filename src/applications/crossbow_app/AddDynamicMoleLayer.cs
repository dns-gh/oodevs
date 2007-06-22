using System;
using System.Drawing;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Catalog;
using ESRI.ArcGIS.CatalogUI;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geodatabase;

namespace crossbow
{
    /// <summary>
    /// Summary description for AddDynamicMoleLayer.
    /// </summary>
    [Guid("603d8bfc-14b0-4a37-a42a-de1f5c7b4156")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("crossbow.AddDynamicMoleLayer")]
    public sealed class AddDynamicMoleLayer : BaseCommand
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

        private IApplication m_application;
        public AddDynamicMoleLayer()
        {
            base.m_category = "CSword"; //localizable text
            base.m_caption = "Add Dynamic Mole Layer";  //localizable text
            base.m_message = "Add a dynamic MOLE layer";  //localizable text 
            base.m_toolTip = "Add Dynamic Mole Layer";  //localizable text 
            base.m_name = "CSword_AddDynamicMoleLayer";   //unique id, non-localizable (e.g. "MyCategory_ArcMapCommand")

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
            if (hook == null)
                return;

            m_application = hook as IApplication;

            //Disable if it is not ArcMap
            if (hook is IMxApplication)
                base.m_enabled = true;
            else
                base.m_enabled = false;
        }

        /// <summary>
        /// Occurs when this command is clicked
        /// </summary>
        public override void OnClick()
        {
            GxDialog openDialog = new GxDialog();
            openDialog.Title = "Add data layer";
            openDialog.ObjectFilter = new GxFilterDatasetsAndLayersClass();
            IEnumGxObject selection;
            if (openDialog.DoModalOpen(0, out selection))
            {
                EnableDynamicDisplay();
                IGxObject element;
                while( (element = selection.Next()) != null )
                {
                    IGxDataset gxDs = element as IGxDataset;
                    if (gxDs != null)
                    {
                        IDataset ds = gxDs.Dataset;
                        DynamicMoleLayer layer = new DynamicMoleLayer();
                        layer.Name = element.Name;
                        layer.FeatureClass = ds as IFeatureClass;

                        IMxDocument mxDocument = Tools.GetMxDocument(m_application);
                        mxDocument.AddLayer(layer);
                        mxDocument.ActiveView.Refresh();
                    }
                }
            }
        }
        #endregion

        private void EnableDynamicDisplay()
        {
            IMxDocument mxDocument = Tools.GetMxDocument(m_application);
            IDynamicMap map = mxDocument.FocusMap as IDynamicMap;
            if (map == null)
                throw new System.Exception("Dynamic display not supported");
            if (!map.DynamicMapEnabled)
            {
                map.DynamicMapEnabled = true;
                mxDocument.ActiveView.Refresh();
            }
        }
    }
}
