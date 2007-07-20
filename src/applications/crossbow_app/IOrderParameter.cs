using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public interface IOrderParameter
    {
        string Name { get; }
        void OnContextMenu(ICommandBar cmd, int x, int y, IFeature selected);
        void Serialize(ITable table, int oid);
    }
}
