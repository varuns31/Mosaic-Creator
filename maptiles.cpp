/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    if(theTiles.empty())return NULL;
    int rows=theSource.getRows();
    int columns=theSource.getColumns();
    if(rows==0 || columns==0)return NULL;
    vector<Point<3>> tiletopoint;
    map<Point<3>,int> tilePos;
    int vectorsize=theTiles.size(); 
    for(int i=0;i<vectorsize;i++)
    {
        LUVAPixel pixel = theTiles[i].getAverageColor();
    	Point<3>point = convertToXYZ(pixel);
        tiletopoint.push_back(point);
        tilePos[point]=i;
    }
    KDTree<3>* tree=new KDTree<3>(tiletopoint);
    MosaicCanvas* answer=new MosaicCanvas(rows,columns);
    for(int i = 0 ; i < rows ; i++)
    {
        for(int j = 0 ; j < columns ; j++)
        {
            LUVAPixel avgColor = theSource.getRegionColor(i, j);
    		Point<3> avgColorPoint = convertToXYZ(avgColor);
    		Point<3> nearestPoint = tree->findNearestNeighbor(avgColorPoint);
            answer->setTile(i,j,&(theTiles[tilePos[nearestPoint]]));
        }
    }
    delete(tree);
    return answer;
}
