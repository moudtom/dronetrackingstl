
// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

// C++ API headers
#include "Basemap.h"
#include "myarcgismap.h"

// Other headers
#include "Map.h"
#include "MapGraphicsView.h"
#include "ArcGISRuntimeEnvironment.h"
#include "GraphicsOverlay.h"
#include "PolylineBuilder.h"
#include "PolygonBuilder.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleFillSymbol.h"

using namespace Esri::ArcGISRuntime;

myArcgisMap::myArcgisMap(QObject *parent) : QObject(parent)
{
    // Create a map using the ArcGISNavigation BasemapStyle
    m_map = new Map(BasemapStyle::ArcGISNavigation, this);

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);

    // Set map to map view
    m_mapView->setMap(m_map);

    // set the mapView as the central widget
    setCentralWidget(m_mapView);

    setupMap();

    GraphicsOverlay* overlay = new GraphicsOverlay(this);
    createGraphics(overlay);
    m_mapView->graphicsOverlays()->append(overlay);
}
