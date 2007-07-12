using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Geodatabase;

namespace crossbow
{
    static class Tools
    {
        #region "Get MxDocument from ArcMap"
        // ArcGIS Snippet Title: 
        // Get MxDocument from ArcMap
        //
        // Add the following references to the project:
        // ESRI.ArcGIS.ArcMapUI
        // ESRI.ArcGIS.Framework
        // ESRI.ArcGIS.System
        // 
        // Intended ArcGIS Products for this snippet:
        // ArcGIS Desktop
        //
        // Required ArcGIS Extensions:
        // (NONE)
        //
        // Notes:
        // This snippet is intended to be inserted at the base level of a Class.
        // It is not intended to be nested within an existing Method.
        //
        // Use the following XML documentation comments to use this snippet:
        /// <summary>Get MxDocument from ArcMap.</summary>
        ///
        /// <param name="application">An IApplication interface that is the ArcMap application.</param>
        /// 
        /// <returns>An IMxDocument interface.</returns>
        /// 
        /// <remarks></remarks>
        public static IMxDocument GetMxDocument(ESRI.ArcGIS.Framework.IApplication application)
        {

            if (application == null)
            {
                return null;
            }
            IMxDocument mxDocument = ((IMxDocument)(application.Document)); // Explicit Cast

            return mxDocument;

        }
        #endregion
        
        #region "Get Index Number from Layer Name"
        // ArcGIS Snippet Title: 
        // Get Index Number from Layer Name
        //
        // Add the following references to the project:
        // ESRI.ArcGIS.Carto
        // 
        // Intended ArcGIS Products for this snippet:
        // ArcGIS Desktop
        // ArcGIS Engine
        // ArcGIS Server
        //
        // Required ArcGIS Extensions:
        // (NONE)
        //
        // Notes:
        // This snippet is intended to be inserted at the base level of a Class.
        // It is not intended to be nested within an existing Method.
        //
        // Use the following XML documentation comments to use this snippet:
        /// <summary>Get the index number for the specified layer name.</summary>
        ///
        /// <param name="activeView">An IActiveView interface</param>
        /// <param name="layerName">A System.String that is the layer name in the active view. Example: "states"</param>
        /// 
        /// <returns>An System.Int32 representing a layer number</returns>
        /// 
        /// <remarks>Returns a System.Int32 where values of 0 and greater are valid layers. A return value of -1 means the layer name was not found.</remarks>
        public static System.Int32 GetIndexNumberOfLayerName(ESRI.ArcGIS.Carto.IActiveView activeView, System.String layerName)
        {
            if (activeView == null || layerName == null)
            {
                return -1;
            }
            ESRI.ArcGIS.Carto.IMap map = activeView.FocusMap;

            // Get the number of layers
            int numberOfLayers = map.LayerCount;

            // Loop through the layers and get the correct layer index
            for (System.Int32 i = 0; i < numberOfLayers; i++)
            {
                if (layerName == map.get_Layer(i).Name)
                {
                    // Layer was found
                    return i;
                }
            }

            // No layer was found
            return -1;
        }
        #endregion
                
        #region "Get FeatureLayer from Layer Name"
        public static IFeatureLayer GetIFeatureLayerFromLayerName(ESRI.ArcGIS.Carto.IActiveView activeView, System.String name)
        {
            if (activeView == null || name == null)
                return null;
            int index = GetIndexNumberOfLayerName(activeView, name);
            if( index < 0 )
                return null;
            IMap map = activeView.FocusMap;
            ILayer layer = map.get_Layer( index );
            if (layer is IFeatureLayer)
                return layer as IFeatureLayer;
            else if (layer is ICompositeLayer)
            {
                ICompositeLayer composite = layer as ICompositeLayer;
                for (int i = 0; i < composite.Count - 1; ++i)
                {
                    layer = composite.get_Layer(i);
                    if (layer is IFeatureLayer)
                        return layer as IFeatureLayer;
                }
            }
            return null;
        }
        #endregion

        #region "Retrieve Current Workspace"
        public static IFeatureWorkspace RetrieveWorkspace(IFeatureLayer pLayer)
        {            
            if (pLayer != null)
            {                
                IDataset pDataset = null;
                if ( pLayer is IDynamicLayerDataset )
                {
                    IDynamicLayerDataset dataset = pLayer as IDynamicLayerDataset;
                    pDataset = dataset.Dataset;
                }
                else
                    pDataset = (IDataset)pLayer;
                if ( pDataset != null )
                    return (IFeatureWorkspace)pDataset.Workspace;
            }
            return null;
        }
        
        public static IFeatureWorkspace OpenWorkspace(ESRI.ArcGIS.Carto.IActiveView activeView, System.String name)
        {
            return RetrieveWorkspace(Tools.GetIFeatureLayerFromLayerName(activeView, name));            
        }
        #endregion

        #region "Get value of specified feature field"
        public static System.String GetFieldValueByName( ESRI.ArcGIS.Geodatabase.IFeature feature, System.String fieldName )
        {
            int index = feature.Fields.FindField(fieldName);
            if (index < 0 )
                return null;
            return (System.String)feature.get_Value(index);
        }
        #endregion

        #region "Create IColor from RGB components"
        public static ESRI.ArcGIS.Display.IColor MakeColor(short red, short green, short blue)
        {
            ESRI.ArcGIS.Display.IRgbColor color = new ESRI.ArcGIS.Display.RgbColorClass();
            color.Red = red;
            color.Green = green;
            color.Blue = blue;
            return color as ESRI.ArcGIS.Display.IColor;
        }
        #endregion
    }
}