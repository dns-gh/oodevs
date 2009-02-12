using System;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geometry;

namespace Sword
{
    namespace Crossbow
    {
        public sealed class ParameterLimit : OrderParameter
        {
            public ParameterLimit(String name)
                : base(name, ParameterTypeFactory.Create("Line"))
            {
            }

            public void UpdateGeometry(IGeometry geometry)
            {
                base.SetValue(geometry);
            }

            public void UpdateGeometry(IPoint first, IPoint second)
            {
                IPolyline line = new PolylineClass();
                line.FromPoint = first;
                line.ToPoint = second;
                base.SetValue(line as IGeometry);
            }
        }

        public sealed class ParameterLimits : IOrderParameter
        {
            class Boundaries
            {
                public ParameterLimit first = new ParameterLimit("Boundary limit 1");
                public ParameterLimit second = new ParameterLimit("Boundary limit 2");
            };            
            
            Boundaries m_limits = new Boundaries();
            IFeature m_potential;

            public ParameterLimits()
            {
                IFeatureWorkspace ws = (IFeatureWorkspace)Tools.OpenWorkspace(Tools.GetSwordExtension().Config.PopulationFile);
                IGeoDataset dataset = (IGeoDataset)ws.OpenFeatureClass("Population");
                m_limits.first.UpdateGeometry(dataset.Extent.UpperLeft, dataset.Extent.LowerLeft);
                m_limits.second.UpdateGeometry(dataset.Extent.UpperRight, dataset.Extent.LowerRight);
            }

            public void Serialize(ITable table, int id)
            {
                m_limits.first.Serialize(table, id);
                m_limits.second.Serialize(table, id);
            }

            public void OnContextMenu(MultiItemContextMenu menu, int x, int y, IFeature selected)
            {
                if (menu == null || selected == null || selected.Class.AliasName != "BoundaryLimits")
                    return;
                m_potential = selected;
                menu.Add("Set limit 1", m_limits.first);
                menu.Add("Set limit 2", m_limits.second);
            }

            public void SetValue(string value)
            {
                if (m_potential == null)
                    return;
                if (value == "Set limit 1")
                    m_limits.first.UpdateGeometry(m_potential.Shape);
                if (value == "Set limit 2")
                    m_limits.second.UpdateGeometry(m_potential.Shape);
            }

            public void NotifyUpdate(IMissionObserver observer)
            {
                if (observer != null)
                    observer.Update(this, m_limits.first.ToString(), m_limits.second.ToString());
            }

            public string Name
            {
                get
                {
                    return "Boundary limits";
                }
            }
        }
    }
}
