using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace Sword
{
    namespace Crossbow
    {
        public interface IOrderParameter
        {
            string Name { get; }
            void OnContextMenu(MultiItemContextMenu menu, int x, int y, IFeature selected);
            void SetValue(string value);
            bool IsSet(string value);
            void Serialize(ITable table, int id);
            void NotifyUpdate(IMissionObserver observer);
        }
    }
}