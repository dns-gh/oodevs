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

namespace Sword
{
    namespace Crossbow
    {
        /// <summary>
        /// Summary description for AddDynamicMoleLayer.
        /// </summary>
        [ComVisible(true)]
        [Guid("603d8bfc-14b0-4a37-a42a-de1f5c7b4156")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("Sword.Crossbow.AddDynamicMoleLayer")]
        public sealed class AddDynamicMoleLayer : BaseCommand
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

            public AddDynamicMoleLayer()
            {
                base.m_category = "Sword"; //localizable text
                base.m_caption = "Add Dynamic Mole Layer";  //localizable text
                base.m_message = "Add a dynamic MOLE layer";  //localizable text 
                base.m_toolTip = "Add Dynamic Mole Layer";  //localizable text 
                base.m_name = "Sword_Crossbow_AddDynamicMoleLayer";   //unique id, non-localizable (e.g. "MyCategory_ArcMapCommand")

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
                base.m_enabled = Tools.IsSupportedApplication(hook);
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
                    IGxObject element;
                    while ((element = selection.Next()) != null)
                    {
                        IGxDataset gxDs = element as IGxDataset;
                        if (gxDs != null)
                        {
                            ILayer layer = CreateLayerGroup(element.Name, gxDs.Dataset as IFeatureClass);
                            DocumentProxy document = Tools.GetDocument();
                            document.AddLayer(layer);
                            document.RefreshView();
                        }
                    }
                }
            }
            #endregion

            private static ILayer CreateLayerGroup(string name, IFeatureClass featureClass)
            {
                IGroupLayer group = new GroupLayerClass();
                ILayer layer = group as ILayer;
                layer.Name = name;

                IFeatureLayer featureLayer = new FeatureLayerClass();
                featureLayer.Name = layer.Name + " - Features";
                featureLayer.FeatureClass = featureClass;
                group.Add(featureLayer);

                DynamicLayer dynamicLayer = new DynamicLayer(Tools.GetExtension().SymbolFactory);
                dynamicLayer.Name = layer.Name + " - Dynamics";
                dynamicLayer.FeatureClass = featureClass;
                dynamicLayer.Connect();
                group.Add(dynamicLayer);
                return group as ILayer;
            }
        }
    }
}