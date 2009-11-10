using ESRI.ArcGIS.ArcGlobe;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Carto;
using System.Collections;


namespace Sword
{
    namespace Crossbow
    {
        sealed class DocumentProxy
        {
            private IMxDocument m_mxDocument;
            private IGMxDocument m_gmxDocument;

            public DocumentProxy(IDocument document)
            {
                if (document is IMxDocument)
                    m_mxDocument = (IMxDocument)document;
                else if (document is IGMxDocument)
                    m_gmxDocument = (IGMxDocument)document;
            }

            public void AddLayer(ILayer layer)
            {
                if (m_mxDocument != null)
                    m_mxDocument.AddLayer(layer);                
                else if (m_gmxDocument != null)
                    m_gmxDocument.AddLayer(layer);
            }

            public void RefreshView()
            {
                if (m_mxDocument != null)
                    m_mxDocument.ActiveView.Refresh();
                else if (m_gmxDocument != null)
                    m_gmxDocument.Scene.SceneGraph.RefreshViewers();
            }

            public void EnableDynamicDisplay(bool enable)
            {
                if (m_mxDocument == null)
                    return;
                IDynamicMap map = m_mxDocument.FocusMap as IDynamicMap;
                if (map == null)
                    throw new System.Exception("Dynamic display not supported");                
                if (map.DynamicMapEnabled != enable)
                {
                    lock (map)
                    {
                        map.DynamicDrawRate = 15;
                        map.DynamicMapEnabled = enable;
                    }
                    // RefreshView();
                }
            }

            public ISelection FeatureSelection
            {
                get
                {
                    if (m_mxDocument != null)
                        return m_mxDocument.FocusMap.FeatureSelection;
                    else if (m_gmxDocument != null)
                        return m_gmxDocument.Scene.FeatureSelection;
                    return null;
                }
            }

            public ISpatialReference SpatialReference
            {
                get
                {
                    if (m_mxDocument != null)
                        return m_mxDocument.ActiveView.ScreenDisplay.DisplayTransformation.SpatialReference;
                    else if (m_gmxDocument != null)
                        return m_gmxDocument.Scene.SpatialReference;
                    return null;
                }
            }

            public IScreenDisplay Display
            {
                get
                {
                    if (m_mxDocument != null)
                        return m_mxDocument.ActiveView.ScreenDisplay;
                    else if (m_gmxDocument != null)
                        return null; // $$$$ SBO 2007-10-01: find something for IGMxApplication
                    return null;
                }
            }

            public double FromPoints(double distance)
            {
                if (m_mxDocument != null)
                    return m_mxDocument.ActiveView.ScreenDisplay.DisplayTransformation.FromPoints(distance);
                //else if (m_gmxDocument != null)
                //    return distance; // $$$$ SBO 2007-10-01: find something for IGMxApplication
                return distance;
            }

            #region Enumeration (should be IEnumerable or something)
            public int LayerCount
            {
                get
                {
                    if (m_mxDocument != null)
                        return m_mxDocument.FocusMap.LayerCount;
                    else if (m_gmxDocument != null)
                        return m_gmxDocument.Scene.LayerCount;
                    return 0;
                }
            }

            public ILayer GetLayer(int index)
            {
                if (index >= LayerCount)
                    return null;
                if (m_mxDocument != null)
                    return m_mxDocument.FocusMap.get_Layer(index);
                else if (m_gmxDocument != null)
                    return m_gmxDocument.Scene.get_Layer(index);
                return null;
            }
            #endregion
        }
    }
}